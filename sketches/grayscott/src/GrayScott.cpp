//
//  GrayScott.cpp
//  grayscott
//
//  Created by Gal Sasson on 5/31/15.
//
//

#include "GrayScott.h"

GrayScott::GrayScott()
{
	bTouching = false;

	ofDisableArbTex();

//	setSize(4096, 4096);
	setSize(1024, 768);

	params.setName("GrayScott");
	params.add(simSteps.set("Steps", 1, 1, 40));
	params.add(timestep.set("timestep", 1.0f, 0.0f, 10.0f));

	renderParams.setName("Render Shader");
	renderParams.add(bRenderWithShader.set("render shader", false));
	renderParams.add(minVal.set("minVal", 0.0f, 0.0f, 1.0f));
	renderParams.add(maxVal.set("maxVal", 1.0f, 0.0f, 1.0f));
	renderParams.add(minColor.set("minColor", ofFloatColor(0.2), ofFloatColor(0), ofFloatColor(1)));
	renderParams.add(maxColor.set("maxColor", ofFloatColor(0.1), ofFloatColor(0), ofFloatColor(1)));
//	params.add(minColor.set("minColor", 0.2f, 0.0f, 1.0f));
//	params.add(maxColor.set("maxColor", 0.4f, 0.0f, 1.0f));
	renderParams.add(borderWidth.set("borderWidth", 0.1f, 0.0f, 1.0f));
	renderParams.add(borderSoftness.set("borderSoftness", 0.01f, 0.0f, 0.3f));
	renderParams.add(roundEdge1.set("roundEdge1", 0.48, 0.0, 1.0));
	renderParams.add(roundEdge2.set("roundEdge2", 0.43, 0.0, 1.0));


	allocateFbos(getWidth(), getHeight());
	seedGrid();
	clearDiffusionMap();

	simShader.load("shaders/grayscott_simulation");
//	simShader.load("shaders/turing_simulation");
	renderShader.load("shaders/grayscott_render");

	plane.mapTexCoords(0, 0, 1, 1);
	plane.setWidth(getWidth());
	plane.setHeight(getHeight());

	renderPlane.mapTexCoords(0, 0, 1, 1);
	renderPlane.setWidth(getWidth());
	renderPlane.setHeight(getHeight());
	renderPlane.setResolution(getWidth(), getHeight());

//	zPlane.setup(getWidth(), getHeight());

	ofAddListener(eventTouchDown, this, &GrayScott::onTouchDown);
	ofAddListener(eventTouchMove, this, &GrayScott::onTouchMove);
	ofAddListener(eventTouchUp, this, &GrayScott::onTouchUp);



	// parameters field
	paramsField.setup(getWidth(), getHeight());
	paramsField.params.setName("Simulation Params");
	paramsField.params.add("Default");
	paramsField.params.get<ofVec4f>(0).set("Default", ofVec4f(0.055f, 0.062f, 0.1f, 1.0f), ofVec4f(0.055f, 0.062f, 0.1f, 1.0f), ofVec4f(0.1f, 0.1f, 0.1f, 1.0f));
	for (int i=0; i<3; i++) {
		paramsField.params.add("User " + ofToString(i+1));
		paramsField.params.get<ofVec4f>(i+1).set("User " + ofToString(i+1), ofVec4f(0.055f, 0.062f, 0.1f, 1.0f), ofVec4f(0.055f, 0.062f, 0.1f, 1.0f), ofVec4f(0.1f, 0.1f, 0.1f, 1.0f));
	}
	paramsField.deactivate();
	addChild(&paramsField);

	// min colors
	minColorField.setup(getWidth(), getHeight());
	minColorField.setName("Min Color");
	minColorField.params.get<ofVec4f>(0).set("Default", ofVec4f(0), ofVec4f(0), ofVec4f(1));
	for (int i=0; i<3; i++) {
		minColorField.params.get<ofVec4f>(i+1).set("User " + ofToString(i+1), ofVec4f(0), ofVec4f(0), ofVec4f(1));
	}
	minColorField.deactivate();
	addChild(&minColorField);

	// max colors
	maxColorField.setup(getWidth(), getHeight());
	maxColorField.params.setName("Max Color");
	maxColorField.params.get<ofVec4f>(0).set("Default", ofVec4f(1), ofVec4f(0), ofVec4f(1));
	for (int i=0; i<3; i++) {
		maxColorField.params.get<ofVec4f>(i+1).set("User " + ofToString(i+1), ofVec4f(1), ofVec4f(0), ofVec4f(1));
	}
	maxColorField.deactivate();
	addChild(&maxColorField);


	ofAddListener(ofEvents().keyPressed, this, &GrayScott::onKeyPressed);

	ofLogNotice("GrayScott") << "initialized!";
}

void GrayScott::setupGui(ofxPanel& panel)
{
	panel.setup(params);
	panel.add(renderParams);
//	panel.add(gradField.params);
//	zPlane.setupGui(panel);
}

GrayScott::~GrayScott()
{

}

void GrayScott::seedGrid()
{
	gridFbo->begin();
	ofFill();
	ofSetColor(A_COLOR);
	ofDrawRectangle(0, 0, gridFbo->getWidth(), gridFbo->getHeight());
//	ofSetColor(B_COLOR);
//	ofDrawCircle(gridFbo->getWidth()/2, gridFbo->getHeight()/2, 10);
	gridFbo->end();
}


void GrayScott::update(float dt)
{
	paramsField.update(dt);
	minColorField.update(dt);
	maxColorField.update(dt);

	if (!bTouching) {
		for (int i=0; i<simSteps; i++) {
			simulationStep(dt);
		}
	}
}

void GrayScott::draw()
{
	drawSimulation();
}

