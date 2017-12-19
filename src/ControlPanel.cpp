#include "ControlPanel.h"


ControlPanel::ControlPanel() {
	DEFAULT_DIR = ofToDataPath("", true);
	m_res = 64;
	m_layers = 5;
	GUI.setup("Controls");
	fractalParams.setName("Fractal Parameters");
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
	fractalParams.add(iterations.set("Iterations", 15, 1, 100));
	fractalParams.add(paramA.set("A", 0, -1, 1));
	fractalParams.add(paramB.set("B", 0, -1, 1));
	fractalParams.add(paramC.set("C", 0, -1, 1));
	fractalParams.add(paramD.set("D", 0, -1, 1));
	fractalParams.add(paramE.set("E", 0, -1, 1));
	fractalParams.add(paramF.set("F", 0, -1, 1));
	fractalParams.add(paramG.set("G", 0, -1, 1));
	fractalParams.add(paramH.set("H", 0, -1, 1));
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
	GUI.add(new ofxGuiGroup(fractalParams));
	GUI.add(new ofxGuiGroup(positionParams));
	GUI.add(&resMatrix);
	algoMatrix.setup("Isosurface Algorithm", 1);
	algoMatrix.setName("Algorithm");
	algoMatrix.add(new ofxMinimalToggle(isMC.set("Marching Cubes", true)));
	algoMatrix.add(new ofxMinimalToggle(isDMC.set("Dual Marching Cubes", false)));
	isDMC.addListener(this, &ControlPanel::algoChanged);
	algoMatrix.setBorderColor(ofColor::darkGrey);
	algoMatrix.setElementHeight(16);
	algoMatrix.allowMultipleActiveToggles(false);
	GUI.add(&algoMatrix);
	loadButton.setup("   Load new function", 200, 30);
	loadButton.addListener(this, &ControlPanel::loadFunction);
	GUI.add(&loadButton);
	GUI.add(new ofxMinimalToggle(isWireframe.set("Wireframe", false)));
	vertices.setup("Vertices", "0", 200, 20);
	polygons.setup("Polygons", "0", 200, 20);
	GUI.add(&vertices);
	GUI.add(&polygons);
	GUI.add(new ofxSlider<float>(fps.set("FPS", 0, 0, 30)));
	functionFile = ofToDataPath("DistanceFunctions/sphere.glsl");
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
