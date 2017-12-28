#pragma once
#include "MeshMakerBase.h"

//concrete mesh maker using marching cubes
class MeshMakerMarchingCubes : public MeshMakerBase {
public:
	MeshMakerMarchingCubes(ControlPanel &c);
	~MeshMakerMarchingCubes() {};
	void makeMesh() override;


private:
	void buildBaseLayer();
	void getVertices();
	void getTriangles();
	void setupFunction() override;
	ofShader buildBaseLayerCS, getVerticesCS, getTrianglesCS;
};