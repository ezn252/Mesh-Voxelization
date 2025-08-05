# Mesh Voxelization Toolkit

Convert watertight triangular meshes into **signed distance fields (SDFs)** or **occupancy grids** at high speed. This project combines a multithreaded C++ backend (with OpenMP acceleration) and lightweight Python utilities for preprocessing and visualization.

---

## 📌 Authors

**EZN (Thomas Chapple)**

---

## 📝 Citation

If this toolkit supports your work, please cite the following:

```bibtex
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
✨ Features
Fast – Multithreaded C++ voxelization processes large batches in seconds.

Flexible Output – Supports both SDFs (for sub-voxel surface capture) and occupancy grids (for discrete tasks).

Sampling Modes – Choose corner-based (default) or center-based SDF sampling.

Color-Preserving – Retain mesh face colors when exporting occupancy volumes.

Portable Format – Outputs compact .h5 volumes via HDF5.

⚡ Quick Start
bash
Copy
Edit
# Build the core
mkdir build && cd build
cmake ..
make -j$(nproc)

# Voxelize to 32³ occupancy grids
../bin/voxelize occ ../examples/input ../examples/occ32.h5

# Create 64³ SDF volumes (center sampling)
../bin/voxelize sdf --height 64 --width 64 --depth 64 --center \
                 ../examples/input ../examples/sdf64.h5
Visualize SDFs with marching cubes:

bash
Copy
Edit
python ../examples/marching_cubes.py ../examples/sdf64.h5 ../examples/mesh_out/
🔧 Installation
C++ Core Dependencies
Dependency	Notes
CMake ≥ 3.12	Build system
C++11 compiler with OpenMP	Tested with GCC and Clang
Boost	Uses filesystem, program_options
HDF5	File I/O backend
Eigen 3 (incl. unsupported/Tensor)	Tensor operations

Ubuntu setup:

bash
Copy
Edit
sudo apt-get install build-essential cmake libboost-all-dev libhdf5-dev libeigen3-dev
Ensure your compiler supports -fopenmp.

Python Utilities
bash
Copy
Edit
pip install numpy h5py scikit-image
# Optional: Use PyMCubes instead of scikit-image for marching cubes
🛠️ Command Line Usage
bash
Copy
Edit
voxelize [mode] [options] <input> <output.h5>
Argument	Default	Description
mode	occ	occ for occupancy grid, sdf for signed distance
--height	32	Grid height (Y-axis)
--width	32	Grid width (X-axis)
--depth	32	Grid depth (Z-axis)
--center	–	Sample SDFs at voxel centers
--color	–	Preserve mesh face colors (occupancy only)

<input> can be a single .off file or a directory of numbered .off files.

🔄 Typical Workflow
Scale input meshes to voxel grid:

bash
Copy
Edit
python examples/scale_off.py examples/raw/ examples/input/ --res 32 --color
Voxelize into occupancy or SDF volumes (see Quick Start).

(Optional) Fill interiors of occupancy volumes:

bash
Copy
Edit
python examples/fill_occupancy.py occ32.h5 filled.h5
Reconstruct triangle meshes from voxel data:

bash
Copy
Edit
python examples/occ_to_off.py filled.h5 mesh_out/ --color examples/input/*_color.h5
🙏 Acknowledgements
This project integrates work from:

SDFGen by Christopher Batty — triangle-to-point SDF

Intersection algorithms by Tomas Akenine-Möller

CMake modules by Tronic

📄 License – Non-Commercial Use
© 2025 EZN (Thomas Chapple) / Vibe Coder

This software is provided under a non-commercial license for academic, scientific, artistic, or educational purposes. You may:

Copy, modify, and distribute it freely

Retain this license notice in all copies

Cite the works listed above in any publications or derived work

Commercial use is strictly prohibited.

The software is provided “as is” without warranty or guarantee. See LICENSE for full terms.
