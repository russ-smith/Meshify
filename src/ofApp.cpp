#include "ofApp.h"
#include "BufferBundle.h"
//--------------------------------------------------------------
void ofApp::setup(){
	//instantiate buffers before doing anything else!
	BufferBundle::instance();
	control.registerAlgorithmCallback([this]() { changeAlgorithm(); });
	control.registerRedrawCallback([this]() {needsRedraw = true; });
	maker = make_unique<MeshMakerMarchingCubes>(control);
	maker->makeMesh();
	ofSetVerticalSync(true);
	ofSetFrameRate(30);
	lightColor = ofVec3f(1);
	renderShader.load("DrawingShaders/phong2lights");
	cam.setDistance(10);
	cam.setNearClip(1);
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
	glEnable(GL_CULL_FACE);
	cam.begin(); 
	float s = control.zoom();
	ofScale(s, s, s);
	//lightPos.set(150 * sin(ofGetElapsedTimef()), 150 * cos(ofGetElapsedTimef()), 200);
	renderShader.begin();
	renderShader.setUniformMatrix4f("normalMatrix", ofGetCurrentNormalMatrix());
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
	maker->makeMesh();
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

void ofApp::changeAlgorithm() {
	
	maker = make_unique<MeshMakerMarchingCubes>(control);

	maker->makeMesh();
}

void ofApp::setLightAndMaterial() {
	
}


