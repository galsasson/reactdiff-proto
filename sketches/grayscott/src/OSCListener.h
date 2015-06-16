//
//  OSCListener.h
//  grayscott
//
//  Created by Gal Sasson on 6/16/15.
//
//

#ifndef __grayscott__OSCListener__
#define __grayscott__OSCListener__

#include <stdio.h>
#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxInterface.h"

#include "RemoteController.h"

using namespace ofxInterface;

class OSCListener : public ofxInterface::Node
{
public:

	void setup();

	void processMessages();
	void draw();

	vector<RemoteController*> getControllers();

private:
	RemoteController remotes[3];
	ofxOscReceiver oscReciever;

	RemoteController* getRemoteForIp(const string& ip);

	stringstream status;
	void updateStatus();
};


#endif /* defined(__grayscott__OSCListener__) */
