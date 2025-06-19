#include "voxelization.h"
#include <algorithm>
#include <limits>
#include <cmath>
#include <omp.h>

using Vec3 = Eigen::Vector3f;

// Helper: point to triangle distance
static float pointTriDist(const Vec3& p, const Triangle& tri)
{
    const Vec3& a = tri.v0;
    const Vec3& b = tri.v1;
    const Vec3& c = tri.v2;

    // Compute vectors
    Vec3 ab = b - a;
    Vec3 ac = c - a;
    Vec3 ap = p - a;

    float d1 = ab.dot(ap);
    float d2 = ac.dot(ap);

    if (d1 <= 0.f && d2 <= 0.f) return (p - a).norm(); // bary (1,0,0)

    Vec3 bp = p - b;
    float d3 = ab.dot(bp);
    float d4 = ac.dot(bp);
    if (d3 >= 0.f && d4 <= d3) return (p - b).norm(); // bary (0,1,0)

    float vc = d1 * d4 - d3 * d2;
    if (vc <= 0.f && d1 >= 0.f && d3 <= 0.f) {
        float v = d1 / (d1 - d3);
        Vec3 proj = a + v * ab;
        return (p - proj).norm();
    }

    Vec3 cp = p - c;
    float d5 = ab.dot(cp);
    float d6 = ac.dot(cp);
    if (d6 >= 0.f && d5 <= d6) return (p - c).norm(); // bary(0,0,1)

    float vb = d5 * d2 - d1 * d6;
    if (vb <= 0.f && d2 >= 0.f && d6 <= 0.f) {
        float w = d2 / (d2 - d6);
        Vec3 proj = a + w * ac;
        return (p - proj).norm();
    }

    float va = d3 * d6 - d5 * d4;
    if (va <= 0.f && (d4 - d3) >= 0.f && (d5 - d6) >= 0.f) {
        float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
        Vec3 proj = b + w * (c - b);
        return (p - proj).norm();
    }

    // Inside face region
    Vec3 n = ab.cross(ac).normalized();
    return fabs((p - a).dot(n));
}

static bool rayIntersectsTriangle(const Vec3& orig, const Vec3& dir, const Triangle& tri)
{
    const float EPS = 1e-6f;
    Vec3 edge1 = tri.v1 - tri.v0;
    Vec3 edge2 = tri.v2 - tri.v0;
    Vec3 h = dir.cross(edge2);
    float a = edge1.dot(h);
    if (fabs(a) < EPS) return false; // Parallel
    float f = 1.f / a;
    Vec3 s = orig - tri.v0;
    float u = f * s.dot(h);
    if (u < 0.f || u > 1.f) return false;
    Vec3 q = s.cross(edge1);
    float v = f * dir.dot(q);
    if (v < 0.f || u + v > 1.f) return false;
    float t = f * edge2.dot(q);
    return t > EPS;
}

void voxelize_occupancy(const std::vector<Triangle>& tris,
                        const VolumeDims& dims,
                        std::vector<uint8_t>& vol)
{
    const int H = dims.H, W = dims.W, D = dims.D;
    vol.assign(H * W * D, 0);

    // Compute overall AABB of mesh
    Vec3 minb = tris[0].v0;
    Vec3 maxb = tris[0].v0;
    for (const auto& t : tris) {
        minb = minb.cwiseMin(t.v0).cwiseMin(t.v1).cwiseMin(t.v2);
        maxb = maxb.cwiseMax(t.v0).cwiseMax(t.v1).cwiseMax(t.v2);
    }
    Vec3 scale = (maxb - minb);
    float maxSide = scale.maxCoeff();
    scale = Vec3::Ones() * (maxSide > 0 ? maxSide : 1.f);

    Vec3 invScale = Vec3( (W - 1) / scale.x(),
                          (H - 1) / scale.y(),
                          (D - 1) / scale.z() );

    auto posToIdx = [&](int y, int x, int z){ return (z * H + y) * W + x; };

    // For each triangle, mark intersecting voxels using its bounding box
    #pragma omp parallel for
    for (size_t i = 0; i < tris.size(); ++i) {
        const auto& t = tris[i];
        Vec3 v0 = (t.v0 - minb).cwiseQuotient(scale); // 0..1
        Vec3 v1 = (t.v1 - minb).cwiseQuotient(scale);
        Vec3 v2 = (t.v2 - minb).cwiseQuotient(scale);

        Vec3 bboxMin = v0.cwiseMin(v1).cwiseMin(v2);
        Vec3 bboxMax = v0.cwiseMax(v1).cwiseMax(v2);

        int y0 = std::max(0, std::min(H-1, (int)std::floor(bboxMin.y() * (H-1))));
        int y1 = std::max(0, std::min(H-1, (int)std::ceil (bboxMax.y() * (H-1))));
        int x0 = std::max(0, std::min(W-1, (int)std::floor(bboxMin.x() * (W-1))));
        int x1 = std::max(0, std::min(W-1, (int)std::ceil (bboxMax.x() * (W-1))));
        int z0 = std::max(0, std::min(D-1, (int)std::floor(bboxMin.z() * (D-1))));
        int z1 = std::max(0, std::min(D-1, (int)std::ceil (bboxMax.z() * (D-1))));

        for (int z = z0; z <= z1; ++z)
            for (int y = y0; y <= y1; ++y)
                for (int x = x0; x <= x1; ++x)
                    vol[posToIdx(y,x,z)] = 1;
    }
}

void voxelize_sdf(const std::vector<Triangle>& tris,
                  const VolumeDims& dims,
                  bool use_centers,
                  std::vector<float>& sdf)
{
    const int H = dims.H, W = dims.W, D = dims.D;
    sdf.assign(H * W * D, 0.f);

    Vec3 minb = tris[0].v0;
    Vec3 maxb = tris[0].v0;
    for (const auto& t : tris) {
        minb = minb.cwiseMin(t.v0).cwiseMin(t.v1).cwiseMin(t.v2);
        maxb = maxb.cwiseMax(t.v0).cwiseMax(t.v1).cwiseMax(t.v2);
    }
    Vec3 scale = (maxb - minb);
    float maxSide = scale.maxCoeff();
    scale = Vec3::Ones() * (maxSide > 0.f ? maxSide : 1.f);

    Vec3 cell = scale.cwiseQuotient(Vec3(W-1, H-1, D-1));
    Vec3 half = use_centers ? cell * 0.5f : Vec3::Zero();

    auto idx = [&](int y,int x,int z){return (z*H+y)*W+x;};

    #pragma omp parallel for collapse(3)
    for (int z = 0; z < D; ++z) {
        for (int y = 0; y < H; ++y) {
            for (int x = 0; x < W; ++x) {
                Vec3 pNorm((float)x/(W-1), (float)y/(H-1), (float)z/(D-1));
                Vec3 p = minb + pNorm.cwiseProduct(scale) + half;

                float dmin = std::numeric_limits<float>::max();
                for (const auto& tri : tris) {
                    dmin = std::min(dmin, pointTriDist(p, tri));
                }

                // Determine sign by ray parity (positive x-direction)
                Vec3 rayDir(1.f, 0.f, 0.f);
                int hits = 0;
                for (const auto& tri : tris) {
                    if (rayIntersectsTriangle(p, rayDir, tri)) ++hits;
                }
                if (hits % 2 == 1) dmin = -dmin;

                sdf[idx(y,x,z)] = dmin;
            }
        }
    }
}