void GrayScott::drawSimulation()
{
	if (bRenderWithShader) {
		renderShader.begin();
		renderShader.setUniformTexture("tex0", gridFbo->getTexture(), 0);
		renderShader.setUniform2f("tex0_size", gridFbo->getWidth(), gridFbo->getHeight());
		renderShader.setUniform1f("minVal", minVal);
		renderShader.setUniform1f("maxVal", maxVal);
		renderShader.setUniform4f("minColor", minColor->r, minColor->g, minColor->b, minColor->a);
		renderShader.setUniform4f("maxColor", maxColor->r, maxColor->g, maxColor->b, maxColor->a);
		renderShader.setUniform1f("borderWidth", borderWidth);
		renderShader.setUniform1f("softness", borderSoftness);
		renderShader.setUniform1f("roundEdge1", roundEdge1);
		renderShader.setUniform1f("roundEdge2", roundEdge2);

		ofPushMatrix();
		ofTranslate(getWidth()/2, getHeight()/2);
		ofSetColor(255);
//		zPlane.draw();
		renderPlane.draw();
		ofPopMatrix();

		renderShader.end();
	}
	else {
		ofSetColor(255);
		gridFbo->draw(0, 0);
	}
}

void GrayScott::drawDiffusionMap()
{
	ofSetColor(255);
	diffusionFlowFbo.draw(0, 0);
}


void GrayScott::allocateFbos(int w, int h)
{
	ofFbo::Settings s;
	s.width = w;
	s.height = h;
	s.internalformat = GL_RGBA32F;
	s.minFilter = GL_NEAREST;
	s.maxFilter = GL_NEAREST;

	gridFbo = new ofFbo();
	gridFbo->allocate(s);
	gridFbo->begin();
	ofClear(0);
	gridFbo->end();

	nextGridFbo = new ofFbo();
	nextGridFbo->allocate(s);
	nextGridFbo->begin();
	ofClear(0);
	nextGridFbo->end();

	diffusionFlowFbo.allocate(s);
	diffusionFlowFbo.begin();
	ofClear(ofFloatColor(0.52f));
//	ofClear(ofFloatColor(0.1, 0, 0, 1));
	diffusionFlowFbo.end();
}

void GrayScott::swapFbos()
{
	ofFbo* tmp = gridFbo;
	gridFbo = nextGridFbo;
	nextGridFbo = tmp;
}

void GrayScott::simulationStep(float dt)
{
	ofDisableDepthTest();

	nextGridFbo->begin();
	ofClear(0, 0, 0, 255);

	simShader.begin();
	simShader.setUniformTexture("tex0", gridFbo->getTexture(), 0);
	simShader.setUniform2f("tex0_size", gridFbo->getWidth(), gridFbo->getHeight());
	simShader.setUniformTexture("gradientFieldTex", paramsField.getTexture(), 2);
	simShader.setUniform1f("TIMESTEP", timestep);

	ofPushMatrix();
	ofTranslate(getWidth()/2, getHeight()/2);
	ofSetColor(255);
	plane.draw();
	ofPopMatrix();

	simShader.end();

	nextGridFbo->end();

	swapFbos();
}

void GrayScott::onTouchDown(ofxInterface::TouchEvent &event)
{
	if (bTouching) {
		return;
	}

	touchId = event.id;
	bTouching = true;
	ofVec2f local = toLocal(event.position);

	gridFbo->begin();
	ofSetColor(B_COLOR);
	ofFill();
	ofDrawCircle(local.x, local.y, 10);
	gridFbo->end();
}

void GrayScott::onTouchMove(ofxInterface::TouchEvent &event)
{
	if (touchId != event.id) {
		return;
	}

	ofVec2f local = toLocal(event.position);

	gridFbo->begin();
	ofSetColor(B_COLOR);
	ofFill();
	ofDrawCircle(local.x, local.y, 10);
	gridFbo->end();


//	move(event.position - event.prevPosition);
}

void GrayScott::onTouchUp(ofxInterface::TouchEvent &event)
{
	if (touchId != event.id) {
		return;
	}

	bTouching = false;
}

void GrayScott::drawRandomDiffusion(float x, float y)
{
	int xx=x;
	int yy=y;

	diffusionFlowFbo.begin();
	float rad = ofRandom(50);
	ofVec2f amount = ofVec2f(ofRandom(-0.2, 0.2),
					  ofRandom(-0.2, 0.2));
	ofFill();
	ofSetColor(ofFloatColor(0.5+amount.x, 0.5+amount.y, 0, 1));
	ofDrawCircle(x, y, rad);
	diffusionFlowFbo.end();
}

void GrayScott::drawDiffusionBump(float x, float y)
{
	ofVec2f p(x, y);

	int rad = (int)ofRandom(50, 60);

	diffusionFlowFbo.begin();
	for (int yy = -rad; yy<rad; yy++) {
		for (int xx = -rad; xx<rad; xx++) {
			ofVec2f vec = ofVec2f(xx, yy);
			if (vec.length() > rad) {
				continue;
			}

			vec /= rad;
			vec *= 0.2;
			ofSetColor(ofFloatColor(0.5f+vec.x, 0.5f+vec.y, 0, 1));
			ofDrawLine(x+xx, y+yy, x+xx, y+yy+1);
		}
	}
	diffusionFlowFbo.end();

}

void GrayScott::clearDiffusionMap()
{
	diffusionFlowFbo.begin();
	ofClear(ofFloatColor(0.5f));
	diffusionFlowFbo.end();
}

void GrayScott::onKeyPressed(ofKeyEventArgs &args)
{
	if (args.key == 'g') {
		if (paramsField.getVisible()) {
			paramsField.deactivate();
		}
		else {
			paramsField.activate();
		}
	}
}

