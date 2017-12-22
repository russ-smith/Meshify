#version 440 core

//Descends the pyramid one layer at a time and sums cube values, 
//until location of current edge found in base layer

layout (local_size_x = 64) in;

uniform int layers;
uniform int total;
uniform int res;
uniform float stride;

layout (binding = 1) uniform usampler3D cubeCases;
layout (binding = 2) uniform usampler3D pyramid[8];
layout (binding = 12) uniform usampler1D newEdges;
layout (binding = 0) uniform writeonly uimage3D edgeNumbers;

struct vector3{									
	float x,y,z;
};

//write out edge positions into what will become the normal buffer later
layout (binding = 2, std430) writeonly buffer p{
	vector3 positions[];
};

struct edgemarker{
	int p0, p1, p2, p3, p4, flip;
};

//store if an edge is 'flipped' i.e. needs its quad's winding order reversed in element buffer
layout (binding = 3, std430) writeonly buffer e{
	edgemarker[] edgeinfo;
};

float DE(vec3 p);

void main(){
	uint id = gl_GlobalInvocationID.x;
	if(id >= total) return;
	uint count = 0;
	uint add;
	ivec3 pos = ivec3(0);
	int index; 
	uvec4 edges;

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
		edges = texelFetch(newEdges, index >> 2, 0);
		add = edges.a;
		if(count+add > id){
			break;
		}
		count += add;
		index = int(texelFetch(cubeCases, pos+ivec3(1,0,0), 0).r);
		edges = texelFetch(newEdges, index >> 2, 0);
		add = edges.a;
		if(count+add > id){
			pos += ivec3 (1,0,0);
			break;
		}
		count += add;
		index = int(texelFetch(cubeCases, pos+ivec3(0,1,0), 0).r);
		edges = texelFetch(newEdges, index >> 2, 0);
		add = edges.a;
		if(count+add > id){
			pos += ivec3 (0,1,0);
			break;
		}
		count += add;
		index = int(texelFetch(cubeCases, pos+ivec3(1,1,0), 0).r);
		edges = texelFetch(newEdges, index >> 2, 0);
		add = edges.a;
		if(count+add > id){
			pos += ivec3 (1,1,0);
			break;
		}
		count += add;
		index = int(texelFetch(cubeCases, pos+ivec3(0,0,1), 0).r);
		edges = texelFetch(newEdges, index >> 2, 0);
		add = edges.a;
		if(count+add > id){
			pos += ivec3 (0,0,1);
			break;
		}
		count += add;
		index = int(texelFetch(cubeCases, pos+ivec3(1,0,1), 0).r);
		edges = texelFetch(newEdges, index >> 2, 0);
		add = edges.a;
		if(count+add > id){
			pos += ivec3 (1,0,1);
			break;
		}
		count += add;
		index = int(texelFetch(cubeCases, pos+ivec3(0,1,1), 0).r);
		edges = texelFetch(newEdges, index >> 2, 0);
		add = edges.a;
		if(count+add > id){
			pos += ivec3 (0,1,1);
			break;
		}
		count += add;
		index = int(texelFetch(cubeCases, pos+ivec3(1,1,1), 0).r);
		edges = texelFetch(newEdges, index >> 2, 0);
		add = edges.a;
		if(count+add > id){
			pos += ivec3 (1,1,1);
			break;
		}
	}

	if(count==id){
		imageStore(edgeNumbers, pos, uvec4(count, 0,0,0));
	}
	count = uint(id)-count;
	uint edge;
	for(int i=0; i<3; i++){
		if(edges[i] == count){
			edge = i;
		}
	}

	edgeinfo[id].flip = (index & 64 > 0 ? 1 : 0);

	//lerp position between edge endpoints depending on distance from surface
	vec3 posA = vec3(pos - (res/2 - 1)) * stride;
	vec3 posB;
	if (edge==0) posB = posA - vec3(stride, 0, 0);
	if (edge==1) posB = posA - vec3(0, stride, 0);
	if (edge==2) posB = posA - vec3(0, 0, stride);
	float dA = DE(posA), dB = DE(posB);
	posB = mix(posA, posB, abs(dA) / (abs(dA) + abs(dB) ) );
	positions[id].x = posB.x; 
	positions[id].y = posB.y;
	positions[id].z = posB.z;	
}