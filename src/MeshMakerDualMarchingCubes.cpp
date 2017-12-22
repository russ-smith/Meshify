#include "MeshMakerDualMarchingCubes.h"

MeshMakerDualMarchingCubes::MeshMakerDualMarchingCubes(ControlPanel & c) : MeshMakerBase(c){
	control.registerFunctionCallback([this]() { setupFunction(); });
	buildBaseLayerCS.setupShaderFromFile(GL_COMPUTE_SHADER, "ComputeShaders/DMC/BuildBaseLayerDMC.glsl");
	buildBaseLayerCS.linkProgram();
}

void MeshMakerDualMarchingCubes::makeMesh() {
	getPointsAndCases();
	buildBaseLayer();
	buildPyramidsFromBaseLayer();
	GLuint* mappedBuff = static_cast<GLuint*>(glMapNamedBuffer(BufferBundle::instance().totalsBuff, GL_READ_ONLY));
	numVerts = mappedBuff[1];
	numPolys = mappedBuff[0] * 2;
	glUnmapNamedBuffer(BufferBundle::instance().totalsBuff);

	control.setLabels(numVerts, numPolys);
}

void MeshMakerDualMarchingCubes::buildBaseLayer() {
	int layer = control.layers() - 1;
	glBindImageTexture(2, BufferBundle::instance().pyramidA[layer], 0, GL_FALSE, 0, GL_WRITE_ONLY, BufferBundle::FORMAT_BY_LAYER_A[layer]);
	glBindImageTexture(3, BufferBundle::instance().pyramidB[layer], 0, GL_FALSE, 0, GL_WRITE_ONLY, BufferBundle::FORMAT_BY_LAYER_B[layer]);
	glUseProgram(buildBaseLayerCS.getProgram());
	int numGroups = control.res() >> 3;
	glDispatchCompute(numGroups, numGroups, numGroups);
	glMemoryBarrier(GL_TEXTURE_FETCH_BARRIER_BIT);
}

void MeshMakerDualMarchingCubes::setupFunction() {
	setupConcatenatedShader(getPointsCS, "ComputeShaders/Common/GetPoints.glsl", control.functionFile);
	getPointsCS.linkProgram();
}
