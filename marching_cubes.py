"""Convert SDF volume to mesh using skimage.marching_cubes."""
import argparse, os, h5py, numpy as np
from skimage import measure

parser = argparse.ArgumentParser()
parser.add_argument('sdf_h5')
parser.add_argument('out_dir')
parser.add_argument('--level', type=float, default=0.0)
args = parser.parse_args()

os.makedirs(args.out_dir, exist_ok=True)
with h5py.File(args.sdf_h5, 'r') as hf:
    sdf = hf['volume'][...]

verts, faces, _, _ = measure.marching_cubes(sdf, level=args.level)

# Save as OFF
off_path = os.path.join(args.out_dir, 'recon.off')
with open(off_path, 'w') as f:
    f.write('OFF\n')
    f.write(f"{len(verts)} {len(faces)} 0\n")
    for v in verts:
        f.write("{0:.6f} {1:.6f} {2:.6f}\n".format(*v))
    for tri in faces:
        f.write(f"3 {tri[0]} {tri[1]} {tri[2]}\n")
print('Mesh written to', off_path)
