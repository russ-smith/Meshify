#version 430

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 normalMatrix;

layout(location=0) in vec4 position;
layout(location=1) in vec3 normal;
out vec3 transVert;
out vec3 transNormal;

void main(){
	gl_Position = modelViewProjectionMatrix * vec4(position.xyz,1);
	transVert = vec3(modelViewMatrix * position);
	transNormal = normalize (mat3(normalMatrix) * normal);
}