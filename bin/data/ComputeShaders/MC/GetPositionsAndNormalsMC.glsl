#version 440 core

layout (local_size_x = 64) in;
uniform int res;
uniform float stride;
uniform int total;

struct marker{
	uint data, pad1, pad2;
};

//work around for std430 padding vec3 to vec4
struct vector3{									
	float x,y,z;
};

layout (binding = 0, std430) readonly buffer m{
	marker vertMarkers[];
};

layout (binding = 1, std430) writeonly buffer p{
	vector3 positions[];
};

layout (binding = 2, std430) writeonly buffer n{
	vector3 normals[];
};

float DE(vec3 p);

void main(){
	
	int id = int(gl_GlobalInvocationID.x);
	if(id >= total) return;
	uint packedPos = vertMarkers[id].data;
	ivec3 intPos = ivec3((packedPos>>20) & 1023, (packedPos>>10) & 1023, packedPos & 1023);
	uint edge = (packedPos >> 30) & 3;
	vec3 posA = vec3(intPos - (res/2 - 1)) * stride;
	vec3 posB;
	if (edge==0) posB = posA - vec3(stride, 0, 0);
	if (edge==1) posB = posA - vec3(0, stride, 0);
	if (edge==2) posB = posA - vec3(0, 0, stride);
	float dA = DE(posA), dB = DE(posB);
	posB = mix(posA, posB, abs(dA) / (abs(dA) + abs(dB) ) );
	positions[id].x = posB.x; 
	positions[id].y = posB.y;
	positions[id].z = posB.z;
	
	//6-tap normal using central differences
	vec2 e = vec2 (.005, 0);
	vec3 normal = normalize (vec3 ( DE (posB + e.xyy) - DE (posB - e.xyy),
									DE (posB + e.yxy) - DE (posB - e.yxy),
									DE (posB + e.yyx) - DE (posB - e.yyx) ) );
	normals[id].x = normal.x;
	normals[id].y = normal.y;
	normals[id].z = normal.z;
}	