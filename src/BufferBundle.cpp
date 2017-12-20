#include "BufferBundle.h"
#include "LookupTables.h"

const GLenum BufferBundle::FORMAT_BY_LAYER_A[]{ GL_R32UI, GL_R32UI, GL_R32UI, GL_R32UI, GL_R16UI, GL_R16UI, GL_R8UI, GL_R8UI };
const GLenum BufferBundle::FORMAT_BY_LAYER_B[]{ GL_R32UI, GL_R32UI, GL_R32UI, GL_R32UI, GL_R16UI, GL_R16UI, GL_R16UI, GL_R8UI };

BufferBundle::BufferBundle() {
	//histogram pyramid buffers and textures
	glCreateBuffers(1, &totalsBuff);
	glNamedBufferStorage(totalsBuff, 2 * sizeof(GLuint), nullptr, GL_MAP_READ_BIT);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, totalsBuff);
	glCreateTextures(GL_TEXTURE_3D, 1, &baseLayerTex);
	glTextureStorage3D(baseLayerTex, 1, GL_R32UI, 512, 512, 512);
	glBindImageTexture(0, baseLayerTex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32UI);
	glBindTextureUnit(0, baseLayerTex);
	glCreateTextures(GL_TEXTURE_3D, 1, &cubeCaseTex);
	glTextureStorage3D(cubeCaseTex, 1, GL_R8UI, 512, 512, 512);
	glBindTextureUnit(1, cubeCaseTex);
	glBindImageTexture(1, cubeCaseTex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R8UI);
	glCreateTextures(GL_TEXTURE_3D, 8, &pyramidA[0]);
	glCreateTextures(GL_TEXTURE_3D, 8, &pyramidB[0]);
	glTextureStorage3D(pyramidA[0], 1, FORMAT_BY_LAYER_A[0], 2, 2, 2);
	glTextureStorage3D(pyramidA[1], 1, FORMAT_BY_LAYER_A[1], 4, 4, 4);
	glTextureStorage3D(pyramidA[2], 1, FORMAT_BY_LAYER_A[2], 8, 8, 8);
	glTextureStorage3D(pyramidA[3], 1, FORMAT_BY_LAYER_A[3], 16, 16, 16);
	glTextureStorage3D(pyramidA[4], 1, FORMAT_BY_LAYER_A[4], 32, 32, 32);
	glTextureStorage3D(pyramidA[5], 1, FORMAT_BY_LAYER_A[5], 64, 64, 64);
	glTextureStorage3D(pyramidA[6], 1, FORMAT_BY_LAYER_A[6], 128, 128, 128);
	glTextureStorage3D(pyramidA[7], 1, FORMAT_BY_LAYER_A[7], 256, 256, 256);
	glTextureStorage3D(pyramidB[0], 1, FORMAT_BY_LAYER_B[0], 2, 2, 2);
	glTextureStorage3D(pyramidB[1], 1, FORMAT_BY_LAYER_B[1], 4, 4, 4);
	glTextureStorage3D(pyramidB[2], 1, FORMAT_BY_LAYER_B[2], 8, 8, 8);
	glTextureStorage3D(pyramidB[3], 1, FORMAT_BY_LAYER_B[3], 16, 16, 16);
	glTextureStorage3D(pyramidB[4], 1, FORMAT_BY_LAYER_B[4], 32, 32, 32);
	glTextureStorage3D(pyramidB[5], 1, FORMAT_BY_LAYER_B[5], 64, 64, 64);
	glTextureStorage3D(pyramidB[6], 1, FORMAT_BY_LAYER_B[6], 128, 128, 128);
	glTextureStorage3D(pyramidB[7], 1, FORMAT_BY_LAYER_B[7], 256, 256, 256);

	//lookup table textures
	glCreateTextures(GL_TEXTURE_1D, 1, &newEdgeTableTex);
	glCreateTextures(GL_TEXTURE_1D, 1, &numTrisMCTableTex);
	glCreateTextures(GL_TEXTURE_1D, 1, &triTableMCTex);
	glTextureStorage1D(newEdgeTableTex, 1, GL_RGBA8UI, 64);
	glTextureSubImage1D(newEdgeTableTex, 0, 0, 64, GL_RGBA_INTEGER, GL_UNSIGNED_BYTE, &LookupTables::newEdgeTable);
	glTextureStorage1D(numTrisMCTableTex, 1, GL_R8UI, 256);
	glTextureSubImage1D(numTrisMCTableTex, 0, 0, 256, GL_RED_INTEGER, GL_UNSIGNED_BYTE, &LookupTables::MCNumTris);
	glTextureStorage1D(triTableMCTex, 1, GL_R8I, 4096);
	glTextureSubImage1D(triTableMCTex, 0, 0, 4096, GL_RED_INTEGER, GL_BYTE, &LookupTables::MCTriTable);
	glBindTextureUnit(11, numTrisMCTableTex);
	glBindTextureUnit(12, newEdgeTableTex);
	glBindTextureUnit(13, triTableMCTex);

	//vertex data buffers
	glCreateBuffers(1, &vertexBuff);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vertexBuff);			//bind this buffer twice
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, vertexBuff);			//gets read and written as different format in some shaders
	glCreateBuffers(1, &normalBuff);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, normalBuff);
	glCreateBuffers(1, &elementBuff);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, elementBuff);

	//vertex array for rendering
	glCreateVertexArrays(1, &VAO);
	glVertexArrayVertexBuffer(VAO, 0, vertexBuff, 0, 3 * sizeof(GLfloat));
	glVertexArrayVertexBuffer(VAO, 1, normalBuff, 0, 3 * sizeof(GLfloat));
	glVertexArrayAttribBinding(VAO, 0, 0);
	glVertexArrayAttribBinding(VAO, 1, 1);
	glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribFormat(VAO, 1, 3, GL_FLOAT, GL_FALSE, 0);
	glEnableVertexArrayAttrib(VAO, 0);
	glEnableVertexArrayAttrib(VAO, 1);
	glVertexArrayElementBuffer(VAO, elementBuff);

	//uniform buffers for light and material data
	glCreateBuffers(1, &lightUniBuff);
	glNamedBufferStorage(lightUniBuff, 16 * sizeof(GLfloat), nullptr, GL_DYNAMIC_STORAGE_BIT);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, lightUniBuff);
}
