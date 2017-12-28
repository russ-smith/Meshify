#version 440 core

//Samples the distance function at all lattice points and 
//writes classification to 3D texture

layout (local_size_x=4, local_size_y =4, local_Size_z=4) in;
layout (binding=0) uniform writeonly uimage3D points;

uniform int res;
uniform float stride;

float DE(vec3 p);

void main(){
	ivec3 id = ivec3(gl_GlobalInvocationID);
	vec3 pos = (vec3(id) - .5 * res) * stride;
	uint inside;
	if (min(id.x , min(id.y, id.z)) == 0 || max(id.x , max(id.y, id.z)) >= res)
		inside = 0;
	else	
		inside = (DE(pos) <= 0 ? 1 : 0);
	
	imageStore(points, id, uvec4(inside, 0,0,0));
} 