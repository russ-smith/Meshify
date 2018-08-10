uniform int Iterations;
uniform float A, B, C, D;

float s = 3.*A;
float r = B + 1.;
float absScalem1 = abs(s - 1.0);
float AbsScaleRaisedTo1mIters = pow(abs(s), float(1-Iterations));
vec4 scale = vec4(s,s,s,abs(s)) * 1./r;

float DE(vec3 p){
	vec4 pn = vec4(p, 1.); 
	vec4 p0 = pn;
	for(int i=0;i<Iterations;i++){
		pn.xyz = clamp(pn.xyz, -1., 1.) * 2. - pn.xyz;
		float r2 = dot(pn.xyz, pn.xyz);
		pn *= clamp(max(r/r2, r),0.,1.);
		pn = pn * scale + p0;
		if (r2>1000.) break;
	}
	return (length(pn.xyz) - absScalem1) / (pn.w - AbsScaleRaisedTo1mIters);
}