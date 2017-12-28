#version 440 core

//Descends the pyramid one layer at a time and sums cube values, 
//until location of current vertex found in base layer
//Writes ID of first vertex in each cube to 3D texture for element array shader
//Calculates final position and normal

layout (local_size_x = 64) in;

uniform int layers;
uniform int total;
uniform int res;
uniform float stride;
uniform vec3 Centre;

layout (binding = 1) uniform usampler3D cubeCases;
layout (binding = 2) uniform usampler3D pyramid[8];
layout (binding = 12) uniform usampler1D newVerts;
layout (binding = 0) uniform writeonly uimage3D vertNumbers;

struct vector3{									
	float x,y,z;
};

layout (binding = 1, std430) writeonly buffer p{
	vector3 positions[];
};

layout (binding = 2, std430) writeonly buffer n{
	vector3 normals[];
};

float DE(vec3 p);

void main(){
	uint id = gl_GlobalInvocationID.x;
	if(id >= total) return;
	uint count = 0;
	uint add;
	ivec3 pos = ivec3(0);
	int index; 
	uvec4 verts;

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

	//write first vert ID per-cube to big texture for later
	if(count==id){
		imageStore(vertNumbers, pos, uvec4(count, 0,0,0));
	}
	
	count = uint(id)-count;
	uint edge;
	for(int i=0; i<3; i++){
		if(verts[i] == count){
			edge = i;
		}
	}

	//lerp position between edge endpoints depending on distance from surface
	vec3 posA = Centre + vec3(pos - (res/2 - 1)) * stride;
	vec3 posB;
	if (edge==0) posB = posA - vec3(stride, 0, 0);
	if (edge==1) posB = posA - vec3(0, stride, 0);
	if (edge==2) posB = posA - vec3(0, 0, stride);
	float dA = DE(posA), dB = DE(posB);
	posB = mix(posA, posB, abs(dA) / (abs(dA) + abs(dB) ) );
	positions[id].x = posB.x - Centre.x; 
	positions[id].y = posB.y - Centre.y;
	positions[id].z = posB.z - Centre.z;
	
	//6-tap normal using central differences
	vec2 e = vec2 (.005, 0);
	vec3 normal = normalize (vec3 ( DE (posB + e.xyy) - DE (posB - e.xyy),
									DE (posB + e.yxy) - DE (posB - e.yxy),
									DE (posB + e.yyx) - DE (posB - e.yyx) ) );
	normals[id].x = normal.x;
	normals[id].y = normal.y;
	normals[id].z = normal.z;
}