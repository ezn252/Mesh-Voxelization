    #pragma once
    /**
     * off_loader.h
     * Simple OFF mesh loader (supports vertex positions and optional RGB colors).
     * By Vibe Coder & Viktor Hassan, 2025.
     */
    #include <vector>
    #include <string>
    #include <Eigen/Core>

    struct Triangle {
        Eigen::Vector3f v0, v1, v2;
        Eigen::Vector3f color; // Optional face color (RGB in 0..1)
    };

    /**
     * Load an OFF file.
     * 
@param path          File path.
     * 
@param triangles     Output list of triangles.
     * 
@return              True on success.
     */
    bool load_off(const std::string& path, std::vector<Triangle>& triangles);
