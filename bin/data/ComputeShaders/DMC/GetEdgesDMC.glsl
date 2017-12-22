#version 440 core

//Descends the pyramid one layer at a time and sums cube values, 
//until location of current edge found in base layer

layout (local_size_x = 64) in;

uniform int layers;
uniform int total;

layout (binding = 1) uniform usampler3D cubeCases;
layout (binding = 2) uniform usampler3D pyramid[8];
layout (binding = 12) uniform usampler1D newVerts;
layout (binding = 0) uniform writeonly uimage3D vertPositions;