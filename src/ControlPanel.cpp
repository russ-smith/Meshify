#include "ControlPanel.h"


ControlPanel::ControlPanel() {
	DEFAULT_DIR = ofToDataPath("", true);
	m_res = 64;
	m_layers = 5;

	//setup main GUI
	meshGUI.setup("Controls");
	functionParams.setName("Function Parameters");
	positionParams.setName("Position Parameters");
	iterations.addListener(this, &ControlPanel::iParamsChanged);
	paramA.addListener(this, &ControlPanel::fParamsChanged);
	paramB.addListener(this, &ControlPanel::fParamsChanged);
	paramC.addListener(this, &ControlPanel::fParamsChanged);
	paramD.addListener(this, &ControlPanel::fParamsChanged);
	paramE.addListener(this, &ControlPanel::fParamsChanged);
	paramF.addListener(this, &ControlPanel::fParamsChanged);
	paramG.addListener(this, &ControlPanel::fParamsChanged);
	paramH.addListener(this, &ControlPanel::fParamsChanged);
	extent.addListener(this, &ControlPanel::fParamsChanged);
	centre.addListener(this, &ControlPanel::vParamsChanged);
	functionParams.add(iterations.set("Iterations", 15, 1, 100));
	functionParams.add(paramA.set("A", 0, -1, 1));
	functionParams.add(paramB.set("B", 0, -1, 1));
	functionParams.add(paramC.set("C", 0, -1, 1));
	functionParams.add(paramD.set("D", 0, -1, 1));
	functionParams.add(paramE.set("E", 0, -1, 1));
	functionParams.add(paramF.set("F", 0, -1, 1));
	functionParams.add(paramG.set("G", 0, -1, 1));
	functionParams.add(paramH.set("H", 0, -1, 1));
	positionParams.add(centre.set("Centre", ofVec3f(0), ofVec3f(-5), ofVec3f(5)));
	positionParams.add(extent.set("Extent", 4, 0.1, 10));
	resMatrix.setup("Resolution", 4);
	resMatrix.setName("Grid Resolution");
	resMatrix.add(new ofxMinimalToggle(res64.set("64", true)));
	resMatrix.add(new ofxMinimalToggle(res128.set("128", false)));
	resMatrix.add(new ofxMinimalToggle(res256.set("256", false)));
	resMatrix.add(new ofxMinimalToggle(res512.set("512", false)));
	res512.addListener(this, &ControlPanel::resChanged);
	resMatrix.setBorderColor(ofColor::darkGrey);
	resMatrix.setElementHeight(26);
	resMatrix.allowMultipleActiveToggles(false);
	meshGUI.add(new ofxGuiGroup(functionParams));
	meshGUI.add(new ofxGuiGroup(positionParams));
	meshGUI.add(&resMatrix);
	meshGUI.add(new ofxMinimalToggle(isWireframe.set("Wireframe", false)));
	algoMatrix.setup("Isosurface Algorithm", 1);
	algoMatrix.setName("Algorithm");
	algoMatrix.add(new ofxMinimalToggle(isMC.set("Marching Cubes", true)));
	algoMatrix.add(new ofxMinimalToggle(isDMC.set("Dual Marching Cubes", false)));
	isDMC.addListener(this, &ControlPanel::algoChanged);
	algoMatrix.setBorderColor(ofColor::darkGrey);
	algoMatrix.setElementHeight(16);
	algoMatrix.allowMultipleActiveToggles(false);
	meshGUI.add(&algoMatrix);
	loadButton.setup("   Load new function", 200, 30);
	loadButton.addListener(this, &ControlPanel::loadFunction);
	meshGUI.add(&loadButton);
	
	vertices.setup("Vertices", "0", 200, 20);
	polygons.setup("Polygons", "0", 200, 20);
	meshGUI.add(&vertices);
	meshGUI.add(&polygons);
	functionFile = ofToDataPath("DistanceFunctions/polyhedra.glsl");

	//setup rendering GUI
	renderGUI.setup("Rendering");
	lightParams.setName("Light Parameters");
	lightParams.add(light0Color.set("light0Color", ofVec3f(0.5), ofVec3f(0), ofVec3f(1)));
	lightParams.add(light0Dir.set("light0Dir", ofVec2f(1), ofVec2f(0), ofVec2f(TWO_PI, PI)));
	lightParams.add(light1Color.set("light1Color", ofVec3f(0.5), ofVec3f(0), ofVec3f(1)));
	lightParams.add(light1Dir.set("light1Dir", ofVec2f(2), ofVec2f(0), ofVec2f(TWO_PI, PI)));
	renderGUI.add(new ofxGuiGroup(lightParams));
	renderGUI.setPosition(ofGetWidth() - 200, 0);
}

//callback methods for parameter changes
void ControlPanel::fParamsChanged(float & param) {
	if (redrawCallback)
		redrawCallback();
}

void ControlPanel::vParamsChanged(ofVec3f & param) {
	if (redrawCallback)
		redrawCallback();
}

void ControlPanel::iParamsChanged(int & param) {
	if (redrawCallback)
		redrawCallback();
}

void ControlPanel::resChanged(bool & param) {	
	if (res64) {
		m_res = 64; m_layers = 5;
	}
	if (res128) {
		m_res = 128; m_layers = 6;
	}
	if (res256) { 
		m_res = 256; m_layers = 7; 
	}
	if (res512) { 
		m_res = 512; m_layers = 8; 
	}
	if (redrawCallback)
		redrawCallback();
}

void ControlPanel::algoChanged(bool & param) {
	if(algorithmCallback)
		algorithmCallback();
	if (redrawCallback)
		redrawCallback();
}

//update vertex and polygon count display
void ControlPanel::setLabels(int verts, int polys) {
	vertices.setup("Vertices", to_string(verts), 200, 20);
	polygons.setup("Polygons", to_string(polys), 200, 20);
}

void ControlPanel::registerRedrawCallback(function<void()> f) {
	redrawCallback = f;
}

void ControlPanel::registerFunctionCallback(function<void()> f) {
	functionCallback = f;
}

void ControlPanel::registerAlgorithmCallback(function<void()> f) {
	algorithmCallback = f;
}

void ControlPanel::draw() {
	if (isMeshGUIDrawing) {
		meshGUI.setPosition(0, 0);
		meshGUI.draw();
	}
	if (isRenderGUIDrawing) {
		renderGUI.setPosition(ofGetWidth() - 200, 0);
		renderGUI.draw();
	}
}

int ControlPanel::res() {
	return m_res;
}

int ControlPanel::layers() {
	return m_layers;
}

float ControlPanel::stride() {
	return extent / float(m_res);
}

float ControlPanel::zoom() {
	return 5 / extent;
}

//load fractal function file from disk
void ControlPanel::loadFunction() {
	ofSetDataPathRoot(DEFAULT_DIR);
	functionFile = ofSystemLoadDialog("Select fractal function", false, ofToDataPath("")).getPath();
	if (functionCallback)
		functionCallback();
	if (redrawCallback)
		redrawCallback();
}
