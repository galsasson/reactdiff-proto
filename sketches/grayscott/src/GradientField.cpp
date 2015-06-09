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
		// render the fbo
		fbo.begin();
		ofClear(ofFloatColor(defaultFeed, defaultKill, 0, 1));

		ofSetColor(255);
		vbo.drawElements(GL_TRIANGLES, 6*3);

		fbo.end();

		bFboDirty = false;
	}
}

void GradientField::draw()
{
	ofSetColor(255);
	fbo.draw(0, 0);
}

void GradientField::allocateFbo()
{
	ofFbo::Settings s;
	s.width = getWidth();
	s.height = getHeight();
	s.internalformat = GL_RGB32F;
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

	ofFloatColor user0Color(feedValues[0], killValues[0], 0, 1);
	ofFloatColor user1Color(feedValues[1], killValues[1], 0, 1);
	ofFloatColor user2Color(feedValues[2], killValues[2], 0, 1);

	colors.push_back((user0Color + user2Color) / 2);
	colors.push_back(user0Color);
	colors.push_back((user0Color + user1Color) / 2);
	colors.push_back(user1Color);
	colors.push_back((user1Color + user2Color) / 2);
	colors.push_back(user2Color);
	colors.push_back((user0Color + user1Color + user2Color) / 3);

	vbo.setColorData(&colors[0], colors.size(), GL_DYNAMIC_DRAW);
}

void GradientField::initParams()
{
	setDefaults();

	params.setName("Gradient Field");
	params.add(defaultFeed);
	defaultFeed.addListener(this, &GradientField::onParamChanged);
	params.add(defaultKill);
	defaultKill.addListener(this, &GradientField::onParamChanged);
	for (int i=0; i<3; i++) {
		params.add(feedValues[i]);
		feedValues[i].addListener(this, &GradientField::onParamChanged);
		params.add(killValues[i]);
		killValues[i].addListener(this, &GradientField::onParamChanged);
	}
}

void GradientField::setDefaults()
{
	defaultFeed.set("Default Feed", 0.055f, 0, 0.1);
	defaultKill.set("Default Kill", 0.062f, 0, 0.1);

	for (int i=0; i<3; i++) {
		feedValues[i].set("Feed "+ofToString(i), defaultFeed, 0, 0.1);
		killValues[i].set("Kill "+ofToString(i), defaultKill, 0, 0.1);
	}
}

void GradientField::onParamChanged(float &param)
{
	bModelColorsDirty = true;
}





