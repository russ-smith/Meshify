#pragma once

#include "ofMain.h"
#include "MeshMakerMarchingCubes.h"
#include "MeshMakerDualMarchingCubes.h"
#include "ControlPanel.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
	private:
		void changeAlgorithm();
		void setLightAndMaterial();
		ofEasyCam cam;
		ofShader renderShader;
		ofVec3f lightPos, lightColor;
		unique_ptr<MeshMakerBase> maker;
		ControlPanel control;
		bool needsRedraw = true;
};
