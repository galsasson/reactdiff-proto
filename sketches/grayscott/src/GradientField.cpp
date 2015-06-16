//
//  GradientField.cpp
//  grayscott
//
//  Created by Gal Sasson on 6/9/15.
//
//

#include "GradientField.h"

void GradientField::setup(float w, float h)
{
	setSize(w, h);

	initParams();
	allocateFbo();
	createModel();
	setModelColors();

	renderShader.load("shaders/gradientfield_render");
	plane.mapTexCoords(0, 0, 1, 1);
	plane.setWidth(getWidth());
	plane.setHeight(getHeight());

	bModelColorsDirty = true;
	bFboDirty = true;
}

ofTexture& GradientField::getTexture()
{
	return fbo.getTexture();
}

void GradientField::update(float dt)
{
	if (bModelColorsDirty) {
		setModelColors();
		bModelColorsDirty = false;

		bFboDirty = true;
	}

	if (bFboDirty) {
		ofDisableAlphaBlending();

		// render the fbo
		fbo.begin();
		ofClear(ofFloatColor(defaultFeed, defaultKill, defaultADiffRate, 1.0));//defaultBDiffRate));

		if (bUseGradientField) {
			ofSetColor(255);
			vbo.drawElements(GL_TRIANGLES, 6*3);
		}

		fbo.end();

		ofEnableAlphaBlending();

		bFboDirty = false;
	}
}

void GradientField::draw()
{
	ofPushMatrix();
	ofTranslate(getWidth()/2, getHeight()/2);

	// clear the background
	ofSetColor(0);
	plane.draw();

	// draw the fbo
	renderShader.begin();
	renderShader.setUniformTexture("tex0", fbo.getTexture(), 0);
	ofSetColor(255);
	plane.draw();
	renderShader.end();

	ofPopMatrix();
}

void GradientField::setRemoteControllers(vector<RemoteController *> _controllers)
{
	controllers = _controllers;
	for (int i=0; i<controllers.size(); i++) {
		ofAddListener(controllers[i]->eventParamChanged, this, &GradientField::onRemoteParamChanged);
	}
}

void GradientField::allocateFbo()
{
	ofFbo::Settings s;
	s.width = getWidth();
	s.height = getHeight();
	s.internalformat = GL_RGBA32F;
	s.minFilter = GL_NEAREST;
	s.maxFilter = GL_NEAREST;
	s.wrapModeHorizontal = GL_REPEAT;
	s.wrapModeVertical = GL_REPEAT;

	fbo.allocate(s);
}

void GradientField::createModel()
{
	int N=6;

	vector<ofVec3f> verts;
	vector<ofIndexType> indices;

	ofVec2f center(getWidth()/2, getHeight()/2);

	// add perimeter vertices
	float rad = getHeight()/2;
	float step = (float)360/N;
	ofVec2f vec(rad, 0);
	for (int i=0; i<N; i++) {
		verts.push_back(center+vec);
		vec.rotate(step);
	}

	// add center vertex
	verts.push_back(center);

	for (int i=0; i<N; i++) {
		indices.push_back(i);
		indices.push_back(N);
		indices.push_back((i+1)%N);
	}

	vbo.setVertexData(&verts[0], verts.size(), GL_STATIC_DRAW);
	vbo.setIndexData(&indices[0], indices.size(), GL_STATIC_DRAW);
}

void GradientField::setModelColors()
{
	vector<ofFloatColor> colors;

	ofFloatColor user0Color(feedValues[0], killValues[0], aDiffValues[0], 1);
	ofFloatColor user1Color(feedValues[1], killValues[1], aDiffValues[1], 1);
	ofFloatColor user2Color(feedValues[2], killValues[2], aDiffValues[2], 1);

	colors.push_back((user0Color/2 + user2Color/2));
	colors.push_back(user0Color);
	colors.push_back((user0Color/2 + user1Color/2));
	colors.push_back(user1Color);
	colors.push_back((user1Color/2 + user2Color/2));
	colors.push_back(user2Color);
	colors.push_back((user0Color/3 + user1Color/3 + user2Color/3));

	vbo.setColorData(&colors[0], colors.size(), GL_DYNAMIC_DRAW);
}

void GradientField::initParams()
{
	setDefaults();



	params.setName("Gradient Field");
	params.add(bUseGradientField.set("Use gradient field", true));
	bUseGradientField.addListener(this, &GradientField::onBoolParamChanged);

	params.add(defaultFeed);
	defaultFeed.addListener(this, &GradientField::onFloatParamChanged);
	params.add(defaultKill);
	defaultKill.addListener(this, &GradientField::onFloatParamChanged);
	params.add(defaultADiffRate);
	defaultADiffRate.addListener(this, &GradientField::onFloatParamChanged);
//	params.add(defaultBDiffRate);
//	defaultBDiffRate.addListener(this, &GradientField::onParamChanged);
	for (int i=0; i<3; i++) {
		params.add(feedValues[i]);
		feedValues[i].addListener(this, &GradientField::onFloatParamChanged);
		params.add(killValues[i]);
		killValues[i].addListener(this, &GradientField::onFloatParamChanged);
		params.add(aDiffValues[i]);
		aDiffValues[i].addListener(this, &GradientField::onFloatParamChanged);
//		params.add(bDiffValues[i]);
//		bDiffValues[i].addListener(this, &GradientField::onParamChanged);
	}
}

void GradientField::setDefaults()
{
	defaultFeed.set("Default Feed", 0.055f, 0, 0.1);
	defaultKill.set("Default Kill", 0.062f, 0, 0.1);
	defaultADiffRate.set("Default Diffusion", 0.1f, 0, 1);
//	defaultBDiffRate.set("Default B Diff", 0.5f, 0, 5);

	for (int i=0; i<3; i++) {
		feedValues[i].set("Feed "+ofToString(i+1), defaultFeed, 0, 0.1);
		killValues[i].set("Kill "+ofToString(i+1), defaultKill, 0, 0.1);
		aDiffValues[i].set("Diffusion "+ofToString(i+1), defaultADiffRate, 0, 1);
//		bDiffValues[i].set("B Diff "+ofToString(i), defaultBDiffRate, 0, 5);
	}
}

void GradientField::onFloatParamChanged(float &param)
{
	bModelColorsDirty = true;
	updateRemoteControllers();
}

void GradientField::onBoolParamChanged(bool &params)
{
	bModelColorsDirty = true;
}

void GradientField::updateRemoteControllers()
{
	for (int i=0; i<controllers.size(); i++) {
		float vals[3];
		ofParameter<float> v1 = params.get<float>("Feed_" + ofToString(i+1));
		vals[0] = v1;
		ofParameter<float> v2 = params.get<float>("Kill_" + ofToString(i+1));
		vals[1] = v2;
		ofParameter<float> v3 = params.get<float>("Diffusion_" + ofToString(i+1));
		vals[2] = v3;
		controllers[i]->setValues(vals);
	}
}

void GradientField::onRemoteParamChanged(RemoteController &controller)
{
	int index = controller.index;
	feedValues[index] = controller.values[0];
	killValues[index] = controller.values[1];
	aDiffValues[index] = controller.values[2];
	bModelColorsDirty = true;
}




