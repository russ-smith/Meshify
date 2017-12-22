#pragma once
#include "MeshMakerBase.h"

class MeshMakerDualMarchingCubes : public MeshMakerBase {
public:
	MeshMakerDualMarchingCubes(ControlPanel &c);
	~MeshMakerDualMarchingCubes() {};
	void makeMesh() override;

private:
	void buildBaseLayer();
	void setupFunction() override;
	ofShader buildBaseLayerCS;
};