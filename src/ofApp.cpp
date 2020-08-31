#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	isOpen.addListener(this, &ofApp::isOpenChanged);
	setDestIpButton.addListener(this, &ofApp::setDestIp);
	setKvalButton.addListener(this, &ofApp::setKval);
	getKvalButton.addListener(this, &ofApp::getKval);
	setSpeedButton.addListener(this, &ofApp::setSpeedProfile);
	getSpeedButton.addListener(this, &ofApp::getSpeedProfile);
	runButton.addListener(this, &ofApp::run);
	softHiZButton.addListener(this, &ofApp::softHiZ);
	goToButton.addListener(this, &ofApp::goTo);

	gui.setup();

	networkGroup.setName("network");
	networkGroup.add(ip.set("ip", "10.0.0.100"));
	networkGroup.add(outPort.set("out port", 50000, 0, 65535));
	networkGroup.add(inPort.set("in port", 50100, 0, 65535));
	networkGroup.add(isOpen.set("open", false));
	networkGroup.add(setDestIpButton.set("/setDestIp"));
	
	kvalGroup.setName("kval");
	kvalGroup.add(kvalTarget.set("target", 0, 0, 4));
	kvalGroup.add(kvalHold.set("hold", 16, 0, 255));
	kvalGroup.add(kvalRun.set("run", 16, 0, 255));
	kvalGroup.add(kvalAcc.set("acc", 16, 0, 255));
	kvalGroup.add(kvalDec.set("dec", 16, 0, 255));
	kvalGroup.add(setKvalButton.set("/setKval"));
	kvalGroup.add(getKvalButton.set("/getKval"));

	speedGroup.setName("speed");
	speedGroup.add(speedTarget.set("target", 0, 0, 4));
	speedGroup.add(speedAcc.set("acc step/s/s", 2000.0, 14.55, 59590.0));
	speedGroup.add(speedDec.set("dec step/s/s", 2000.0, 14.55, 59590.0));
	speedGroup.add(speedMax.set("max step/s", 620.0, 15.25, 15610.0));
	speedGroup.add(setSpeedButton.set("/setSpeedProfile"));
	speedGroup.add(getSpeedButton.set("/getSpeedProfile"));

	runGroup.setName("run");
	runGroup.add(runTarget.set("target", 0, 0, 4));
	runGroup.add(runSpeed.set("speed step/s", 620.0, -15610.0, 15610.0));
	runGroup.add(runButton.set("/run"));
	runGroup.add(softHiZButton.set("/softHiZ"));

	goToGroup.setName("goTo");
	goToGroup.add(goToTarget.set("target", 0, 0, 4));
	goToGroup.add(goToSteps.set("steps", 25600, -256000, 256000));
	goToGroup.add(goToButton.set("/goTo"));

	gui.add(networkGroup);
	gui.add(kvalGroup);
	gui.add(speedGroup);
	gui.add(runGroup);
	gui.add(goToGroup);

	gui.setPosition(ofGetWidth() - 220, 20);
}

//--------------------------------------------------------------
void ofApp::update(){
	while (receiver.hasWaitingMessages()) {

		ofxOscMessage m;
		receiver.getNextMessage(m);

		if (MESSAGE_LINE_MAX <= receiveStrings.size()) {
			receiveStrings.pop_back();
		}
		receiveStrings.insert(receiveStrings.begin(), getOscMsgAsString(m));
		
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	stringstream receiveStringStream;
	stringstream sendStringStream;
	receiveStringStream << "receive osc message" << endl << endl;
	sendStringStream << "send osc message" << endl << endl;

	for (string s : receiveStrings) {
		receiveStringStream << s << endl;
	}
	for (string s : sendStrings) {
		sendStringStream << s << endl;
	}
	ofDrawBitmapString(receiveStringStream.str().c_str(), 10, 20);
	ofDrawBitmapString(sendStringStream.str().c_str(), 10, ofGetHeight()/2+20);

	gui.draw();
}

void ofApp::exit() {
	receiver.stop();
}

void ofApp::isOpenChanged(bool &b) {
	if (b) {
		sender.setup(ip, outPort);
		receiver.setup(inPort);
		cout << "sender: " << ip << ", " << outPort << endl;
		cout << "receiver: " << outPort << endl;
	} else {
		receiver.stop();
	}
}


void ofApp::setDestIp() {
	ofxOscMessage m;
	m.setAddress("/setDestIp");
	sendOsc(m);
}
void ofApp::setKval() {
	ofxOscMessage m;
	m.setAddress("/setKval");
	m.addInt32Arg(getTarget(kvalTarget));
	m.addInt32Arg(kvalHold);
	m.addInt32Arg(kvalRun);
	m.addInt32Arg(kvalAcc);
	m.addInt32Arg(kvalDec);
	sendOsc(m);
}
void ofApp::getKval() {
	ofxOscMessage m;
	m.setAddress("/getKval");
	m.addInt32Arg(getTarget(kvalTarget));
	sendOsc(m);
}
void ofApp::setSpeedProfile() {
	ofxOscMessage m;
	m.setAddress("/setSpeedProfile");
	m.addInt32Arg(getTarget(speedTarget));
	m.addFloatArg(speedAcc);
	m.addFloatArg(speedDec);
	m.addFloatArg(speedMax);
	sendOsc(m);
}
void ofApp::getSpeedProfile() {
	ofxOscMessage m;
	m.setAddress("/getSpeedProfile");
	m.addInt32Arg(getTarget(speedTarget));
	sendOsc(m);
}
void ofApp::run() {
	ofxOscMessage m;
	m.setAddress("/run");
	m.addInt32Arg(getTarget(runTarget));
	m.addFloatArg(runSpeed);
	sendOsc(m);
}
void ofApp::softHiZ() {
	ofxOscMessage m;
	m.setAddress("/softHiZ");
	m.addInt32Arg(getTarget(runTarget));
	sendOsc(m);
}
void ofApp::goTo() {
	ofxOscMessage m;
	m.setAddress("/goTo");
	m.addInt32Arg(getTarget(goToTarget));
	m.addInt32Arg(goToSteps);
	sendOsc(m);
}

void ofApp::sendOsc(ofxOscMessage m) {
	if (MESSAGE_LINE_MAX <= sendStrings.size()) {
		sendStrings.pop_back();
	}
	sendStrings.insert(sendStrings.begin(), getOscMsgAsString(m));
	sender.sendMessage(m, false);
}

int ofApp::getTarget(int i) {
	if (i == 0) {
		return TARGET_ALL;
	} else {
		return i;
	}
}

string ofApp::getOscMsgAsString(ofxOscMessage m) {
	string msg_string;
	msg_string = m.getAddress();
	msg_string += ":";
	for (size_t i = 0; i < m.getNumArgs(); i++) {
		
		msg_string += " " + m.getArgTypeName(i);
		msg_string += ":";
		
		if (m.getArgType(i) == OFXOSC_TYPE_INT32) {
			msg_string += ofToString(m.getArgAsInt32(i));
		} else if (m.getArgType(i) == OFXOSC_TYPE_FLOAT) {
			msg_string += ofToString(m.getArgAsFloat(i));
		} else if (m.getArgType(i) == OFXOSC_TYPE_STRING) {
			msg_string += m.getArgAsString(i);
		} else if (m.getArgType(i) == OFXOSC_TYPE_TRUE) {
			msg_string += "true";
		} else if (m.getArgType(i) == OFXOSC_TYPE_FALSE) {
			msg_string += "false";
		} else {
			msg_string += "unhandled argument type " + m.getArgTypeName(i);
		}
	}
	return msg_string;
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
