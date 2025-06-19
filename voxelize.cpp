/**
 * voxelize.cpp
 * Minimal stub demonstrating CLI argument parsing.
 * Replace with full implementation.
 */
#include <iostream>
#include <string>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: voxelize <mode> [options] <input> <output.h5>\n";
        return 1;
    }
    std::cout << "Voxelization stub — replace with full implementation." << std::endl;
    return 0;
}
