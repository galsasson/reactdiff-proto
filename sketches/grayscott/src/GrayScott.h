//
//  GrayScott.h
//  grayscott
//
//  Created by Gal Sasson on 5/31/15.
//
//

#ifndef __grayscott__GrayScott__
#define __grayscott__GrayScott__

#include <stdio.h>
#include "ofMain.h"
#include "ofxInterface.h"
#include "ofxGui.h"

#define A_COLOR ofColor(255, 0, 0)
#define B_COLOR ofColor(0, 255, 0)

using namespace ofxInterface;

class GrayScott : public ofxInterface::Node
{
public:

	GrayScott();
	~GrayScott();

	void setupGui(ofxPanel& panel);
	void seedGrid();

	void clearDiffusionMap();

	void update(float dt);
	void draw();
	void drawSimulation();
	void drawDiffusionMap();

private:

	ofParameterGroup params;
	ofParameter<float> feedRate;
	ofParameter<float> killRate;
	ofParameter<float> aDiffRate;
	ofParameter<float> bDiffRate;
	ofParameter<int> simSteps;
	ofParameter<float> timestep;
	ofParameter<float> minColor;
	ofParameter<float> maxColor;
	ofParameter<bool> bRenderWithShader;
	ofParameter<float> borderWidth;
	ofParameter<float> borderSoftness;
	ofParameter<bool>bDiffusionMapMode;

	ofShader simShader;
	ofShader renderShader;
	ofPlanePrimitive plane;

	void allocateFbos(int w, int h);
	void swapFbos();
	ofFbo* gridFbo;
	ofFbo* nextGridFbo;
	void simulationStep(float dt);

	bool bTouching;
	int touchId;
	void onTouchDown(TouchEvent& event);
	void onTouchMove(TouchEvent& event);
	void onTouchUp(TouchEvent& event);

	ofFbo diffusionFlowFbo;
	void drawRandomDiffusion(float x, float y);
	void drawDiffusionBump(float x, float y);
};
#endif /* defined(__grayscott__GrayScott__) */
