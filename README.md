# Parallel Mesh Voxelization Toolkit

Convert watertight triangular meshes to **signed‑distance fields (SDFs)** or **occupancy volumes** at lightning speed.
The core is a multithreaded C++ engine, complemented by lightweight Python helpers for data preparation and visualisation.

---

## Why use it?

* \*\*Fast & Parallel \*\* OpenMP‑driven code voxelises large batches in seconds.
* \*\*Two Outputs, One Tool \*\* Produce SDFs for sub‑voxel surface recovery *or* raw occupancy grids for discrete tasks.
* \*\*Corner or Centre Reference \*\* Choose the voxel corner (default) or the centre as the SDF sampling point.
* **Colour‑Aware** Mesh face colours can be preserved in occupancy mode, enabling colourful voxel renders.
* \*\*HDF5 Everywhere \*\* All volumes are stored as portable `.h5` files.

---

## Citation

If this toolkit supports your research, please reference the original method it builds upon:

```bibtex
@inproceedings{Stutz2018CVPR,
  title     = {Learning 3D Shape Completion from Laser Scan Data with Weak Supervision},
  author    = {Stutz, David and Geiger, Andreas},
  booktitle = {Proc. IEEE/CVF Conf. on Computer Vision and Pattern Recognition (CVPR)},
  year      = {2018}
}
@misc{Stutz2017,
  author       = {David Stutz},
  title        = {Learning Shape Completion from Bounding Boxes with CAD Shape Priors},
  institution  = {RWTH Aachen University},
  year         = {2017},
  howpublished = {\url{http://davidstutz.de/}}
}
```

---

## Quick Start

```bash
# Clone and build
mkdir build && cd build
cmake ..
make -j$(nproc)

# Voxelise all meshes in `examples/input` to 32³ occupancy grids
../bin/voxelize occ ../examples/input ../examples/occ32.h5

# …or create 64³ signed‑distance volumes using voxel centres
../bin/voxelize sdf --height 64 --width 64 --depth 64 --center \
                 ../examples/input ../examples/sdf64.h5
```

Visualise SDFs with marching cubes:

```bash
python ../examples/marching_cubes.py ../examples/sdf64.h5 ../examples/mesh_out/
```

---

## Installation

### C++ core

| Requirement                           | Notes                         |
| ------------------------------------- | ----------------------------- |
| CMake ≥ 3.12                          |                               |
| C++11‑compatible compiler with OpenMP | tested with GCC & Clang       |
| Boost                                 | filesystem & program\_options |
| HDF5                                  | I/O backend                   |
| Eigen 3 (incl. *unsupported/Tensor*)  |                               |

Ubuntu example:

```bash
sudo apt-get install build-essential cmake libboost-all-dev libhdf5-dev libeigen3-dev
```

Ensure your compiler exposes `-fopenmp` and that Eigen’s *Tensor* headers are present.

### Python utilities

```bash
pip install numpy h5py scikit-image   # or install PyMCubes as a substitute for scikit‑image
```

---

## Command Reference

`voxelize [mode] [options] <input> <output.h5>`

| Flag       | Default | Description                                    |
| ---------- | ------- | ---------------------------------------------- |
| `mode`     | `occ`   | `occ` for occupancy, `sdf` for signed distance |
| `--height` | 32      | Voxel grid height *(Y/up)*                     |
| `--width`  | 32      | Grid width *(X/right)*                         |
| `--depth`  | 32      | Grid depth *(Z/forward)*                       |
| `--center` |  ‑      | Sample SDF at voxel centres instead of corners |
| `--color`  |  ‑      | Preserve face colours (occupancy mode only)    |

`input` can be a single `.off` file or a directory of consecutively numbered OFF files (zero padding allowed).

---

## Workflow in a Nutshell

1. **Scale** meshes into the target volume:

   ```bash
   python examples/scale_off.py examples/raw/ examples/input/ --res 32 --color
   ```
2. **Voxelise** to SDF or occupancy volumes (see *Quick Start* above).
3. *(Optional)* **Fill** occupancy grids to obtain solid interiors:
   `python examples/fill_occupancy.py occ32.h5 filled.h5`
4. **Reconstruct** triangle meshes for visualisation:
   `python examples/occ_to_off.py filled.h5 mesh_out/ --color examples/input/*_color.h5`

---

## Acknowledgements

The toolkit incorporates code from the following open‑source projects:

* **SDFGen** by Christopher Batty – triangle‑to‑point distance
* **Ray–Triangle & Box–Triangle Intersections** by Tomas Akenine‑Möller
* **cmake‑modules** by Tronic

---

## License (Non‑Commercial)

Copyright © 2025 **Vibe Coder**

This software is released for *non‑commercial* scientific, educational and artistic use.
You may copy, modify and distribute the code **free of charge** provided that:

* You include this license notice in all copies/substantial portions of the software.
* You cite the above‐listed papers in any publication or presentation derived from this work.
* You do **not** use the software, in whole or in part, for commercial gain (including integration into commercial products or services).

The software is provided **“as is”** without warranty of any kind. See `LICENSE` for full terms.
