#pragma once
/**
 * voxelization.h
 * Core CPU voxelization routines.
 */
#include <vector>
#include <Eigen/Core>
#include "off_loader.h"

struct VolumeDims { int H, W, D; };

// Occupancy grid: uint8_t (0 = empty, 1 = surface)
void voxelize_occupancy(const std::vector<Triangle>& tris,
                        const VolumeDims& dims,
                        std::vector<uint8_t>& volume);

// Signed distance field: float values (negative = inside)
void voxelize_sdf(const std::vector<Triangle>& tris,
                  const VolumeDims& dims,
                  bool use_centers,
                  std::vector<float>& sdf);
