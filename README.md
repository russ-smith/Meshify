# Meshify

A cleanup / rewrite of my thesis project from 2015/16.

Built using openFrameworks.

Meshify takes a shape expressed as a distance function in GLSL, and runs isosurface 
extraction algorithms on GPU using OpenGL compute shaders, to extract an indexed mesh.
It combines the histogram pyramid technique described by Dyken et al at 
http://heim.ifi.uio.no/~erikd/pdf/hpmarcher_draft.pdf
with Geiss' technique for sharing vertices across cubes at
https://developer.nvidia.com/gpugems/GPUGems3/gpugems3_ch01.html

Marching Cubes currently working, Dual Marching Cubes coming soon.

For instructions on building and usage, and a brief algorithm description, 
see the doc folder.

## TODO

Finish Dual Marching Cubes implementation

Add more distance functions

Better lighting and material parameters

Add mesh export


