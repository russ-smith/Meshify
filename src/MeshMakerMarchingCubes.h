#pragma once
#include "MeshMakerBase.h"

class MeshMakerMarchingCubes : public MeshMakerBase {
public:
	MeshMakerMarchingCubes(ControlPanel &c);
	~MeshMakerMarchingCubes() {};
	void makeMesh() override;


private:
	void buildBaseLayer();
	void getVertices();
	void getTriangles();
	void getPositionsAndNormals();
	void setupFunction() override;
	ofShader buildBaseLayerCS, getVerticesCS, getTrianglesCS, getPositionsAndNormalsCS;
};