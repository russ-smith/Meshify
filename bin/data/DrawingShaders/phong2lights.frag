#version 440 core

uniform vec3 light0Dir;
uniform vec3 light0Color;
uniform vec3 light1Dir;
uniform vec3 light1Color;

in vec3 transVert;
in vec3 transNormal;
out vec4 outputColor;

vec3 diffuseColor = vec3 (0.8,0.8,0.9);
float shine = 60;

void main(){
	vec3 normal = normalize(transNormal);
	vec3 intensity = vec3(0);
	vec3 cameraDir = normalize(- transVert);
	
	intensity += light0Color * diffuseColor * max(dot(normal,light0Dir) , 0) ;  
	intensity += light0Color * pow(max(dot(reflect( - light0Dir, normal), cameraDir) , 0), shine); 
	intensity += light1Color * diffuseColor * max(dot(normal,light1Dir) , 0) ;  
	intensity += light1Color * pow(max(dot(reflect( - light1Dir, normal), cameraDir) , 0), shine); 
	intensity += 0.05 + 0.02*transNormal.y;	//simple ambient light
	intensity = pow (clamp (intensity, vec3(0), vec3(1)), vec3(1/2.2)); //gamma
	outputColor = vec4(intensity, 1) ;
}