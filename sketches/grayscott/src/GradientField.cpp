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

	allocateFbo();
	createModel(vbo);
	setModelColors(vbo);

	bVboDirty = true;
	bFboDirty = true;

	renderShader.load("shaders/gradientfield_render");
	plane.mapTexCoords(0, 0, 1, 1);
	plane.setWidth(getWidth());
	plane.setHeight(getHeight());
}

ofTexture& GradientField::getTexture()
{
	return fbo.getTexture();
}

void GradientField::update(float dt)
{
	if (bVboDirty) {
		setModelColors(vbo);
		bVboDirty = false;
		bFboDirty = true;
	}

	if (bFboDirty) {
		ofDisableAlphaBlending();

		// render the fbo
		fbo.begin();
		ofFloatColor
		ofClear((ofFloatColor(background)));

		if (bEnableField) {
			ofSetColor(255);
			vbo.drawElements(GL_TRIANGLES, 6*3);
		}

		fbo.end();

		ofEnableAlphaBlending();

		bFboDirty = false;
	}
}

void GradientField::draw(float magR, float magG, float magB, float magA)
{
	ofPushMatrix();
	ofTranslate(getWidth()/2, getHeight()/2);

	// clear the background
	ofSetColor(0);
	plane.draw();

	// draw the fbo
	renderShader.begin();
	renderShader.setUniformTexture("tex0", fbo.getTexture(), 0);
	renderShader.setUniform4f("mag", magR, magG, magB, magA);
	ofSetColor(255);
	plane.draw();
	renderShader.end();

	ofPopMatrix();
}

void GradientField::allocateFbo()
{
	ofFbo::Settings s;
	s.width = getWidth();
	s.height = getHeight();
	s.internalformat = GL_RGBA32F;
	s.minFilter = GL_NEAREST;
	s.maxFilter = GL_NEAREST;

	fbo.allocate(s);
}

void GradientField::createModel(ofVbo& vbo)
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

void GradientField::setModelColors(ofVbo& vbo)
{
	vector<ofFloatColor> colors;

	colors.push_back(ofFloatColor((values[0].get() + values[2].get())/2));
	colors.push_back(ofFloatColor(values[0]));
	colors.push_back(ofFloatColor((values[0].get() + values[1].get())/2));
	colors.push_back(ofFloatColor(values[1]));
	colors.push_back(ofFloatColor((values[1].get() + values[2].get())/2));
	colors.push_back(ofFloatColor(values[2]));
	colors.push_back(ofFloatColor((values[0].get() + values[1].get() + values[2].get())/3));

	vbo.setColorData(&colors[0], colors.size(), GL_DYNAMIC_DRAW);
}

void GradientField::initParams(const string& name)
{
//	setDefaults();

//	panel.setup(params);
//	params.setName(name);
	params.add(bEnableField.set("Enable " + name, true));
//	bEnableField.addListener(this, &GradientField::onBoolParamChanged);

	params.add(background);
//	background.addListener(this, &GradientField::onVec4ParamChanged);

	for (int i=0; i<3; i++) {
		params.add(values[i]);
//		values[i].addListener(this, &GradientField::onVec4ParamChanged);
	}
}

void GradientField::setDefaults()
{
	/*
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
	 */
}

void GradientField::onFloatParamChanged(float &param)
{
	bVboDirty = true;
}

void GradientField::onBoolParamChanged(bool &params)
{
	bVboDirty = true;
}

void GradientField::onVec4ParamChanged(ofVec4f &params)
{
	bVboDirty = true;
}




