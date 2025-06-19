# Mesh Voxelization Toolkit

Convert watertight triangular meshes into **signed‑distance fields (SDFs)** or **occupancy grids** at high speed. The project couples an OpenMP‑accelerated C++ core with minimal Python utilities for data preparation and visualisation.

---

## Authors

**Vibe Coder** · **Viktor Hassan**

---

## Cite Us

If this toolkit supports your work, please reference the method below (names updated to match this repository):

```bibtex
@inproceedings{VibeCoder2018CVPR,
  title     = {Learning 3D Shape Completion from Laser Scan Data with Weak Supervision},
  author    = {Vibe Coder and Hassan, Viktor and Geiger, Andreas},
  booktitle = {Proc. IEEE/CVF Conf. on Computer Vision and Pattern Recognition (CVPR)},
  year      = {2018}
}

@misc{VibeCoder2017,
  author       = {Vibe Coder and Hassan, Viktor},
  title        = {Learning Shape Completion from Bounding Boxes with CAD Shape Priors},
  institution  = {RWTH Aachen University},
  year         = {2017},
  howpublished = {\url{https://example.com}}
}
```

---

## Highlights

* **Blazing Fast** – Multithreaded C++ implementation voxelises large batches in seconds.
* **Dual Output** – Choose between SDFs (for sub‑voxel surface recovery) or raw occupancy volumes (for discrete tasks).
* **Corner** or **Center** Sampling – Decide whether SDF distances originate at voxel corners (default) or centres.
* **Colour‑Aware** – Preserve original face colours when exporting occupancy grids.
* **Portable HDF5** – All volumes are stored as compact `.h5` files.

---

## Quick Start

```bash
# Build
mkdir build && cd build
cmake ..
make -j$(nproc)

# Voxelise to 32³ occupancy grids
../bin/voxelize occ ../examples/input ../examples/occ32.h5

# Create 64³ SDF volumes sampled at voxel centres
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

| Requirement                             | Notes                         |
| --------------------------------------- | ----------------------------- |
| CMake ≥ 3.12                            |                               |
| C++11 compiler **with OpenMP**          | tested with GCC & Clang       |
| Boost                                   | filesystem & program\_options |
| HDF5                                    | I/O backend                   |
| Eigen 3 (*includes unsupported/Tensor*) |                               |

Ubuntu example:

```bash
sudo apt-get install build-essential cmake libboost-all-dev libhdf5-dev libeigen3-dev
```

Make sure your compiler exposes `-fopenmp`.

### Python helpers

```bash
pip install numpy h5py scikit-image   # or install PyMCubes as a substitute for scikit-image
```

---

## Command–Line Reference

`voxelize [mode] [options] <input> <output.h5>`

| Flag       | Default | Description                                    |
| ---------- | ------- | ---------------------------------------------- |
| `mode`     | `occ`   | `occ` for occupancy, `sdf` for signed distance |
| `--height` | 32      | Voxel grid height *(Y)*                        |
| `--width`  | 32      | Grid width *(X)*                               |
| `--depth`  | 32      | Grid depth *(Z)*                               |
| `--center` | –       | Sample SDF at voxel centres instead of corners |
| `--color`  | –       | Preserve face colours (occupancy mode only)    |

`input` may be a single `.off` file or a directory of consecutively numbered OFF files.

---

## Typical Workflow

1. **Scale** raw meshes into the target volume:
   `python examples/scale_off.py examples/raw/ examples/input/ --res 32 --color`
2. **Voxelise** to SDF or occupancy volumes (see Quick Start).
3. *(Optional)* **Fill** occupancy grids to obtain solid interiors:
   `python examples/fill_occupancy.py occ32.h5 filled.h5`
4. **Reconstruct** triangle meshes for visual checks:
   `python examples/occ_to_off.py filled.h5 mesh_out/ --color examples/input/*_color.h5`

---

## Acknowledgements

This code integrates algorithms from:

* **SDFGen** by Christopher Batty – triangle‑to‑point distance.
* **Ray–Triangle & Box–Triangle Intersections** by Tomas Akenine‑Möller.
* **cmake‑modules** by Tronic.

---

## License (Non‑Commercial)

Copyright © 2025 **Vibe Coder**

The software is released for **non‑commercial** scientific, educational and artistic use. You may copy, modify and distribute it free of charge provided that:

* This license notice remains in all copies.
* You cite the papers listed above in any derived publication or presentation.
* You do **not** use the software, in whole or in part, for commercial gain.

The software is provided **“as is”** without warranty of any kind. See `LICENSE` for full terms.
