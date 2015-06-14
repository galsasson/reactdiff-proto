//
//  ZoomablePlane.h
//  grayscott
//
//  Created by Gal Sasson on 6/14/15.
//
//

#ifndef __grayscott__ZoomablePlane__
#define __grayscott__ZoomablePlane__

#include <stdio.h>
#include "ofMain.h"
#include "ofxGui.h"

class ZoomablePlane
{
public:

	void setup(int rows, int cols);

	void setupGui(ofxPanel& panel);
	
	void draw();

private:
	int rows;
	int cols;

	void buildModel();
	void setHeights();
	ofVbo vbo;

	ofParameterGroup params;
	ofParameter<float> radius;
	ofParameter<float> height;

	void onFloatParamChanged(float& param);

};
#endif /* defined(__grayscott__ZoomablePlane__) */
