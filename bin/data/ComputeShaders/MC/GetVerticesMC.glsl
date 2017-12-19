#version 440 core

//Descends the pyramid one layer at a time and sums cube values, 
//until location of current vertex found in base layer

layout (local_size_x = 64) in;

uniform int layers;
uniform int total;

layout (binding = 1) uniform usampler3D cubeCases;
layout (binding = 2) uniform usampler3D pyramid[8];
layout (binding = 12) uniform usampler1D newVerts;
layout (binding = 0) uniform writeonly uimage3D vertPositions;

struct marker {	
	uint data, pad1, pad2;
};

layout (binding = 0, std430) buffer vm {
	marker vertMarkers[];
};

void main(){
	uint id = gl_GlobalInvocationID.x;
	if(id >= total) return;
	uint count = 0;
	uint add;
	ivec3 pos = ivec3(0);
	int index; 
	uvec4 verts;

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

	for(int i = 0; i<1; i++){		
		pos *= 2;

		index = int(texelFetch(cubeCases, pos, 0).r);
		verts = texelFetch(newVerts, index >> 2, 0);
		add = verts.a;
		if(count+add > id){
			break;
		}
		count += add;
		index = int(texelFetch(cubeCases, pos+ivec3(1,0,0), 0).r);
		verts = texelFetch(newVerts, index >> 2, 0);
		add = verts.a;
		if(count+add > id){
			pos += ivec3 (1,0,0);
			break;
		}
		count += add;
		index = int(texelFetch(cubeCases, pos+ivec3(0,1,0), 0).r);
		verts = texelFetch(newVerts, index >> 2, 0);
		add = verts.a;
		if(count+add > id){
			pos += ivec3 (0,1,0);
			break;
		}
		count += add;
		index = int(texelFetch(cubeCases, pos+ivec3(1,1,0), 0).r);
		verts = texelFetch(newVerts, index >> 2, 0);
		add = verts.a;
		if(count+add > id){
			pos += ivec3 (1,1,0);
			break;
		}
		count += add;
		index = int(texelFetch(cubeCases, pos+ivec3(0,0,1), 0).r);
		verts = texelFetch(newVerts, index >> 2, 0);
		add = verts.a;
		if(count+add > id){
			pos += ivec3 (0,0,1);
			break;
		}
		count += add;
		index = int(texelFetch(cubeCases, pos+ivec3(1,0,1), 0).r);
		verts = texelFetch(newVerts, index >> 2, 0);
		add = verts.a;
		if(count+add > id){
			pos += ivec3 (1,0,1);
			break;
		}
		count += add;
		index = int(texelFetch(cubeCases, pos+ivec3(0,1,1), 0).r);
		verts = texelFetch(newVerts, index >> 2, 0);
		add = verts.a;
		if(count+add > id){
			pos += ivec3 (0,1,1);
			break;
		}
		count += add;
		index = int(texelFetch(cubeCases, pos+ivec3(1,1,1), 0).r);
		verts = texelFetch(newVerts, index >> 2, 0);
		add = verts.a;
		if(count+add > id){
			pos += ivec3 (1,1,1);
			break;
		}
	}

	if(count==id){
		imageStore(vertPositions, pos, uvec4(count, 0,0,0));
	}
	count = uint(id)-count;
	uint edge;
	for(int i=0; i<3; i++){
		if(verts[i] == count){
			edge = i;
		}
	}

	vertMarkers[id].data = (edge << 30) + (pos.x << 20) + (pos.y << 10) + pos.z ;
}