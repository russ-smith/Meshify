#include "MeshMakerDualMarchingCubes.h"

MeshMakerDualMarchingCubes::MeshMakerDualMarchingCubes(ControlPanel & c) : MeshMakerBase(c){
	control.registerFunctionCallback([this]() { setupFunction(); });
	buildBaseLayerCS.setupShaderFromFile(GL_COMPUTE_SHADER, "ComputeShaders/DMC/BuildBaseLayerDMC.glsl");
	buildBaseLayerCS.linkProgram();
	setupConcatenatedShader(getEdgesCS, "ComputeShaders/DMC/GetEdgesDMC.glsl", control.functionFile);
	getEdgesCS.linkProgram();
	setupConcatenatedShader(getPositionsAndNormalsCS, "ComputeShaders/DMC/GetPositionsAndNormalsDMC.glsl", control.functionFile);
	getPositionsAndNormalsCS.linkProgram();
	getVerticesCS.setupShaderFromFile(GL_COMPUTE_SHADER, "ComputeShaders/DMC/GetVerticesDMC.glsl");
	getVerticesCS.linkProgram();
	getTrianglesCS.setupShaderFromFile(GL_COMPUTE_SHADER, "ComputeShaders/DMC/GetTrianglesDMC.glsl");
	getTrianglesCS.linkProgram();
}

void MeshMakerDualMarchingCubes::makeMesh() {
	getPointsAndCases();
	buildBaseLayer();
	buildPyramidsFromBaseLayer();
	GLuint* mappedBuff = static_cast<GLuint*>(glMapNamedBuffer(BufferBundle::instance().totalsBuff, GL_READ_ONLY));
	numVerts = mappedBuff[1];
	numPolys = mappedBuff[0];
	glUnmapNamedBuffer(BufferBundle::instance().totalsBuff);
	glNamedBufferData(BufferBundle::instance().vertexBuff, 3 * numVerts * sizeof(GLfloat), nullptr, GL_DYNAMIC_DRAW);
	glNamedBufferData(BufferBundle::instance().normalBuff, 3 * max(numVerts, numPolys) * sizeof(GLfloat), nullptr, GL_DYNAMIC_DRAW);
	glNamedBufferData(BufferBundle::instance().elementBuff, 6 * numPolys * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);
	getEdges();
	getVertices();
	getPositionsAndNormals();
	getTriangles();
	numPolys *= 2;
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

void MeshMakerDualMarchingCubes::getEdges() {
	glBindTextures(2, 8, BufferBundle::instance().pyramidA);
	glUseProgram(getEdgesCS.getProgram());
	getEdgesCS.setUniform1i("total", numPolys);
	getEdgesCS.setUniform1i("layers", control.layers());
	getEdgesCS.setUniform1i("res", control.res());
	getEdgesCS.setUniform1f("stride", control.stride());
	getEdgesCS.setUniforms(control.functionParams);
	glDispatchCompute((numPolys + 63) / 64, 1, 1);
	glMemoryBarrier(GL_TEXTURE_FETCH_BARRIER_BIT);
}

void MeshMakerDualMarchingCubes::getVertices() {
	glBindTextures(2, 8, BufferBundle::instance().pyramidB);
	glUseProgram(getVerticesCS.getProgram());
	getVerticesCS.setUniform1i("total", numVerts);
	getVerticesCS.setUniform1i("layers", control.layers());
	glDispatchCompute((numVerts + 63) / 64, 1, 1);
	glMemoryBarrier(GL_BUFFER_UPDATE_BARRIER_BIT);
}

void MeshMakerDualMarchingCubes::getPositionsAndNormals() {
	glUseProgram(getPositionsAndNormalsCS.getProgram());
	getPositionsAndNormalsCS.setUniform1i("total", numVerts);
	glDispatchCompute((numVerts + 63) / 64, 1, 1);
	glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT | GL_BUFFER_UPDATE_BARRIER_BIT);
}

void MeshMakerDualMarchingCubes::getTriangles() {
	glUseProgram(getTrianglesCS.getProgram());
	getTrianglesCS.setUniform1i("total", numPolys);
	glDispatchCompute((numPolys + 63) / 64, 1, 1);
	glMemoryBarrier(GL_ELEMENT_ARRAY_BARRIER_BIT);
}

void MeshMakerDualMarchingCubes::setupFunction() {
	setupConcatenatedShader(getPointsCS, "ComputeShaders/Common/GetPoints.glsl", control.functionFile);
	getPointsCS.linkProgram();
	setupConcatenatedShader(getEdgesCS, "ComputeShaders/DMC/GetEdgesDMC.glsl", control.functionFile);
	getEdgesCS.linkProgram();
	setupConcatenatedShader(getPositionsAndNormalsCS, "ComputeShaders/DMC/GetPostionsAndNormalsDMC.glsl", control.functionFile);
	getPositionsAndNormalsCS.linkProgram();
}
