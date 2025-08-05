Mesh Voxelization Toolkit
A high-performance toolkit for converting watertight triangular meshes into signed distance fields (SDFs) or occupancy grids. This project combines a multithreaded C++ backend (accelerated via OpenMP) with lightweight Python utilities for data preparation and visualization.

Authors
EZN (Thomas Chapple)

Citation
If you use this toolkit in your research or projects, please cite the following works (authors updated to match this repository):

bibtex
Copy
Edit
@inproceedings{VibeCoder2018CVPR,
  title     = {Learning 3D Shape Completion from Laser Scan Data with Weak Supervision},
  author    = {Vibe Coder and Olsen, Jack and Geiger, Andreas},
  booktitle = {Proc. IEEE/CVF Conf. on Computer Vision and Pattern Recognition (CVPR)},
  year      = {2018}
}

@misc{Ezn252,
  author       = {EZN - Chapple, Thomas},
  title        = {Learning Shape Completion from Bounding Boxes with CAD Shape Priors},
  institution  = {RWTH Aachen University},
  year         = {2025},
}
Key Features
High Performance – Multithreaded C++ core handles large mesh batches in seconds.

Flexible Output – Generate SDF volumes for fine-grained surface approximation, or binary occupancy grids for discrete applications.

Voxel Sampling Control – Choose between corner-based (default) or center-based SDF sampling.

Color Support – Retains face-level color information when exporting occupancy volumes.

Compact Output Format – All volumetric data is stored as .h5 files using the HDF5 standard.

Quick Start
bash
Copy
Edit
# Build the C++ backend
mkdir build && cd build
cmake ..
make -j$(nproc)

# Convert meshes to 32³ occupancy grids
../bin/voxelize occ ../examples/input ../examples/occ32.h5

# Convert meshes to 64³ SDF volumes (center sampling)
../bin/voxelize sdf --height 64 --width 64 --depth 64 --center \
                 ../examples/input ../examples/sdf64.h5
Visualize SDF volumes using marching cubes:

bash
Copy
Edit
python ../examples/marching_cubes.py ../examples/sdf64.h5 ../examples/mesh_out/
Installation
C++ Core Dependencies
Dependency	Notes
CMake ≥ 3.12	
C++11 compiler with OpenMP	Tested with GCC and Clang
Boost	Requires filesystem and program_options
HDF5	For file I/O
Eigen 3 (including unsupported/Tensor)	

To install on Ubuntu:

bash
Copy
Edit
sudo apt-get install build-essential cmake libboost-all-dev libhdf5-dev libeigen3-dev
Ensure your compiler supports OpenMP (-fopenmp).

Python Utilities
Install required Python packages:

bash
Copy
Edit
pip install numpy h5py scikit-image
# Alternatively, use PyMCubes in place of scikit-image
Command-Line Usage
bash
Copy
Edit
voxelize [mode] [options] <input> <output.h5>
Argument	Default	Description
mode	occ	occ for occupancy grid, sdf for signed distance
--height	32	Height (Y-axis resolution)
--width	32	Width (X-axis resolution)
--depth	32	Depth (Z-axis resolution)
--center	–	Use voxel center points for SDF sampling
--color	–	Retain face colors (occupancy mode only)

The input may be a single .off file or a directory of sequentially numbered .off files.

Typical Workflow
Scale meshes to the target voxel resolution:

bash
Copy
Edit
python examples/scale_off.py examples/raw/ examples/input/ --res 32 --color
Voxelize the meshes (as shown in Quick Start).

(Optional) Fill occupancy grids to create solid interiors:

bash
Copy
Edit
python examples/fill_occupancy.py occ32.h5 filled.h5
Reconstruct meshes from voxel data:

bash
Copy
Edit
python examples/occ_to_off.py filled.h5 mesh_out/ --color examples/input/*_color.h5
Acknowledgements
This toolkit incorporates components from the following works:

SDFGen – For triangle-to-point distance computation (Christopher Batty)

Triangle/Box Intersection Algorithms – By Tomas Akenine-Möller

CMake Modules – Adapted from Tronic’s cmake-modules repository

License (Non‑Commercial Use Only)
Copyright © 2025
EZN (Thomas Chapple) / Vibe Coder

This software is released for non-commercial use in academic, educational, and artistic contexts. Redistribution and modification are permitted under the following conditions:

This license notice must be retained in all copies.

You must cite the papers listed in the Citations section in any publications or derivative works.

You may not use the software for any commercial purpose.

This software is provided “as is”, without any warranties or guarantees. See the LICENSE file for full legal terms.
