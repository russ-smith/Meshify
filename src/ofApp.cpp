#include "ofApp.h"
#include "BufferBundle.h"
//--------------------------------------------------------------
void ofApp::setup(){
	//instantiate buffers before doing anything else!
	BufferBundle::instance();
	control.registerAlgorithmCallback([this]() { changeAlgorithm(); });
	control.registerRedrawCallback([this]() {needsRedraw = true; });
	maker = make_unique<MeshMakerDualMarchingCubes>(control);
	maker->makeMesh();
	ofSetVerticalSync(true);
	ofSetFrameRate(30);
	lightColor = ofVec3f(1);
	renderShader.load("DrawingShaders/phong2lights");
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

void ofApp::changeAlgorithm() {
	
	maker = make_unique<MeshMakerMarchingCubes>(control);

	maker->makeMesh();
}

void ofApp::setLightAndMaterial() {
	ofVec3f col = control.lightParams.getVec3f("light0Color");
	renderShader.setUniform3f("light0Color", col);
	col = control.lightParams.getVec3f("light1Color");
	renderShader.setUniform3f("light1Color", col);
	ofVec2f dir = control.lightParams.getVec2f("light0Dir");
	ofVec3f realDir = ofVec3f(sin(dir.y)*cos(dir.x), cos(dir.y), sin(dir.y)*sin(dir.x));
	renderShader.setUniform3f("light0Dir", realDir);
	dir = control.lightParams.getVec2f("light1Dir");
	realDir = ofVec3f(sin(dir.y)*cos(dir.x), cos(dir.y), sin(dir.y)*sin(dir.x));
	renderShader.setUniform3f("light1Dir", realDir);
}


