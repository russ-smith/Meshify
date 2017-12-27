#version 440 core

layout(local_size_x = 64) in;

uniform int total;

struct vector3{
	float x, y, z;
};

layout (binding = 1, std430) readonly buffer p {
	vector3 positions[];
};

struct quad {
	uint a, b, c, d, e, f;
};

layout (binding = 3, std430) buffer e {
	quad quads[];
};

//create 2 triangles from each edge quad 
//split is done across shortest diagonal to avoid skinny triangles
//winding order determined by the quad's 'flipped' marker (stored in f)
void main(){
	int id = int(gl_GlobalInvocationID.x);
	if (id >= total) return;

	bool flipped = (quads[id].f > 0);
	uint A = quads[id].a;
	uint B = quads[id].b;
	uint C = quads[id].c;
	uint D = quads[id].d;

	vec3 posA = vec3(positions[A].x, positions[A].y, positions[A].z);
	vec3 posB = vec3(positions[B].x, positions[B].y, positions[B].z);
	posA -= vec3(positions[C].x, positions[C].y, positions[C].z);
	posB -= vec3(positions[D].x, positions[D].y, positions[D].z);

	bool inorder = (dot(posA,posA) <= dot (posB, posB));

	quads[id].a = quads[id].f = inorder ? A : B;
	quads[id].c = quads[id].d = inorder ? C : D;
	quads[id].b = inorder ? (flipped ? D : B) : (flipped ? A : C);
	quads[id].e = inorder ? (flipped ? B : D) : (flipped ? C : A);
}