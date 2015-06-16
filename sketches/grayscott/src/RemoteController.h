//
//  RemoteController.h
//  grayscott
//
//  Created by Gal Sasson on 6/16/15.
//
//

#ifndef __grayscott__RemoteController__
#define __grayscott__RemoteController__

#include <stdio.h>
#include "ofMain.h"
#include "ofxOsc.h"

#define MAX_VALS 3

class RemoteController
{
public:
	static int GlobalIndex;

	~RemoteController();
	RemoteController();

	void setHost(const string& _ip, int _port);

	void setValues(float vals[MAX_VALS]);
	void setValue(int i, float val);
	void parseValues(const ofxOscMessage& msg);

	bool bConnected;
	int index;
	string ip;
	int port;
	float values[MAX_VALS];

	ofEvent<RemoteController> eventParamChanged;

private:
	ofxOscSender sender;

	void setConnectedLED(float val);
	void setUserLabel(const string& str);
};



#endif /* defined(__grayscott__RemoteController__) */
