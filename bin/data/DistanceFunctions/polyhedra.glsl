uniform float A, B, C, D;

#define PI 3.1415

//credit goes to Knighty's work at Fragmentarium and Shadertoy for this one
//using Wythoff construction to create regular polyhedra using 3 folding planes
//first two are xz and yz planes, third one depends on symmetry group
float DE(vec3 p){
	float type = (A < -.33) ? 3 : (A > .33) ? 5 : 4;
	float foldY = cos(PI / type);
	float foldZ = sqrt(.75 - foldY*foldY);
	vec3 mirrorC = vec3(-0.5, -foldY, foldZ);
	vec3 crossAB = vec3(0,0,1);
	vec3 crossBC = vec3(foldZ,0,.5);
	vec3 crossCA = vec3(0,foldZ,foldY);
	vec3 corner = normalize((B*.5+.5) * crossAB + (C*.5+.5) * crossBC + (D*.5+.5) * crossCA);
	for(int i=0;i<5;i++){
		p.xy = abs(p.xy);
		p-=2*min(0,dot(p,mirrorC))*mirrorC;
	} 
	p -= corner;
	float d = dot(p, crossAB);
	d = max(d, dot(p, crossBC));
	d = max(d, dot(p, crossCA));
	return d;
};