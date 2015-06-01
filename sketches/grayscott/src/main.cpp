#include "ofMain.h"
#include "ofApp.h"
#include "ofGLProgrammableRenderer.h"

//========================================================================
int main( ){

	ofGLFWWindowSettings s;
	s.width = 1024;
	s.height = 768;
	s.setGLVersion(3, 2);

	ofCreateWindow(s);

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
