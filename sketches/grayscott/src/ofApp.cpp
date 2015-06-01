#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofLogNotice("ofApp") << "app started";

	drawDebug = false;
	ofDisableBlendMode();
//	ofEnableAlphaBlending();
//	ofEnableBlendMode(OF_BLENDMODE_ALPHA);



	TouchManager::one().setup(&scene);
	scene.setSize(ofGetWindowWidth(), ofGetWindowHeight());

	grayScott = new GrayScott();
	scene.addChild(grayScott);

	grayScott->setupGui(gui);
	
//	grayScott.setScale(ofGetWindowWidth()/grayScott.getWidth(), ofGetWindowHeight()/grayScott.getHeight(), 1.0f);
}

//--------------------------------------------------------------
void ofApp::update(){
	scene.updateSubtreePostOrder(1.0f / 60.0f);
}

//--------------------------------------------------------------
void ofApp::draw(){
	scene.render();

	if (drawDebug) {
		scene.renderDebug();
	}

	ofSetColor(0);
	ofDrawBitmapString("Scale: " + ofToString(grayScott->getScale().x), 10, ofGetWindowHeight()-20);

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
