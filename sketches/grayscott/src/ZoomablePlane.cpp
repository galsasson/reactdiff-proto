//
//  ZoomablePlane.cpp
//  grayscott
//
//  Created by Gal Sasson on 6/14/15.
//
//

#include "ZoomablePlane.h"

void ZoomablePlane::setup(int _cols, int _rows)
{
	cols = _cols;
	rows = _rows;

	buildModel();
}

void ZoomablePlane::setupGui(ofxPanel &panel)
{
	params.setName("Skin Plane");
	params.add(radius.set("radius", rows/2, 0, rows));
	radius.addListener(this, &ZoomablePlane::onFloatParamChanged);
	params.add(height.set("height", 100, 0, 700));
	height.addListener(this, &ZoomablePlane::onFloatParamChanged);

	panel.add(params);
}

void ZoomablePlane::draw()
{
	vbo.drawElements(GL_TRIANGLES, rows*cols*6);
}

void ZoomablePlane::buildModel()
{
	vector<ofVec3f> verts;
	vector<ofVec2f> texcoords;
	vector<ofIndexType> indices;

	for (int y=0; y<rows; y++)
	{
		for (int x=0; x<cols; x++)
		{
			verts.push_back(ofVec3f(x, y, 0));
			texcoords.push_back(ofVec2f((float)x / cols, (float)y / rows));

			if (x == cols-1 ||
				y == rows-1) {
				continue;
			}

			indices.push_back(y*cols + x);
			indices.push_back(y*cols + x+1);
			indices.push_back((y+1)*cols + x);

			indices.push_back(y*cols + x+1);
			indices.push_back((y+1)*cols + x+1);
			indices.push_back((y+1)*cols + x);
		}
	}

	vbo.setVertexData(&verts[0], indices.size(), GL_DYNAMIC_DRAW);
	vbo.setTexCoordData(&texcoords[0], texcoords.size(), GL_STATIC_DRAW);
	vbo.setIndexData(&indices[0], indices.size(), GL_STATIC_DRAW);
}

void ZoomablePlane::setHeights()
{
	vector<ofVec3f> verts;

	ofVec2f center = ofVec2f(cols/2, rows/2) + ofVec2f(0.5, 0.5f);

	for (int y=0; y<rows; y++)
	{
		for (int x=0; x<cols; x++)
		{
			float distanceToCenter = ofVec2f(x, y).distance(center);
			float z = ofMap(distanceToCenter, 0, radius, height, 0, true);
			verts.push_back(ofVec3f(x, y, z));

		}
	}

	vbo.setVertexData(&verts[0], verts.size(), GL_DYNAMIC_DRAW);
}

void ZoomablePlane::onFloatParamChanged(float &param)
{
	setHeights();
}
