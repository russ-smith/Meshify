#version 440 core

layout(local_size_x = 4, local_size_y = 4, local_size_z = 4) in;
layout(binding = 0) uniform usampler3D points;
layout(binding = 1) uniform writeonly uimage3D cases;

void main(){
	ivec3 id = ivec3(gl_GlobalInvocationID);
	uint cubeCase = texelFetch(points, id, 0).r;
	cubeCase |= texelFetch(points, id + ivec3(1,0,0), 0).r << 1;
	cubeCase |= texelFetch(points, id + ivec3(1,1,0), 0).r << 2;
	cubeCase |= texelFetch(points, id + ivec3(0,1,0), 0).r << 3;
	cubeCase |= texelFetch(points, id + ivec3(0,0,1), 0).r << 4;
	cubeCase |= texelFetch(points, id + ivec3(1,0,1), 0).r << 5;
	cubeCase |= texelFetch(points, id + ivec3(1,1,1), 0).r << 6;
	cubeCase |= texelFetch(points, id + ivec3(0,1,1), 0).r << 7;
	imageStore(cases, id, uvec4(cubeCase,0,0,0));
}
