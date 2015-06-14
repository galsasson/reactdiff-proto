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

#include "GradientField.h"
#include "ZoomablePlane.h"

#define A_COLOR ofFloatColor(1, 0, 0)
#define B_COLOR ofFloatColor(0, 1, 0)

using namespace ofxInterface;

class GrayScott : public ofxInterface::Node
{
public:

	GrayScott();
	~GrayScott();

	void setupGui(ofxPanel& panel);
	void clearGrid();
	void seedGrid();

	void clearDiffusionMap();

	void update(float dt);
	void draw();
	void drawSimulation();
	void drawDiffusionMap();

private:

	ofParameterGroup params;
	ofParameter<int> simSteps;
	ofParameter<float> timestep;

	ofParameterGroup renderParams;
	ofParameter<bool> bRenderWithShader;
	ofParameter<float> minVal;
	ofParameter<float> maxVal;
	ofParameter<ofFloatColor> minColor;
	ofParameter<ofFloatColor> maxColor;
	ofParameter<float> borderWidth;
	ofParameter<float> borderSoftness;
	ofParameter<float> roundEdge1;
	ofParameter<float> roundEdge2;

	ofShader simShader;
	ofShader renderShader;
	ofPlanePrimitive plane;

	ofPlanePrimitive renderPlane;
//	ZoomablePlane zPlane;

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

	GradientField gradField;

	void onKeyPressed(ofKeyEventArgs& args);
};
#endif /* defined(__grayscott__GrayScott__) */
