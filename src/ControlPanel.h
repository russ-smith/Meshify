#pragma once
#include "ofMain.h"
#include "ofxGuiExtended.h"

//This class holds the GUI and backing variables.
//Other classes can query the variables to see if they need to take action.
class ControlPanel {
public:
	ControlPanel();
	void setLabels(int verts, int polys);
	void registerRedrawCallback(function<void()> f);
	void registerFunctionCallback(function<void()> f);
	void registerAlgorithmCallback(function<void()> f);
	void draw();
	
	int res();
	int layers();
	float stride();
	float zoom();
	ofParameterGroup functionParams, positionParams, lightParams, materialParams;
	string functionFile;
	string DEFAULT_DIR;

protected:
	//internal callbacks called by ofParameter changes
	void fParamsChanged(float &param);
	void vParamsChanged(ofVec3f &param);
	void iParamsChanged(int & param);
	void resChanged(bool & param);
	void algoChanged(bool & param);
	void loadFunction();

	//external callbacks registered by other objects
	function<void()> redrawCallback;
	function<void()> functionCallback;
	function<void()> algorithmCallback;

	ofxPanelExtended meshGUI, renderGUI;
	ofxGuiMatrix resMatrix;
	ofxGuiMatrix algoMatrix;
	ofxMinimalButton loadButton;
	ofParameter<int> iterations;
	ofParameter<float> paramA, paramB, paramC, paramD, paramE, paramF, paramG, paramH, extent;
	ofParameter<ofVec3f> centre, light0Color, light1Color;
	ofParameter<ofVec2f> light0Dir, light1Dir;
	ofParameter<bool> res64, res128, res256, res512, isMC, isDMC, isWireframe;
	ofxLabel vertices, polygons;

	int m_res, m_layers;
	bool isMeshGUIDrawing = true, isRenderGUIDrawing = true;
};