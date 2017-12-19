#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	ofGLWindowSettings s;			// <-------- setup the GL context
	s.setGLVersion(4, 5);
	ofCreateWindow(s);
	ofRunApp(new ofApp());

}
