#pragma once
#include "ofMain.h"
#include "BufferBundle.h"
#include "ControlPanel.h"

class MeshMakerBase {
public:
	virtual ~MeshMakerBase() {};
	virtual void makeMesh() = 0;
	void render();
	virtual void setupFunction() = 0;

protected:
	MeshMakerBase(ControlPanel &c);
	void getPointsAndCases();
	void buildPyramidsFromBaseLayer();
	bool setupConcatenatedShader(ofShader & shdr, string shdrSrcFile, string fracFuncFile);

	ofShader getPointsCS, getCasesCS, buildNextLayerCS, buildFinalLayerCS;
	int layers, numVerts, numPolys;
	ControlPanel &control;
};