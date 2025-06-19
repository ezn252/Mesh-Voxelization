"""Scale OFF meshes into a target [0,1]^3 cube so they fit the voxel grid."""
import argparse, os, numpy as np

def load_off(path):
    with open(path) as f:
        if f.readline().strip() != 'OFF':
            raise ValueError('Not OFF')
        v, fcount, _ = map(int, f.readline().split())
        verts = np.loadtxt([next(f) for _ in range(v)])
        faces = []
        for _ in range(fcount):
            parts = list(map(int, next(f).split()))
            assert parts[0] == 3
            faces.append(parts[1:4])
        return verts, np.array(faces)

def save_off(path, verts, faces):
    with open(path, 'w') as f:
        f.write('OFF\n')
        f.write(f"{len(verts)} {len(faces)} 0\n")
        for v in verts:
            f.write("{0:.6f} {1:.6f} {2:.6f}\n".format(*v))
        for tri in faces:
            f.write(f"3 {tri[0]} {tri[1]} {tri[2]}\n")

parser = argparse.ArgumentParser()
parser.add_argument('input_dir')
parser.add_argument('output_dir')
parser.add_argument('--pad', action='store_true', help='keep aspect ratio with padding')
args = parser.parse_args()

os.makedirs(args.output_dir, exist_ok=True)

for fname in os.listdir(args.input_dir):
    if not fname.endswith('.off'): continue
    v, f = load_off(os.path.join(args.input_dir, fname))
    minv, maxv = v.min(0), v.max(0)
    scale = (maxv - minv).max()
    v_norm = (v - minv) / scale
    if args.pad:
        # Center with padding
        bbox = maxv - minv
        pad = (scale - bbox) / (2 * scale)
        v_norm += pad
    save_off(os.path.join(args.output_dir, fname), v_norm, f)
    print('Scaled', fname)
