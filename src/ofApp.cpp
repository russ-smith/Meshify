#include "ofApp.h"
#include "BufferBundle.h"
//--------------------------------------------------------------
void ofApp::setup(){
	//instantiate buffers before doing anything else!
	BufferBundle::instance();
	control.registerAlgorithmCallback([this](int i) { changeAlgorithm(i); });
	control.registerRedrawCallback([this]() {needsRedraw = true; });
	maker = make_unique<MeshMakerMarchingCubes>(control);
	maker->makeMesh();
	ofSetVerticalSync(true);
	ofSetFrameRate(30);
	lightColor = ofVec3f(1);
	renderShader.load("DrawingShaders/normalizedblinnphong");
	cam.setDistance(10);
	cam.setNearClip(0.5);
}

//--------------------------------------------------------------
void ofApp::update(){
	if (needsRedraw) {
		maker->makeMesh();
		needsRedraw = false;
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackgroundGradient(ofColor::lightSkyBlue, ofColor::black);
	ofEnableDepthTest();
	if (control.isWireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	cam.begin(); 
	float s = control.zoom();
	ofScale(s, s, s);
	renderShader.begin();
	renderShader.setUniformMatrix4f("normalMatrix", ofGetCurrentNormalMatrix());
	setLightAndMaterial();
	maker->render();
	renderShader.end();
	cam.end();
	ofSetColor(255);

	//GUI needs depth and cull disabled to render properly
	ofDisableDepthTest();
	glDisable(GL_CULL_FACE);
	control.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == '1')
		control.toggleGUI(0);
	else if (key == '2')
		control.toggleGUI(1);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

void ofApp::changeAlgorithm(int algo) {
	
	if (algo == 0)
		maker = make_unique<MeshMakerMarchingCubes>(control);
	else
		maker = make_unique<MeshMakerDualMarchingCubes>(control);
}

void ofApp::setLightAndMaterial() {
	ofVec3f col = control.lightParams.getVec3f("Light0Color");
	renderShader.setUniform3f("Light0Color", col);
	col = control.lightParams.getVec3f("Light1Color");
	renderShader.setUniform3f("Light1Color", col);
	ofVec2f dir = control.lightParams.getVec2f("Light0Dir");
	ofVec3f realDir = ofVec3f(sin(dir.y)*cos(dir.x), cos(dir.y), sin(dir.y)*sin(dir.x));
	renderShader.setUniform3f("Light0Dir", realDir);
	dir = control.lightParams.getVec2f("Light1Dir");
	realDir = ofVec3f(sin(dir.y)*cos(dir.x), cos(dir.y), sin(dir.y)*sin(dir.x));
	renderShader.setUniform3f("Light1Dir", realDir);
	renderShader.setUniforms(control.materialParams);
}


