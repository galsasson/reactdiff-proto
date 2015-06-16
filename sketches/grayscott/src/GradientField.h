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

#include "RemoteController.h"

using namespace ofxInterface;

class GradientField : public ofxInterface::Node
{
public:
	void setup(float w, float h);

	ofTexture& getTexture();

	void update(float dt);
	void draw();

	ofParameterGroup params;

	void onRemoteParamChanged(RemoteController& controller);

private:

	ofShader renderShader;
	ofPlanePrimitive plane;

	ofFbo fbo;
	bool bFboDirty;
	void allocateFbo();

	ofVbo vbo;
	bool bModelColorsDirty;
	void createModel();
	void setModelColors();

	void initParams();
	ofParameter<bool> bUseGradientField;
	ofParameter<float> defaultFeed;
	ofParameter<float> defaultKill;
	ofParameter<float> defaultADiffRate;
//	ofParameter<float> defaultBDiffRate;
	ofParameter<float> feedValues[3];
	ofParameter<float> killValues[3];
	ofParameter<float> aDiffValues[3];
//	ofParameter<float> bDiffValues[3];

	void setDefaults();
	void onFloatParamChanged(float& param);
	void onBoolParamChanged(bool& params);
};
#endif /* defined(__grayscott__GradientField__) */
