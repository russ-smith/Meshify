#version 430

uniform vec3 lightPos;
uniform vec3 lightColor;

in vec3 transVert;
in vec3 transNormal;
out vec4 outputColor;

vec3 diffuseColor = vec3 (0.8,0.8,0.9);
float shine = 60;

void main(){
	vec3 normal = normalize(transNormal);
	vec3 intensity;
	vec3 cameraDir = normalize(- transVert);
	vec3 lightDir = (lightPos - transVert);
	float dist = length(lightDir);
	lightDir = lightDir * 1/dist;
	float attenuate = (1 / (1 + 0.005 * dist));
	intensity += lightColor * attenuate * diffuseColor * max(dot(normal,lightDir) , 0) ;  // diffuse component
	intensity += lightColor * attenuate * pow(max(dot(reflect( - lightDir, normal), cameraDir) , 0), shine);
	
	intensity = max(intensity, 0.05);
	outputColor = vec4(intensity, 1) ;
}