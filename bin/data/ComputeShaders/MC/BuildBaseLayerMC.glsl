#version 440 core

//This shader builds the first layer of the 2 histopyramids (vertices and triangles).
//Each invocation takes 2x2x2 samples from the cube-cases grid
//The numbers of triangles and new vertices are looked up for each cube case, summed,
//and written into the histopyramids at half-resolution.

layout(local_size_x = 4, local_size_y = 4, local_size_z = 4) in;
layout(binding = 1) uniform usampler3D cubeCases;
layout(binding = 11) uniform usampler1D numTris;
layout(binding = 12) uniform usampler1D newEdges;
layout(binding = 2) uniform writeonly uimage3D edgePyramid;
layout(binding = 3) uniform writeonly uimage3D triPyramid;

void main(){
	ivec3 id = ivec3(gl_GlobalInvocationID);
	uint edges = 0, tris = 0;
	int currCase = int (texelFetch(cubeCases, 2*id, 0).r);
	tris += texelFetch(numTris, currCase, 0).r;
	edges += texelFetch(newEdges, currCase >> 2, 0).a;
	currCase = int (texelFetch(cubeCases, 2*id+ivec3(1,0,0), 0).r);
	tris += texelFetch(numTris, currCase, 0).r;
	edges += texelFetch(newEdges, currCase >> 2, 0).a;
	currCase = int (texelFetch(cubeCases, 2*id+ivec3(0,1,0), 0).r);
	tris += texelFetch(numTris, currCase, 0).r;
	edges += texelFetch(newEdges, currCase >> 2, 0).a;
	currCase = int (texelFetch(cubeCases, 2*id+ivec3(1,1,0), 0).r);
	tris += texelFetch(numTris, currCase, 0).r;
	edges += texelFetch(newEdges, currCase >> 2, 0).a;
	currCase = int (texelFetch(cubeCases, 2*id+ivec3(0,0,1), 0).r);
	tris += texelFetch(numTris, currCase, 0).r;
	edges += texelFetch(newEdges, currCase >> 2, 0).a;
	currCase = int (texelFetch(cubeCases, 2*id+ivec3(1,0,1), 0).r);
	tris += texelFetch(numTris, currCase, 0).r;
	edges += texelFetch(newEdges, currCase >> 2, 0).a;
	currCase = int (texelFetch(cubeCases, 2*id+ivec3(0,1,1), 0).r);
	tris += texelFetch(numTris, currCase, 0).r;
	edges += texelFetch(newEdges, currCase >> 2, 0).a;
	currCase = int (texelFetch(cubeCases, 2*id+ivec3(1,1,1), 0).r);
	tris += texelFetch(numTris, currCase, 0).r;
	edges += texelFetch(newEdges, currCase >> 2, 0).a;

	imageStore(triPyramid, id, uvec4(tris, 0,0,0));
	imageStore(edgePyramid, id, uvec4(edges, 0,0,0));
}