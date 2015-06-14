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
#include "ofxGui.h"

using namespace ofxInterface;

class GradientField : public ofxInterface::Node
{
public:
	void setup(float w, float h);

	ofTexture& getTexture();

	void update(float dt);
	void draw(float magR, float magG, float magB, float magA);

	void initParams(const string& name);
	ofParameterGroup params;
	ofxPanel panel;

private:

	ofShader renderShader;
	ofPlanePrimitive plane;

	ofFbo fbo;
	bool bFboDirty;
	void allocateFbo();

	ofVbo vbo;
	bool bVboDirty;
	void createModel(ofVbo& vbo);
	void setModelColors(ofVbo& vbo);

	ofParameter<bool> bEnableField;
	ofParameter<ofVec4f> values[3];
	ofParameter<ofVec4f> background;

	void setDefaults();
	void onFloatParamChanged(float& param);
	void onBoolParamChanged(bool& params);
	void onVec4ParamChanged(ofVec4f& params);
};
#endif /* defined(__grayscott__GradientField__) */
