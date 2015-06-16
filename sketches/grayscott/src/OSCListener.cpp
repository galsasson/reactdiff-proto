//
//  OSCListener.cpp
//  grayscott
//
//  Created by Gal Sasson on 6/16/15.
//
//

#include "OSCListener.h"

void OSCListener::setup()
{
	setName("OSC Listener");
	setSize(200, 200);

	oscReciever.setup(8000);
}

void OSCListener::processMessages()
{
	while (oscReciever.hasWaitingMessages()) {
		ofxOscMessage msg;
		oscReciever.getNextMessage(&msg);

		string remoteIp = msg.getRemoteIp();
		RemoteController* controller = getRemoteForIp(remoteIp);
		if (controller == NULL) {
			ofLogWarning("OSCListener") << "no available slots";
			continue;
		}

		if (!controller->bConnected) {
			ofLogNotice("OSCListener") << "setting up new controller: ip = "<<remoteIp<<", port = "<<msg.getRemotePort();
			controller->setHost(remoteIp, 9000);
		}

		controller->parseValues(msg);
	}

	updateStatus();
}

void OSCListener::draw()
{
	ofSetColor(0, 128);
	ofFill();
	ofDrawRectangle(0, 0, getWidth(), getHeight());
	ofSetColor(255);
	ofDrawBitmapString(status.str(), 10, 10);
}

vector<RemoteController*> OSCListener::getControllers()
{
	vector<RemoteController*> clers;
	clers.push_back(&remotes[0]);
	clers.push_back(&remotes[1]);
	clers.push_back(&remotes[2]);

	return clers;
}

RemoteController* OSCListener::getRemoteForIp(const string &ip)
{
	for (int i=0; i<3; i++) {
		if (remotes[i].bConnected) {
			if (remotes[i].ip == ip) {
				return &remotes[i];
			}
		}
		else  {
			return &remotes[i];
		}
	}

	return NULL;
}

void OSCListener::updateStatus()
{
	status.str("");

	status <<
	"Listening...\n"<<
	"Controllers:\n";

	for (int i=0; i<3; i++) {
		if (remotes[i].bConnected) {
			status << i << ": "<<remotes[i].ip<<endl<<
			"   0: "<<remotes[i].values[0]<<endl<<
			"   1: "<<remotes[i].values[1]<<endl<<
			"   2: "<<remotes[i].values[2]<<endl;
		}
	}
}