#version 440 core

//Descends the pyramid one layer at a time and sums cube values, 
//until location of current triangle found in base layer
//Uses cube case texture and vertex number texture to find vertex IDs 
//forming triangle, and writes IDs to element buffer

layout (local_size_x = 64) in;

uniform int total;
uniform int layers;

layout (binding = 0) uniform usampler3D vertPositions;
layout (binding = 1) uniform usampler3D cubeCases;
layout (binding = 2) uniform usampler3D pyramid[8];

layout (binding = 11) uniform usampler1D numTris;
layout (binding = 12) uniform usampler1D newVerts;
layout (binding = 13) uniform isampler1D triTable;

struct tri {
	uint data[3];
};

layout (binding = 3, std430) buffer tr {
	tri triangles[];
};

void main(){
	uint id = gl_GlobalInvocationID.x;
	if(id >= total) return;
	uint count = 0;
	uint add;
	ivec3 pos = ivec3(0);
	ivec3 offset;
	uvec4 verts;
	int index, index2; 
	int vertCode;
	uint vertNum;

	//descend histopyramid layer by layer. carry over current position (*2),
	//and sum of previous cubes when moving to next layer
	for(int i = 0; i<layers; i++){
		pos *= 2;
		add = texelFetch(pyramid[i], pos, 0).r;
		if(count+add > id){
			continue;
		}
		count += add;
		add = texelFetch(pyramid[i], pos+ivec3(1,0,0), 0).r;
		if(count+add > id){
			pos += ivec3 (1,0,0);
			continue;
		}
		count += add;
		add = texelFetch(pyramid[i], pos+ivec3(0,1,0), 0).r;
		if(count+add > id){
			pos += ivec3 (0,1,0);
			continue;
		}
		count += add;
		add = texelFetch(pyramid[i], pos+ivec3(1,1,0), 0).r;
		if(count+add > id){
			pos += ivec3 (1,1,0);
			continue;
		}
		count += add;
		add = texelFetch(pyramid[i], pos+ivec3(0,0,1), 0).r;
		if(count+add > id){
			pos += ivec3 (0,0,1);
			continue;
		}
		count += add;
		add = texelFetch(pyramid[i], pos+ivec3(1,0,1), 0).r;
		if(count+add > id){
			pos += ivec3 (1,0,1);
			continue;
		}
		count += add;
		add = texelFetch(pyramid[i], pos+ivec3(0,1,1), 0).r;
		if(count+add > id){
			pos += ivec3 (0,1,1);
			continue;
		}
		count += add;
		add = texelFetch(pyramid[i], pos+ivec3(1,1,1), 0).r;
		if(count+add > id){
			pos += ivec3 (1,1,1);
			continue;
		}
	}

	//find final cube containing id number in base layer 
	//(lookup count from cube case)
	for(int i = 0; i<1; i++){		
		pos *= 2;

		index = int(texelFetch(cubeCases, pos, 0).r);
		add = texelFetch(numTris, index, 0).r;
		if(count+add > id){
			break;
		}
		count += add;
		index = int(texelFetch(cubeCases, pos+ivec3(1,0,0), 0).r);
		add = texelFetch(numTris, index, 0).r;
		if(count+add > id){
			pos += ivec3 (1,0,0);
			break;
		}
		count += add;
		index = int(texelFetch(cubeCases, pos+ivec3(0,1,0), 0).r);
		add = texelFetch(numTris, index, 0).r;
		if(count+add > id){
			pos += ivec3 (0,1,0);
			break;
		}
		count += add;
		index = int(texelFetch(cubeCases, pos+ivec3(1,1,0), 0).r);
		add = texelFetch(numTris, index, 0).r;
		if(count+add > id){
			pos += ivec3 (1,1,0);
			break;
		}
		count += add;
		index = int(texelFetch(cubeCases, pos+ivec3(0,0,1), 0).r);
		add = texelFetch(numTris, index, 0).r;
		if(count+add > id){
			pos += ivec3 (0,0,1);
			break;
		}
		count += add;
		index = int(texelFetch(cubeCases, pos+ivec3(1,0,1), 0).r);
		add = texelFetch(numTris, index, 0).r;
		if(count+add > id){
			pos += ivec3 (1,0,1);
			break;
		}
		count += add;
		index = int(texelFetch(cubeCases, pos+ivec3(0,1,1), 0).r);
		add = texelFetch(numTris, index, 0).r;
		if(count+add > id){
			pos += ivec3 (0,1,1);
			break;
		}
		count += add;
		index = int(texelFetch(cubeCases, pos+ivec3(1,1,1), 0).r);
		add = texelFetch(numTris, index, 0).r;
		if(count+add > id){
			pos += ivec3 (1,1,1);
			break;
		}
	}

	//decode triangle vertex information from triTable
	//lookup base vertex numbers for participating cubes in 3D texture
	//calculate offsets from base, and write to element buffer 
	count = uint(id) - count;
	index = index * 16;
	for(int i=0; i<3; i++){
		vertCode = texelFetch(triTable, index + 3 * int(count) + i, 0).r;
		offset = ivec3 ( ((vertCode & 16) > 0) ? -1 : 0, ((vertCode & 8) > 0) ? -1 : 0,  ((vertCode & 4) > 0) ? -1 : 0);
		index2 = int(texelFetch(cubeCases, pos + offset, 0).r) >> 2;
		vertNum = texelFetch(vertPositions, pos + offset, 0).r;
		verts = texelFetch(newVerts, index2, 0);
		vertNum += verts[vertCode & 3];
		triangles[id].data[i] = vertNum;
	}

}