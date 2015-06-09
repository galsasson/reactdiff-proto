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
	params.add(feedRate.set("feedRate", 0.055f, 0, 0.1f));
	params.add(killRate.set("killRate", 0.062f, 0, 0.1f));
	params.add(aDiffRate.set("aDiffRate", 1.0f, 0.0f, 1.0f));
	params.add(bDiffRate.set("bDiffRate", 0.5f, 0.0f, 1.0f));
	params.add(simSteps.set("Steps", 1, 1, 40));
	params.add(timestep.set("timestep", 1.0f, 0.0f, 10.0f));
	params.add(bRenderWithShader.set("render shader", false));
	params.add(minColor.set("minColor", 0.2f, 0.0f, 1.0f));
	params.add(maxColor.set("maxColor", 0.4f, 0.0f, 1.0f));
	params.add(borderWidth.set("borderWidth", 0.1f, 0.0f, 1.0f));
	params.add(borderSoftness.set("borderSoftness", 0.01f, 0.0f, 0.3f));
	params.add(bDiffusionMapMode.set("diffusion map", false));
	params.add(bUseGradientField.set("Use gradient field", false));


	allocateFbos(getWidth(), getHeight());
	seedGrid();
	clearDiffusionMap();

	simShader.load("shaders/grayscott_simulation");
	renderShader.load("shaders/grayscott_render");
//	plane.mapTexCoordsFromTexture(gridFbo->getTexture());
	plane.mapTexCoords(0, 0, 1, 1);
	plane.setWidth(getWidth());
	plane.setHeight(getHeight());

	ofAddListener(eventTouchDown, this, &GrayScott::onTouchDown);
	ofAddListener(eventTouchMove, this, &GrayScott::onTouchMove);
	ofAddListener(eventTouchUp, this, &GrayScott::onTouchUp);

	gradField.setup(getWidth(), getHeight());
	gradField.deactivate();
	addChild(&gradField);

	ofAddListener(ofEvents().keyPressed, this, &GrayScott::onKeyPressed);

	ofLogNotice("GrayScott") << "initialized!";
}

void GrayScott::setupGui(ofxPanel& panel)
{
	panel.setup(params);
	panel.add(gradField.params);
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
	gradField.update(dt);
	
	if (bDiffusionMapMode) {
		return;
	}

	if (!bTouching) {
		for (int i=0; i<simSteps; i++) {
			simulationStep(dt);
		}
	}
}

void GrayScott::draw()
{
	if (bDiffusionMapMode) {
		drawDiffusionMap();
	}
	else {
		drawSimulation();
	}
}

void GrayScott::drawSimulation()
{
	if (bRenderWithShader) {
		renderShader.begin();
		renderShader.setUniformTexture("tex0", gridFbo->getTexture(), 0);
		renderShader.setUniform2f("tex0_size", gridFbo->getWidth(), gridFbo->getHeight());
		renderShader.setUniform1f("minColor", minColor);
		renderShader.setUniform1f("maxColor", maxColor);
		renderShader.setUniform1f("borderWidth", borderWidth);
		renderShader.setUniform1f("softness", borderSoftness);

		ofPushMatrix();
		ofTranslate(getWidth()/2, getHeight()/2);
		ofSetColor(255);
		plane.draw();
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
	s.internalformat = GL_RGB32F;
	s.minFilter = GL_NEAREST;
	s.maxFilter = GL_NEAREST;
//	s.wrapModeHorizontal = GL_REPEAT;
//	s.wrapModeVertical = GL_REPEAT;

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
	simShader.setUniformTexture("diffusionFlowTex", diffusionFlowFbo.getTexture(), 1);
	simShader.setUniformTexture("gradientFieldTex", gradField.getTexture(), 2);
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

	if (bDiffusionMapMode) {
//		for (int i=0; i<200; i++) {
//			drawRandomDiffusion(ofRandom(0, diffusionFlowFbo.getWidth()), ofRandom(0, diffusionFlowFbo.getHeight()));
//		}
//		drawRandomDiffusion(local.x, local.y);
		drawDiffusionBump(local.x, local.y);
		return;
	}

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

	if (bDiffusionMapMode) {
		drawRandomDiffusion(local.x, local.y);
		return;
	}

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
		if (gradField.getVisible()) {
			gradField.deactivate();
		}
		else {
			gradField.activate();
		}
	}
}

