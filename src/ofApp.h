#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOsc.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();
	void exit();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

private:
	void isOpenChanged(bool &b);
	void setDestIp();
	void setKval();
	void getKval();
	void setSpeedProfile();
	void getSpeedProfile();
	void run();
	void softHiZ();
	void goTo();
	void sendOsc(ofxOscMessage m);
	int getTarget(int i);
	string getOscMsgAsString(ofxOscMessage m);

	ofxPanel gui;

	ofParameterGroup networkGroup;
	ofParameter<string> ip;
	ofParameter<int> outPort;
	ofParameter<int> inPort;
	ofParameter<bool> isOpen;
	ofParameter<void> setDestIpButton;

	ofParameterGroup kvalGroup;
	ofParameter<int> kvalTarget;
	ofParameter<int> kvalHold;
	ofParameter<int> kvalRun;
	ofParameter<int> kvalAcc;
	ofParameter<int> kvalDec;
	ofParameter<void> setKvalButton;
	ofParameter<void> getKvalButton;

	ofParameterGroup speedGroup;
	ofParameter<int> speedTarget;
	ofParameter<float> speedAcc;
	ofParameter<float> speedDec;
	ofParameter<float> speedMax;
	ofParameter<void> setSpeedButton;
	ofParameter<void> getSpeedButton;

	ofParameterGroup runGroup;
	ofParameter<int> runTarget;
	ofParameter<float> runSpeed;
	ofParameter<void> runButton;
	ofParameter<void> softHiZButton;

	ofParameterGroup goToGroup;
	ofParameter<int> goToTarget;
	ofParameter<int> goToSteps;
	ofParameter<void> goToButton;

	ofxOscSender sender;
	ofxOscReceiver receiver;
	

	vector<string> receiveStrings;
	vector<string> sendStrings;

	const int TARGET_ALL = 255;
	const int MESSAGE_LINE_MAX = 20;
};
