//
//  GradientField.h
//  grayscott
//
//  Created by Gal Sasson on 6/9/15.
//
//

#ifndef __grayscott__GradientField__
#define __grayscott__GradientField__

#include <stdio.h>
#include "ofMain.h"
#include "ofxInterface.h"

using namespace ofxInterface;

class GradientField : public ofxInterface::Node
{
public:
	void setup(float w, float h);

	ofTexture& getTexture();


	void update(float dt);
	void draw();

	ofParameterGroup params;

private:


	ofFbo fbo;
	bool bFboDirty;
	void allocateFbo();

	ofVbo vbo;
	bool bModelColorsDirty;
	void createModel();
	void setModelColors();

	void initParams();
	ofParameter<float> defaultFeed;
	ofParameter<float> defaultKill;
	ofParameter<float> feedValues[3];
	ofParameter<float> killValues[3];
	void setDefaults();
	void onParamChanged(float& param);
};
#endif /* defined(__grayscott__GradientField__) */
