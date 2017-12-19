#version 440 core

//Builds a new layer of the histopyramids.
//Gathers and sums 2x2x2 samples from the previous layer and writes to the next layer

layout(local_size_x = 4, local_size_y = 4, local_size_z = 4) in;
layout(binding=2) uniform usampler3D oldA;
layout(binding=3) uniform usampler3D oldB;
layout(binding=2) uniform writeonly uimage3D newA;
layout(binding=3) uniform writeonly uimage3D newB;

void main(){
	ivec3 id = ivec3(gl_GlobalInvocationID);
	uint A = 0, B = 0;
	A += texelFetch(oldA, 2*id, 0).r;
	B += texelFetch(oldB, 2*id, 0).r; 
	A += texelFetch(oldA, 2*id+ivec3(1,0,0), 0).r;
	B += texelFetch(oldB, 2*id+ivec3(1,0,0), 0).r; 
	A += texelFetch(oldA, 2*id+ivec3(0,1,0), 0).r;
	B += texelFetch(oldB, 2*id+ivec3(0,1,0), 0).r; 
	A += texelFetch(oldA, 2*id+ivec3(1,1,0), 0).r;
	B += texelFetch(oldB, 2*id+ivec3(1,1,0), 0).r; 
	A += texelFetch(oldA, 2*id+ivec3(0,0,1), 0).r;
	B += texelFetch(oldB, 2*id+ivec3(0,0,1), 0).r; 
	A += texelFetch(oldA, 2*id+ivec3(1,0,1), 0).r;
	B += texelFetch(oldB, 2*id+ivec3(1,0,1), 0).r; 
	A += texelFetch(oldA, 2*id+ivec3(0,1,1), 0).r;
	B += texelFetch(oldB, 2*id+ivec3(0,1,1), 0).r; 
	A += texelFetch(oldA, 2*id+ivec3(1,1,1), 0).r;
	B += texelFetch(oldB, 2*id+ivec3(1,1,1), 0).r; 

	imageStore(newA, id, uvec4(A, 0,0,0));
	imageStore(newB, id, uvec4(B, 0,0,0));
}