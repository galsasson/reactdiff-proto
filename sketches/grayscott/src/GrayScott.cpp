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
	setSize(ofGetWindowWidth(), ofGetWindowHeight());

	// took these from: http://www.karlsims.com/rd.html
	params.setName("GrayScott");
	params.add(feedRate.set("feedRate", 0.0557f, 0, 0.1f));
	params.add(killRate.set("killRate", 0.062f, 0, 0.1f));
	params.add(aDiffRate.set("aDiffRate", 1.0f, 0.0f, 1.0f));
	params.add(bDiffRate.set("bDiffRate", 0.5f, 0.0f, 1.0f));
	params.add(simSteps.set("Steps", 1, 1, 40));
	params.add(timestep.set("timestep", 1.0f, 0.0f, 10.0f));
	params.add(minColor.set("minColor", 0.2f, 0.0f, 1.0f));
	params.add(maxColor.set("maxColor", 0.4f, 0.0f, 1.0f));


	allocateFbos(getWidth(), getHeight());
	seedGrid();


	simShader.load("shaders/grayscott_simulation");
	renderShader.load("shaders/grayscott_render");
//	plane.mapTexCoordsFromTexture(gridFbo->getTexture());
	plane.mapTexCoords(0, 0, 1, 1);
	plane.setWidth(getWidth());
	plane.setHeight(getHeight());

	ofAddListener(eventTouchDown, this, &GrayScott::onTouchDown);
	ofAddListener(eventTouchMove, this, &GrayScott::onTouchMove);
	ofAddListener(eventTouchUp, this, &GrayScott::onTouchUp);

	ofLogNotice("GrayScott") << "initialized!";
}

void GrayScott::setupGui(ofxPanel& panel)
{
	panel.setup(params);
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
	for (int i=0; i<simSteps; i++) {
		simulationStep(dt);
	}

}

void GrayScott::draw()
{
	ofSetColor(255);
	renderShader.begin();
	renderShader.setUniformTexture("tex0", gridFbo->getTexture(), 0);
	renderShader.setUniform2f("tex0_size", gridFbo->getWidth(), gridFbo->getHeight());
	renderShader.setUniform1f("minColor", minColor);
	renderShader.setUniform1f("maxColor", maxColor);

	ofPushMatrix();
	ofTranslate(getWidth()/2, getHeight()/2);
	ofSetColor(255);
	plane.draw();
	ofPopMatrix();
//	gridFbo->draw(0, 0);

	renderShader.end();
}


void GrayScott::allocateFbos(int w, int h)
{
	ofFbo::Settings s;
	s.width = w;
	s.height = h;
	s.internalformat = GL_RGBA;
	s.minFilter = GL_NEAREST;
	s.maxFilter = GL_NEAREST;
	s.wrapModeHorizontal = GL_REPEAT;
	s.wrapModeVertical = GL_REPEAT;

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
	simShader.setUniform2f("tex0_size", getWidth(), getHeight());
	simShader.setUniform1f("feedRate", feedRate);
	simShader.setUniform1f("killRate", killRate);
	simShader.setUniform1f("aDiffRate", aDiffRate);
	simShader.setUniform1f("bDiffRate", bDiffRate);
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
	ofFill();
	ofSetColor(B_COLOR);
	ofDrawRectangle((int)local.x, (int)local.y, 20, 10);
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

