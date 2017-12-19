#include "MeshMakerMarchingCubes.h"

MeshMakerMarchingCubes::MeshMakerMarchingCubes(ControlPanel &c) : MeshMakerBase(c) {
	c.registerRedrawCallback([&]() { makeMesh(); });
	c.registerFunctionCallback([&]() { setupFunction(); });
	buildBaseLayerCS.setupShaderFromFile(GL_COMPUTE_SHADER, "ComputeShaders/MC/BuildBaseLayerMC.glsl");
	buildBaseLayerCS.linkProgram();
	getVerticesCS.setupShaderFromFile(GL_COMPUTE_SHADER, "ComputeShaders/MC/GetVerticesMC.glsl");
	getVerticesCS.linkProgram();
	getTrianglesCS.setupShaderFromFile(GL_COMPUTE_SHADER, "ComputeShaders/MC/GetTrianglesMC.glsl");
	getTrianglesCS.linkProgram();
	setupConcatenatedShader(getPositionsAndNormalsCS, "ComputeShaders/MC/GetPositionsAndNormalsMC.glsl", control.functionFile);
	getPositionsAndNormalsCS.linkProgram();
}

void MeshMakerMarchingCubes::makeMesh() {
	getPointsAndCases();
	buildBaseLayer();
	buildPyramidsFromBaseLayer();
	GLuint* mappedBuff = static_cast<GLuint*>(glMapNamedBuffer(BufferBundle::instance().totalsBuff, GL_READ_ONLY));
	numVerts = mappedBuff[0];
	numPolys = mappedBuff[1];
	glUnmapNamedBuffer(BufferBundle::instance().totalsBuff);
	getVertices();
	getTriangles();
	getPositionsAndNormals();
	ofstream fout("logfile.txt");
	fout << numVerts << "  " << numPolys << "\n\n";	
	control.setLabels(numVerts, numPolys);
}


void MeshMakerMarchingCubes::buildBaseLayer() {
	glBindImageTexture(2, BufferBundle::instance().pyramidA[7], 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R8UI);
	glBindImageTexture(3, BufferBundle::instance().pyramidB[7], 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R8UI);
	glUseProgram(buildBaseLayerCS.getProgram());
	glDispatchCompute(64, 64, 64);
	glMemoryBarrier(GL_TEXTURE_FETCH_BARRIER_BIT);
}

void MeshMakerMarchingCubes::getVertices() {
	//glNamedBufferData(BufferBundle::instance().vertexBuff, 3 * numVerts * sizeof(GLfloat), nullptr, GL_DYNAMIC_DRAW);
	//glNamedBufferData(BufferBundle::instance().normalBuff, 3 * numVerts * sizeof(GLfloat), nullptr, GL_DYNAMIC_DRAW);
	glBindTextures(2, 8, BufferBundle::instance().pyramidA);
	glUseProgram(getVerticesCS.getProgram());
	getVerticesCS.setUniform1i("total", numVerts);
	getVerticesCS.setUniform1i("layers", layers);
	glDispatchCompute((numVerts + 63) / 64, 1, 1);	
}

void MeshMakerMarchingCubes::getTriangles() {
	//glNamedBufferData(BufferBundle::instance().elementBuff, 3 * numPolys * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);
	glBindTextures(2, 8, BufferBundle::instance().pyramidB);
	glUseProgram(getTrianglesCS.getProgram());
	getTrianglesCS.setUniform1i("total", numPolys);
	getTrianglesCS.setUniform1i("layers", layers);
	glDispatchCompute((numPolys + 63) / 64, 1, 1);
}

void MeshMakerMarchingCubes::getPositionsAndNormals() {
	glUseProgram(getPositionsAndNormalsCS.getProgram());
	getPositionsAndNormalsCS.setUniform1i("total", numVerts);
	getPositionsAndNormalsCS.setUniform1i("res", 512);
	getPositionsAndNormalsCS.setUniform1f("stride", 0.5);
	glDispatchCompute((numVerts + 63) / 64, 1, 1);
}

void MeshMakerMarchingCubes::setupFunction() {
	setupConcatenatedShader(getPointsCS, "ComputeShaders/Common/GetPoints.glsl", control.functionFile);
	getPointsCS.linkProgram();
	setupConcatenatedShader(getPositionsAndNormalsCS, "ComputeShaders/MC/GetPositionsAndNormalsMC.glsl", control.functionFile);
	getPositionsAndNormalsCS.linkProgram();
}
