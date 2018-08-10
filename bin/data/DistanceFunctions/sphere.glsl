uniform float A, B, C;

float DE(vec3 p){
	vec3 r = vec3(A, B, C) + vec3(1.05); 
	return (length(p/r)-1.) * min(r.x,min(r.y,r.z));
}