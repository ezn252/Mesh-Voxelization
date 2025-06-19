#include "off_loader.h"
#include <fstream>
#include <sstream>
#include <iostream>

bool load_off(const std::string& path, std::vector<Triangle>& tris)
{
    std::ifstream in(path);
    if (!in) {
        std::cerr << "Cannot open " << path << std::endl;
        return false;
    }

    std::string header;
    in >> header;
    if (header != "OFF") {
        std::cerr << path << " is not a valid OFF file." << std::endl;
        return false;
    }

    size_t vcount, fcount, ecount;
    in >> vcount >> fcount >> ecount;

    std::vector<Eigen::Vector3f> vertices(vcount);
    for (size_t i = 0; i < vcount; ++i) {
        float x, y, z;
        in >> x >> y >> z;
        vertices[i] = {x, y, z};
    }

    tris.reserve(fcount);
    for (size_t i = 0; i < fcount; ++i) {
        int verts;
        in >> verts;
        if (verts != 3) {
            std::cerr << "Only triangular faces are supported." << std::endl;
            return false;
        }
        int a, b, c;
        in >> a >> b >> c;
        Triangle t;
        t.v0 = vertices[a];
        t.v1 = vertices[b];
        t.v2 = vertices[c];

        // Optional RGB (0-255)
        if (in.peek() != '\n' && in.peek() != EOF) {
            float r, g, bcol;
            in >> r >> g >> bcol;
            t.color = {r / 255.f, g / 255.f, bcol / 255.f};
        } else {
            t.color = {1.f, 1.f, 1.f};
        }
        tris.push_back(t);
    }
    return true;
}
