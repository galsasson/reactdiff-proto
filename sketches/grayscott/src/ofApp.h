#pragma once

#include "ofMain.h"
#include "ofxInterface.h"
#include "ofxGui.h"

#include "GrayScott.h"
#include "OSCListener.h"

using namespace ofxInterface;

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

	ofxInterface::Node scene;

	ofParameter<float> scale;
	ofVec2f translate;
	GrayScott* grayScott;

	OSCListener* oscListener;

//	ofEasyCam cam;
	ofCamera cam;

	bool drawDebug;

	ofxPanel gui;
	void setPreset(int i);
	void loadPreset(int i);
	void savePreset(int i);
	int currentPreset;

	stringstream controls;
	bool bShowControls;
};
