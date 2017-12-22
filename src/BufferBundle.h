#pragma once
#include "GL\glew.h"

//Singleton class managing all GL buffer and texture objects used in mesh creation
class BufferBundle {
public:
	static const GLenum FORMAT_BY_LAYER_A[8];
	static const GLenum FORMAT_BY_LAYER_B[8];
	static BufferBundle & instance() {
		static BufferBundle b;
		return b;
	}
	BufferBundle(BufferBundle const&) = delete;
	BufferBundle& operator = (BufferBundle const&) = delete;
	BufferBundle(BufferBundle &&) = delete;
	BufferBundle& operator = (BufferBundle &&) = delete;

	GLuint
		baseLayerTex,
		debugTex,
		cubeCaseTex,
		totalsBuff,
		vertexBuff,
		normalBuff,
		elementBuff,
		lightUniBuff,
		materialUniBuff,
		VAO,
		pyramidA[8],
		pyramidB[8],
		newEdgeTableTex,
		MCNumTrisTableTex,
		MCTriTableTex,
		DMCNumVertsTableTex,
		DMCVertToEdgeTableTex;
private:
	BufferBundle();
};