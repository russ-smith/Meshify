#include "MeshMakerMarchingCubes.h"

MeshMakerMarchingCubes::MeshMakerMarchingCubes(ControlPanel &c) : MeshMakerBase(c) {
	control.registerRedrawCallback([&]() { makeMesh(); });
	control.registerFunctionCallback([&]() { setupFunction(); });
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
	ofstream fout("logfile.txt");
	getPointsAndCases();
	/*GLuint* points = new GLuint[65 * 65 * 65];
	glGetTextureSubImage(BufferBundle::instance().baseLayerTex, 0, 0, 0, 0, 65, 65, 65, GL_RED_INTEGER, GL_UNSIGNED_INT, 65 * 65 * 65 * 4, points);
	for (size_t i = 0; i < 65 * 65 * 65; i++) {
		if (i % 65 == 0) fout << endl;
		if (i % (65 * 65) == 0) fout << endl;
		fout << (int)points[i];
	}
	fout << "\n\n\n\n\n\n\n\n";
	GLubyte* cases = new GLubyte[64 * 64 * 64];
	glGetTextureSubImage(BufferBundle::instance().cubeCaseTex, 0, 0, 0, 0, 64, 64, 64, GL_RED_INTEGER, GL_UNSIGNED_BYTE, 64 * 64 * 64, cases);
	for (size_t i = 0; i < 64 * 64 * 64; i++) {
		if (i % 64 == 0) fout << endl;
		if (i % (64 * 64) == 0) fout << endl;
		fout << setw(3) << (int)cases[i];
	}*/
	buildBaseLayer();
	buildPyramidsFromBaseLayer();
	GLuint* mappedBuff = static_cast<GLuint*>(glMapNamedBuffer(BufferBundle::instance().totalsBuff, GL_READ_ONLY));
	numVerts = mappedBuff[0];
	numPolys = mappedBuff[1];
	glUnmapNamedBuffer(BufferBundle::instance().totalsBuff);
	
	fout<< "\n\n\n" << numVerts << "  " << numPolys << "\n\n";
	getVertices();
	getTriangles();
	getPositionsAndNormals();
	control.setLabels(numVerts, numPolys);
}


void MeshMakerMarchingCubes::buildBaseLayer() {
	int layer = control.layers() - 1;
	glBindImageTexture(2, BufferBundle::instance().pyramidA[layer], 0, GL_FALSE, 0, GL_WRITE_ONLY, BufferBundle::FORMAT_BY_LAYER_A[layer]);
	glBindImageTexture(3, BufferBundle::instance().pyramidB[layer], 0, GL_FALSE, 0, GL_WRITE_ONLY, BufferBundle::FORMAT_BY_LAYER_B[layer]);
	glUseProgram(buildBaseLayerCS.getProgram());
	int numGroups = control.res() >> 3;
	glDispatchCompute(numGroups, numGroups, numGroups);
	glMemoryBarrier(GL_TEXTURE_FETCH_BARRIER_BIT);
}

void MeshMakerMarchingCubes::getVertices() {
	glNamedBufferData(BufferBundle::instance().vertexBuff, 3 * numVerts * sizeof(GLfloat), nullptr, GL_DYNAMIC_DRAW);
	glNamedBufferData(BufferBundle::instance().normalBuff, 3 * numVerts * sizeof(GLfloat), nullptr, GL_DYNAMIC_DRAW);
	glBindTextures(2, 8, BufferBundle::instance().pyramidA);
	glUseProgram(getVerticesCS.getProgram());
	getVerticesCS.setUniform1i("total", numVerts);
	getVerticesCS.setUniform1i("layers", control.layers());
	glDispatchCompute((numVerts + 63) / 64, 1, 1);	
	glMemoryBarrier(GL_BUFFER_UPDATE_BARRIER_BIT);
}

void MeshMakerMarchingCubes::getTriangles() {
	glNamedBufferData(BufferBundle::instance().elementBuff, 3 * numPolys * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);
	glBindTextures(2, 8, BufferBundle::instance().pyramidB);
	glUseProgram(getTrianglesCS.getProgram());
	getTrianglesCS.setUniform1i("total", numPolys);
	getTrianglesCS.setUniform1i("layers", control.layers());
	glDispatchCompute((numPolys + 63) / 64, 1, 1);
	glMemoryBarrier(GL_ELEMENT_ARRAY_BARRIER_BIT);
}

void MeshMakerMarchingCubes::getPositionsAndNormals() {
	glUseProgram(getPositionsAndNormalsCS.getProgram());
	getPositionsAndNormalsCS.setUniform1i("total", numVerts);
	getPositionsAndNormalsCS.setUniform1i("res", control.res());
	getPositionsAndNormalsCS.setUniform1f("stride", control.stride());
	getPositionsAndNormalsCS.setUniforms(control.functionParams);
	glDispatchCompute((numVerts + 63) / 64, 1, 1);
	glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
}

void MeshMakerMarchingCubes::setupFunction() {
	setupConcatenatedShader(getPointsCS, "ComputeShaders/Common/GetPoints.glsl", control.functionFile);
	getPointsCS.linkProgram();
	setupConcatenatedShader(getPositionsAndNormalsCS, "ComputeShaders/MC/GetPositionsAndNormalsMC.glsl", control.functionFile);
	getPositionsAndNormalsCS.linkProgram();
}

