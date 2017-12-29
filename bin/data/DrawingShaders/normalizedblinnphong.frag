#version 440 core

uniform vec3 Light0Dir;
uniform vec3 Light0Color;
uniform vec3 Light1Dir;
uniform vec3 Light1Color;
uniform float Smoothness;
uniform vec3 DiffuseColor;
in vec3 transVert;
in vec3 transNormal;
out vec4 outputColor;
float specColor = 0.04;					//approx specular color for dielectric


void main(){
	//not physically accurate but whatever...
	float shine = 2 / pow(1-Smoothness, 6) - 2;
	vec3 normal = normalize(transNormal);
	vec3 color = vec3(0);
	vec3 cameraDir = normalize(- transVert);
	float normalizeFactor = (shine+8)*0.125;
	float nl, nh;
	vec3 h;

	nl = max(dot(normal,Light0Dir) , 0);
	if (nl>0){
		h = normalize(normal + Light0Dir);
		nh = dot(normal, h);
		color += nl * Light0Color * (DiffuseColor + normalizeFactor * specColor * pow(nh, shine));  
	}
	nl = max(dot(normal,Light1Dir) , 0); 
	if (nl>0){
		h = normalize(normal + Light1Dir);
		nh = dot(normal, h);
		color += nl * Light1Color * (DiffuseColor + normalizeFactor * specColor * pow(nh, shine));  
	}
	color += 0.03 + 0.01*transNormal.y * DiffuseColor;	//simple ambient light
	color = pow (clamp (color, vec3(0), vec3(1)), vec3(1/2.2)); //gamma
	outputColor = vec4(color, 1) ;
}