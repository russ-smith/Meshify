uniform int iterations;
uniform float A, B, C, D, E, F, G, H;

float DE(vec3 p){
	vec4 q = vec4(p, 0);
	vec4 dq = vec4(1,0,0,0);
	vec4 coeff = vec4 (A, B, C, D);
	float threshold = pow(10, E+1);
	for (int i = 0; i < iterations; i++) {
		dq = 2.0* vec4(q.x*dq.x-dot(q.yzw, dq.yzw), q.x*dq.yzw+dq.x*q.yzw+cross(q.yzw, dq.yzw));
		q = vec4(q.x*q.x-dot(q.yzw, q.yzw), vec3(2.0*q.x*q.yzw)) + coeff;
		float q2 = dot(q,q);
		if (q2 > threshold) break;
	}
	float r = length(q);
	return  0.5 * r * log(r) / length(dq);
}