#version 440 core

//Calculates normal for each vertex position
//Uses normal and distance value at vertex position to move vertex a little 
//closer to the true surface

layout (local_size_x = 64) in;
uniform int total;

struct vector3{									
	float x,y,z;
};

layout (binding = 1, std430) buffer p{
	vector3 positions[];
};

layout (binding = 2, std430) writeonly buffer n{
	vector3 normals[];
};

float DE(vec3 p);

void main(){
	int id = int(gl_GlobalInvocationID.x);
	if(id >= total) return;
	
	vec3 pos = vec3(positions[id].x, positions[id].y, positions[id].z); 

	//6-tap normal using central differences
	vec2 e = vec2 (.005, 0);
	vec3 normal = normalize (vec3 ( DE (pos + e.xyy) - DE (pos - e.xyy),
									DE (pos + e.yxy) - DE (pos - e.yxy),
									DE (pos + e.yyx) - DE (pos - e.yyx) ) );
	normals[id].x = normal.x;
	normals[id].y = normal.y;
	normals[id].z = normal.z;

	//nudge the point a bit closer to the real surface
	float d = DE(pos);
	pos -= normal * d;
	positions[id].x = pos.x;
	positions[id].y = pos.y;
	positions[id].z = pos.z;
}