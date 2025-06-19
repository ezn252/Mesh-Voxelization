"""Fill occupancy grid interiors using 6‑connected flood fill."""
import h5py, numpy as np, argparse, collections

parser = argparse.ArgumentParser()
parser.add_argument('input', help='.h5 with surface occupancy')
parser.add_argument('output')
args = parser.parse_args()

with h5py.File(args.input, 'r') as hf:
    occ = hf['volume'][...].astype(np.uint8)

filled = occ.copy()
D, H, W = filled.shape
q = collections.deque()

# seed exterior voxels (faces)
for z in [0, D-1]:
    for y in range(H):
        for x in range(W):
            if filled[z,y,x] == 0:
                q.append((z,y,x)); filled[z,y,x] = 2
for z in range(D):
    for y in [0, H-1]:
        for x in range(W):
            if filled[z,y,x] == 0:
                q.append((z,y,x)); filled[z,y,x] = 2
    for y in range(H):
        for x in [0, W-1]:
            if filled[z,y,x] == 0:
                q.append((z,y,x)); filled[z,y,x] = 2

# 6‑connected flood fill
while q:
    z,y,x = q.popleft()
    for dz,dy,dx in ((1,0,0),(-1,0,0),(0,1,0),(0,-1,0),(0,0,1),(0,0,-1)):
        nz,ny,nx = z+dz, y+dy, x+dx
        if 0<=nz<D and 0<=ny<H and 0<=nx<W and filled[nz,ny,nx]==0:
            filled[nz,ny,nx] = 2
            q.append((nz,ny,nx))
# voxels still 0 are interior
solid = (filled!=2).astype(np.uint8)

with h5py.File(args.output, 'w') as hf:
    hf.create_dataset('volume', data=solid, compression='gzip')
print('saved', args.output)
