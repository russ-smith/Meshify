#pragma once
#include "ofMain.h"
#include "ofxGuiExtended.h"

//This class holds the GUI and backing variables.
//Other classes can register callbacks to respond to parameter changes
class ControlPanel {
public:
	ControlPanel();
	void setLabels(int verts, int polys);
	void registerRedrawCallback(function<void()> f);
	void registerFunctionCallback(function<void()> f);
	void registerAlgorithmCallback(function<void(int)> f);
	void registerExportCallback(function<void(string)> f);
	void draw();
	void toggleGUI(int which);
	int res();
	int layers();
	float stride();
	float zoom();
	ofParameterGroup functionParams, positionParams, lightParams, materialParams;
	ofParameter<bool> isWireframe;
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
	void exportMesh();

	//external callbacks registered by other objects
	function<void()> redrawCallback;
	function<void()> functionCallback;
	function<void(int)> algorithmCallback;
	function<void(string)> exportCallback;

	ofxPanelExtended meshGUI, renderGUI;
	ofxGuiMatrix resMatrix;
	ofxGuiMatrix algoMatrix;
	ofxMinimalButton loadButton, exportButton;
	ofParameter<int> iterations;
	ofParameter<float> paramA, paramB, paramC, paramD, paramE, paramF, paramG, paramH, extent, smoothness;
	ofParameter<ofVec3f> centre, light0Color, light1Color, diffColor;
	ofParameter<ofVec2f> light0Dir, light1Dir;
	ofParameter<bool> res64, res128, res256, res512, isMC, isDMC;
	ofxLabel vertices, polygons;

	int m_res, m_layers;
	bool isMeshGUIDrawing = true, isRenderGUIDrawing = true;
};