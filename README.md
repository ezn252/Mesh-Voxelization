# 🧊 Mesh Voxelization Toolkit

A high-performance tool for converting watertight triangular meshes into **signed distance fields (SDFs)** or **occupancy grids**. Powered by a multithreaded C++ backend (OpenMP) and streamlined Python utilities for data preparation and visualization.

---

## 👤 Author

**EZN (Thomas Chapple)**

---

## 📚 Citation

If you use this toolkit in your research, please cite:

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
  year         = {2025}
}
```

---

## ✨ Features

- ⚡ **Fast & Multithreaded** – High-speed C++ backend with OpenMP
- 🎯 **Dual Output** – Signed distance fields and occupancy volumes
- 🔧 **Custom Sampling** – Corner-based or center-based SDF sampling
- 🎨 **Color-Preserving** – Retains face colors in occupancy mode
- 📦 **Compact Output** – HDF5 `.h5` format for all outputs

---

## 🚀 Quick Start

### 🔨 Build the core

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### 🔲 Voxelize to 32³ occupancy grid

```bash
../bin/voxelize occ ../examples/input ../examples/occ32.h5
```

### 🔳 Voxelize to 64³ SDF volume (center sampling)

```bash
../bin/voxelize sdf --height 64 --width 64 --depth 64 --center \
                 ../examples/input ../examples/sdf64.h5
```

### 🧊 Visualize SDF volumes

```bash
python ../examples/marching_cubes.py ../examples/sdf64.h5 ../examples/mesh_out/
```

---

## 🔧 Installation

### 🛠️ C++ Core Requirements

| Dependency                             | Purpose                              |
|----------------------------------------|--------------------------------------|
| CMake ≥ 3.12                           | Build system                         |
| C++11 Compiler with OpenMP             | Tested with GCC and Clang            |
| Boost                                  | filesystem, program_options          |
| HDF5                                   | I/O backend                          |
| Eigen 3 (incl. unsupported/Tensor)     | Tensor operations                    |

Install on Ubuntu:

```bash
sudo apt-get install build-essential cmake libboost-all-dev libhdf5-dev libeigen3-dev
```

> ✅ Ensure your compiler supports OpenMP (`-fopenmp`)

---

### 🐍 Python Dependencies

```bash
pip install numpy h5py scikit-image
# Or use PyMCubes instead of scikit-image
```

---

## 🛠️ Command-Line Usage

```bash
voxelize [mode] [options] <input> <output.h5>
```

| Option      | Default | Description                                          |
|-------------|---------|------------------------------------------------------|
| `mode`      | `occ`   | `occ` for occupancy grid, `sdf` for signed distance |
| `--height`  | `32`    | Grid height (Y axis)                                 |
| `--width`   | `32`    | Grid width (X axis)                                  |
| `--depth`   | `32`    | Grid depth (Z axis)                                  |
| `--center`  | –       | Use voxel centers for SDF sampling                  |
| `--color`   | –       | Retain mesh face colors (occupancy only)            |

`<input>` can be a single `.off` file or a directory of numbered `.off` files.

---

## 🔄 Workflow Example

```bash
# 1. Scale raw meshes
python examples/scale_off.py examples/raw/ examples/input/ --res 32 --color

# 2. Voxelize
../bin/voxelize sdf ../examples/input ../examples/sdf32.h5

# 3. (Optional) Fill interiors
python examples/fill_occupancy.py occ32.h5 filled.h5

# 4. Convert to .off mesh
python examples/occ_to_off.py filled.h5 mesh_out/ --color examples/input/*_color.h5
```

---

## 🙏 Acknowledgements

- **SDFGen** by Christopher Batty – triangle-to-point SDF distance  
- **Intersection Algorithms** by Tomas Akenine-Möller  
- **CMake Modules** by Tronic  

---

## 📄 License – Non-Commercial Use

© 2025 **EZN (Thomas Chapple)** / **Vibe Coder**

This software is released under a **non-commercial license**. You may:

- ✅ Use, copy, and modify it for scientific, academic, or artistic purposes  
- ✅ Retain this license notice in all copies  
- ✅ Cite the referenced papers in any derivative works  
- ❌ **Not** use this software or any derivatives for commercial purposes  

The software is provided **“as is”** without any warranty. See `LICENSE` for full terms.
