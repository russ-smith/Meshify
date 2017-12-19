#version 440 core

//Builds the final 3 layers of the histopyramids in one pass

layout(local_size_x = 4, local_size_y = 4, local_size_z = 4) in;

layout(binding=2) uniform usampler3D A8;
layout(binding=3) uniform usampler3D B8;
//layout(binding=4) uniform usampler3D A4;
//layout(binding=5) uniform usampler3D B4;
//layout(binding=6) uniform usampler3D A2;
//layout(binding=7) uniform usampler3D B2;
layout(binding=2) uniform writeonly uimage3D A4w;
layout(binding=3) uniform writeonly uimage3D B4w;
layout(binding=4) uniform writeonly uimage3D A2w;
layout(binding=5) uniform writeonly uimage3D B2w;
layout(binding=5) writeonly buffer totals {
	uint A1w;
	uint B1w;
};

shared uint valA[72];
shared uint valB[72];

void main(){
	ivec3 id = ivec3(gl_GlobalInvocationID);
	uint A = 0, B = 0;
	int valPos = id.x + 4*id.y + 16*id.z;
	A += texelFetch(A8, 2*id, 0).r;
	B += texelFetch(B8, 2*id, 0).r; 
	A += texelFetch(A8, 2*id+ivec3(1,0,0), 0).r;
	B += texelFetch(B8, 2*id+ivec3(1,0,0), 0).r; 
	A += texelFetch(A8, 2*id+ivec3(0,1,0), 0).r;
	B += texelFetch(B8, 2*id+ivec3(0,1,0), 0).r; 
	A += texelFetch(A8, 2*id+ivec3(1,1,0), 0).r;
	B += texelFetch(B8, 2*id+ivec3(1,1,0), 0).r; 
	A += texelFetch(A8, 2*id+ivec3(0,0,1), 0).r;
	B += texelFetch(B8, 2*id+ivec3(0,0,1), 0).r; 
	A += texelFetch(A8, 2*id+ivec3(1,0,1), 0).r;
	B += texelFetch(B8, 2*id+ivec3(1,0,1), 0).r; 
	A += texelFetch(A8, 2*id+ivec3(0,1,1), 0).r;
	B += texelFetch(B8, 2*id+ivec3(0,1,1), 0).r; 
	A += texelFetch(A8, 2*id+ivec3(1,1,1), 0).r;
	B += texelFetch(B8, 2*id+ivec3(1,1,1), 0).r; 

	imageStore(A4w, id, uvec4(A, 0,0,0));
	imageStore(B4w, id, uvec4(B, 0,0,0));
	valA[valPos] = A;
	valB[valPos] = B;
	
	memoryBarrierShared();

	if(max(id.x,max(id.y,id.z)) < 2) {
		valPos *= 2;
		A = 0, B = 0;
		A += valA[valPos];
		A += valA[valPos + 1];
		A += valA[valPos + 4];
		A += valA[valPos + 5];
		A += valA[valPos + 16];
		A += valA[valPos + 17];
		A += valA[valPos + 20];
		A += valA[valPos + 21];
		B += valB[valPos];
		B += valB[valPos + 1];
		B += valB[valPos + 4];
		B += valB[valPos + 5];
		B += valB[valPos + 16];
		B += valB[valPos + 17];
		B += valB[valPos + 20];
		B += valB[valPos + 21];
		
		imageStore(A2w, id, uvec4(A, 0,0,0));
		imageStore(B2w, id, uvec4(B, 0,0,0));
		valPos = 64 + id.x + 2*id.y + 4*id.z;
		valA[valPos] = A;
		valB[valPos] = B;
	}

	memoryBarrierShared();

	if(id.x + id.y + id.z == 0) {
		A = 0, B = 0;
		for(int i=64; i<72; i++){
			A += valA[i];
			B += valB[i];
		}		
		A1w = A;
		B1w = B;
	}
}