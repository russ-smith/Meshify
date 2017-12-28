#include "ofApp.h"

int main( ){
	ofGLWindowSettings s;			
	s.setGLVersion(4, 5);
	ofCreateWindow(s);
	ofRunApp(new ofApp());
}
