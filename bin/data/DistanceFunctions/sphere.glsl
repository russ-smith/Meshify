uniform float A;

float DE(vec3 p){
	return length(p) - (A+1);
}