//
//  RemoteController.cpp
//  grayscott
//
//  Created by Gal Sasson on 6/16/15.
//
//

#include "RemoteController.h"

int RemoteController::GlobalIndex = 0;

RemoteController::~RemoteController()
{
	setConnectedLED(0);
	setUserLabel("User ?");
}

RemoteController::RemoteController()
{
	bConnected = false;
	index = GlobalIndex++;
}

void RemoteController::setHost(const string& _ip, int _port)
{
	ip = _ip;
	port = _port;
	sender.setup(ip, port);
	bConnected = true;

	setConnectedLED(1);
	setUserLabel("User " + ofToString(index+1));
	setValues(values);
}

void RemoteController::setValues(float vals[MAX_VALS])
{
	for (int i=0; i<MAX_VALS; i++)
	{
		values[i] = vals[i];
		setValue(i, vals[i]);
	}
}

void RemoteController::setValue(int i, float val)
{
	if (!bConnected) {
		return;
	}

	ofxOscMessage msg;
	msg.setAddress("/reacdiff/fader/" + ofToString(i));
	msg.addFloatArg(val);
	sender.sendMessage(msg);
}

void RemoteController::parseValues(const ofxOscMessage &msg)
{
	if (msg.getRemoteIp() != ip) {
		ofLogWarning("RemoteController") << "not my ip (" << msg.getRemoteIp() << " != " << ip << ")";
	}

	string address = msg.getAddress();
	if (address.find("/reacdiff/fader/") != string::npos) {
		int valCharIndex = address.rfind('/')+1;
		string indexStr = address.substr(valCharIndex, address.length());
		int index = ofToInt(indexStr);
		if (index > MAX_VALS) {
			ofLogWarning("RemoteController") << "index > MAX_VALS ("<<index<<" > "<<MAX_VALS <<")";
			return;
		}

		float value = msg.getArgAsFloat(0);
		values[index] = value;

		ofNotifyEvent(eventParamChanged, *this, this);
	}
}

void RemoteController::setConnectedLED(float val)
{
	if (!bConnected) {
		return;
	}

	ofxOscMessage msg;
	msg.setAddress("/reacdiff/connected-led");
	msg.addFloatArg(val);
	sender.sendMessage(msg);
}

void RemoteController::setUserLabel(const string& str)
{
	if (!bConnected) {
		return;
	}

	ofxOscMessage msg;
	msg.setAddress("/reacdiff/user-label");
	msg.addStringArg(str);
	sender.sendMessage(msg);
}