#include "MeshMakerBase.h"

void MeshMakerBase::render() {
	glBindVertexArray(BufferBundle::instance().VAO);
	glDrawElements(GL_TRIANGLES, 3 * numPolys, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

MeshMakerBase::MeshMakerBase(ControlPanel &c) : control(c)  {
	setupConcatenatedShader(getPointsCS, "ComputeShaders/Common/GetPoints.glsl", control.functionFile);
	getPointsCS.linkProgram();
	getCasesCS.setupShaderFromFile(GL_COMPUTE_SHADER, "ComputeShaders/Common/GetCases.glsl");
	getCasesCS.linkProgram();
	buildNextLayerCS.setupShaderFromFile(GL_COMPUTE_SHADER, "ComputeShaders/Common/BuildNextLayer.glsl");
	buildNextLayerCS.linkProgram();
	buildFinalLayerCS.setupShaderFromFile(GL_COMPUTE_SHADER, "ComputeShaders/Common/BuildFinalLayer.glsl");
	buildFinalLayerCS.linkProgram();
}

void MeshMakerBase::getPointsAndCases() {
	glUseProgram(getPointsCS.getProgram());
	getPointsCS.setUniform1i("res", control.res());
	getPointsCS.setUniform1f("stride", control.stride());
	getPointsCS.setUniforms(control.fractalParams);
	int numGroups = control.res() >> 2;
	glDispatchCompute(numGroups+1, numGroups+1, numGroups+1);
	glMemoryBarrier(GL_TEXTURE_FETCH_BARRIER_BIT);
	glUseProgram(getCasesCS.getProgram());
	getCasesCS.setUniform1i("resMinus1", control.res()-1);
	glDispatchCompute(numGroups, numGroups, numGroups);
	glMemoryBarrier(GL_TEXTURE_FETCH_BARRIER_BIT);
}

void MeshMakerBase::buildPyramidsFromBaseLayer() {
	for (size_t i = control.layers() - 1; i > 2; i--) {
		int numGroups = 1 << (i - 2);
		glBindTextureUnit(2, BufferBundle::instance().pyramidA[i]);
		glBindImageTexture(2, BufferBundle::instance().pyramidA[i - 1], 0, GL_FALSE, 0, GL_WRITE_ONLY, BufferBundle::FORMAT_BY_LAYER_A[i - 1]);
		glBindTextureUnit(3, BufferBundle::instance().pyramidB[i]);
		glBindImageTexture(3, BufferBundle::instance().pyramidB[i - 1], 0, GL_FALSE, 0, GL_WRITE_ONLY, BufferBundle::FORMAT_BY_LAYER_B[i - 1]);
		glUseProgram(buildNextLayerCS.getProgram());
		glDispatchCompute(numGroups, numGroups, numGroups);
		glMemoryBarrier(GL_TEXTURE_FETCH_BARRIER_BIT);
	}
	glBindTextureUnit(2, BufferBundle::instance().pyramidA[2]);
	glBindTextureUnit(3, BufferBundle::instance().pyramidB[2]);
	glBindImageTexture(2, BufferBundle::instance().pyramidA[1], 0, GL_FALSE, 0, GL_WRITE_ONLY, BufferBundle::FORMAT_BY_LAYER_A[1]);
	glBindImageTexture(3, BufferBundle::instance().pyramidB[1], 0, GL_FALSE, 0, GL_WRITE_ONLY, BufferBundle::FORMAT_BY_LAYER_B[1]);
	glBindImageTexture(4, BufferBundle::instance().pyramidA[0], 0, GL_FALSE, 0, GL_WRITE_ONLY, BufferBundle::FORMAT_BY_LAYER_A[0]);
	glBindImageTexture(5, BufferBundle::instance().pyramidB[0], 0, GL_FALSE, 0, GL_WRITE_ONLY, BufferBundle::FORMAT_BY_LAYER_B[0]);
	glUseProgram(buildFinalLayerCS.getProgram());
	glDispatchCompute(1, 1, 1);
	glMemoryBarrier(GL_BUFFER_UPDATE_BARRIER_BIT);
}





bool MeshMakerBase::setupConcatenatedShader(ofShader & shdr, string shdrSrcFile, string fracFuncFile) {
	ofBuffer buffer = ofBufferFromFile(shdrSrcFile);
	ofBuffer funcBuffer = ofBufferFromFile(fracFuncFile);
	buffer.append(funcBuffer.getText());
	string absoluteFilePath = ofFilePath::getAbsolutePath(shdrSrcFile, true);
	string sourceDirectoryPath = ofFilePath::getEnclosingDirectory(absoluteFilePath, false);
	if (funcBuffer.size()) {
		return shdr.setupShaderFromSource(GL_COMPUTE_SHADER, buffer.getText(), sourceDirectoryPath);
	}
	else {
		ofLogError("ofShader") << "setupShaderFromFile(): couldn't load shader from \"" << fracFuncFile << "\"";
		return false;
	}
}
