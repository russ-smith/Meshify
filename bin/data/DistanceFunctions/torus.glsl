uniform float A, B, C;

float DE(vec3 p){
	vec2 r = vec2(A, B) + vec2(1.05);
	float xy =length(p.xy) - r.x;
	return length(vec2(xy,p.z)) - r.y;
}