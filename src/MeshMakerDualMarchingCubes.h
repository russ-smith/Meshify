#pragma once
#include "MeshMakerBase.h"

//concrete mesh maker using dual marching cubes
class MeshMakerDualMarchingCubes : public MeshMakerBase {
public:
	MeshMakerDualMarchingCubes(ControlPanel &c);
	~MeshMakerDualMarchingCubes() {};
	void makeMesh() override;

private:
	void buildBaseLayer();
	void getEdges();
	void getVertices();
	void getPositionsAndNormals();
	void getTriangles();
	void setupFunction() override;
	ofShader buildBaseLayerCS, getEdgesCS, getVerticesCS, getPositionsAndNormalsCS, getTrianglesCS;
};