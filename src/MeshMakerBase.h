#pragma once
#include "ofMain.h"
#include "BufferBundle.h"
#include "ControlPanel.h"

class MeshMakerBase {
public:
	virtual ~MeshMakerBase() {};
	virtual void makeMesh() = 0;
	void render();
	

protected:
	MeshMakerBase(ControlPanel &c);
	void getPointsAndCases();
	void buildPyramidsFromBaseLayer();
	bool setupConcatenatedShader(ofShader & shdr, string shdrSrcFile, string fracFuncFile);
	virtual void setupFunction() = 0;

	ofShader getPointsCS, getCasesCS, buildNextLayerCS, buildFinalLayerCS;
	int numVerts, numPolys;
	ControlPanel &control;
};