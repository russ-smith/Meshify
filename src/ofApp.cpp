#include "ofApp.h"
#include "BufferBundle.h"
//--------------------------------------------------------------
void ofApp::setup(){
	//instantiate buffers before doing anything else!
	BufferBundle::instance();
	control.registerAlgorithmCallback([&]() { changeAlgorithm(); });
	maker = make_unique<MeshMakerMarchingCubes>(control);
	maker->makeMesh();
	ofSetVerticalSync(true);
	ofSetFrameRate(30);
	lightColor = ofVec3f(1);
	renderShader.load("DrawingShaders/lightsphong");
	cam.setDistance(10);
	cam.setNearClip(1);
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackgroundGradient(ofColor::lightSkyBlue, ofColor::black);
	ofEnableDepthTest();
	glEnable(GL_CULL_FACE);
	cam.begin(); 
	float s = control.zoom();
	ofScale(s, s, s);
	lightPos.set(150 * sin(ofGetElapsedTimef()), 150 * cos(ofGetElapsedTimef()), 200);
	renderShader.begin();
	renderShader.setUniformMatrix4f("normalMatrix", ofGetCurrentNormalMatrix());
	renderShader.setUniform3f("lightPos", lightPos * ofGetCurrentViewMatrix());
	renderShader.setUniform3f("lightColor", lightColor);
	maker->render();
	renderShader.end();
	cam.end();
	ofSetColor(255);

	//GUI needs depth and cull disabled to render properly
	ofDisableDepthTest();
	glDisable(GL_CULL_FACE);
	control.GUI.draw();
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


