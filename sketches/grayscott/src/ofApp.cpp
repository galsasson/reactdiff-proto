#include "ofApp.h"
#include "ofxTimeMeasurements.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofLogNotice("ofApp") << "app started";

	drawDebug = false;
//	ofDisableBlendMode();
	ofSetFrameRate(60);
	ofEnableAlphaBlending();



	TIME_SAMPLE_SET_FRAMERATE(ofGetTargetFrameRate());
//	TIME_SAMPLE_DISABLE();



	TouchManager::one().setup(&scene);
	scene.setSize(ofGetWindowWidth(), ofGetWindowHeight());

	grayScott = new GrayScott();
	scene.addChild(grayScott);

	grayScott->setupGui(gui);
	gui.add(scale.set("Scale", 1, 0, 200));
	
//	grayScott.setScale(ofGetWindowWidth()/grayScott.getWidth(), ofGetWindowHeight()/grayScott.getHeight(), 1.0f);
}

//--------------------------------------------------------------
void ofApp::update(){
	grayScott->setScale(scale);
	scene.updateSubtreePostOrder(1.0f / 60.0f);
}

//--------------------------------------------------------------
void ofApp::draw(){
	scene.render();

	if (drawDebug) {
		scene.renderDebug();
	}

	ofSetColor(255, 255, 255, 128);
	ofFill();
	ofDrawRectangle(0, ofGetWindowHeight()-50, ofGetWindowWidth(), 50);

	float y = ofGetWindowHeight()-35;
	ofSetColor(0);
	ofDrawBitmapString("Frame Rate: " + ofToString(ofGetFrameRate()), 5, y);
	y+=15;
	ofDrawBitmapString("Scale: " + ofToString(grayScott->getScale().x), 5, y);

	gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (key == 'd') {
		drawDebug = !drawDebug;
	}
	else if (key == '=') {
		grayScott->setScale(grayScott->getScale()+0.1f);
	}
	else if (key == '-') {
		grayScott->setScale(grayScott->getScale()-0.1f);
	}
	else if (key == OF_KEY_RIGHT) {
		grayScott->move(-100, 0, 0);
	}
	else if (key == OF_KEY_LEFT) {
		grayScott->move(100, 0, 0);
	}
	else if (key == OF_KEY_UP) {
		grayScott->move(0, 100, 0);
	}
	else if (key == OF_KEY_DOWN) {
		grayScott->move(0, -100, 0);
	}
	else if (key == ' ') {
		grayScott->seedGrid();
	}
	else if (key == '0') {
		grayScott->setScale(1);
	}
	else if (key == '1') {
		grayScott->setScale(ofGetWindowWidth()/256);
	}
	else if (key == '2') {
		grayScott->setScale(ofGetWindowWidth()/128);
	}
	else if (key == '3') {
		grayScott->setScale(ofGetWindowWidth()/32);
	}
	else if (key == '4') {
		grayScott->setScale(ofGetWindowWidth()/8);
	}
	else if (key == '5') {
		grayScott->setScale(ofGetWindowWidth());
	}

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	ofxInterface::TouchManager::one().touchMove(button, ofVec2f(x, y));
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	ofxInterface::TouchManager::one().touchDown(button, ofVec2f(x, y));
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	ofxInterface::TouchManager::one().touchUp(button, ofVec2f(x, y));
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
