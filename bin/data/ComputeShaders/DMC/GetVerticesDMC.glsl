#version 440 core


layout (local_size_x = 64) in;

uniform int total;
uniform int layers;

layout (binding = 0) uniform usampler3D edgeNumbers;
layout (binding = 1) uniform usampler3D cubeCases;
layout (binding = 2) uniform usampler3D pyramid[8];

layout (binding = 12) uniform usampler1D newEdges;
layout (binding = 14) uniform usampler1D numVerts;
layout (binding = 15) uniform isampler1D vertToEdge;
layout (binding = 16) uniform usampler1D vertBegin;

struct vector3{									
	float x,y,z;
};

layout (binding = 1, std430) writeonly buffer vp{
	vector3 vertPositions[];
};

layout (binding = 2, std430) readonly buffer ep{
	vector3 edgePositions[];
};

layout (binding = 3, std430) writeonly buffer qu {
	uint quads[];
};

void main(){
	uint id = gl_GlobalInvocationID.x;
	if(id >= total) return;
	uint count = 0;
	uint add;
	ivec3 pos = ivec3(0);
	ivec3 offset;
	int begin;
	int vertCode;
	vec4 vertPos = vec4(0);
	int index;

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
		add = texelFetch(numVerts, index, 0).r;
		if(count+add > id){
			break;
		}
		count += add;
		index = int(texelFetch(cubeCases, pos+ivec3(1,0,0), 0).r);
		add = texelFetch(numVerts, index, 0).r;
		if(count+add > id){
			pos += ivec3 (1,0,0);
			break;
		}
		count += add;
		index = int(texelFetch(cubeCases, pos+ivec3(0,1,0), 0).r);
		add = texelFetch(numVerts, index, 0).r;
		if(count+add > id){
			pos += ivec3 (0,1,0);
			break;
		}
		count += add;
		index = int(texelFetch(cubeCases, pos+ivec3(1,1,0), 0).r);
		add = texelFetch(numVerts, index, 0).r;
		if(count+add > id){
			pos += ivec3 (1,1,0);
			break;
		}
		count += add;
		index = int(texelFetch(cubeCases, pos+ivec3(0,0,1), 0).r);
		add = texelFetch(numVerts, index, 0).r;
		if(count+add > id){
			pos += ivec3 (0,0,1);
			break;
		}
		count += add;
		index = int(texelFetch(cubeCases, pos+ivec3(1,0,1), 0).r);
		add = texelFetch(numVerts, index, 0).r;
		if(count+add > id){
			pos += ivec3 (1,0,1);
			break;
		}
		count += add;
		index = int(texelFetch(cubeCases, pos+ivec3(0,1,1), 0).r);
		add = texelFetch(numVerts, index, 0).r;
		if(count+add > id){
			pos += ivec3 (0,1,1);
			break;
		}
		count += add;
		index = int(texelFetch(cubeCases, pos+ivec3(1,1,1), 0).r);
		add = texelFetch(numVerts, index, 0).r;
		if(count+add > id){
			pos += ivec3 (1,1,1);
			break;
		}
	}

	count = uint(id) - count;
	begin = int(texelFetch(vertBegin, index, 0)[count]);
	index *= 16;

	vertPositions[id].x = float(index);

	//accumulate positions from each edge associated with the vertex to find average position
	//at the same time write the vertex's ID to the element buffer for each edge found
	for(int i = 0; i<6; i++){				
		vertCode = texelFetch(vertToEdge, index+begin+i, 0).r;
		if (vertCode < 0) break;
		offset = ivec3 ( ((vertCode & 16) > 0) ? -1 : 0, ((vertCode & 8) > 0) ? -1 : 0,  ((vertCode & 4) > 0) ? -1 : 0);
		int index2 = int(texelFetch(cubeCases, pos + offset, 0).r) >> 2;
		uint edgeNum = texelFetch(edgeNumbers, pos + offset, 0).r;
		uvec4 edges = texelFetch(newEdges, index2, 0);
		edgeNum += edges[vertCode &3];
		vertPos += vec4(edgePositions[edgeNum].x, edgePositions[edgeNum].y, edgePositions[edgeNum].z, 1);
		quads[6*int(edgeNum) + ( (vertCode >> 5) & 3)] = uint(id);
	}
	vertPos.xyz *= 1/vertPos.w;
	vertPositions[id].x = vertPos.x;
	vertPositions[id].y = vertPos.y;
	vertPositions[id].z = vertPos.z;
}