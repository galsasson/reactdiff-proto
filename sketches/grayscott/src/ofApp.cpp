#include "ofApp.h"
#include "ofxTimeMeasurements.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofLogNotice("ofApp") << "app started";

	controls <<
	"Keyboard Controls:\n" <<
	"0-9    - Load Preset\n" <<
	"space  - Random Seed\n" <<
	"c      - Empty Seed\n" <<
	"h      - Hide Controls\n" <<
	"+/-    - zoom in/out\n" <<
	"r      - Reload Preset\n" <<
	"s      - Save Preset\n" <<
	"g      - toggle gradient field";

	drawDebug = false;
	bShowControls = true;
	currentPreset = 0;
//	ofDisableBlendMode();
	ofSetFrameRate(60);
	ofEnableAlphaBlending();

	cam.setupPerspective();
	cam.setNearClip(0.0);
	cam.setFarClip(10000);


	TIME_SAMPLE_SET_FRAMERATE(ofGetTargetFrameRate());
//	TIME_SAMPLE_DISABLE();



	TouchManager::one().setup(&scene);
	scene.setSize(ofGetWindowWidth(), ofGetWindowHeight());

	grayScott = new GrayScott();
	scene.addChild(grayScott);

	grayScott->setupGui(gui);
	gui.add(scale.set("Scale", 1, 1, grayScott->getWidth()));


	loadPreset(0);
//	grayScott.setScale(ofGetWindowWidth()/grayScott.getWidth(), ofGetWindowHeight()/grayScott.getHeight(), 1.0f);
}

//--------------------------------------------------------------
void ofApp::update(){
	grayScott->setScale(scale);
	grayScott->setPosition(
						   (ofGetWindowWidth() - grayScott->getLocalWidth()) / 2 + translate.x,
						   (ofGetWindowHeight() - grayScott->getLocalHeight()) / 2 + translate.y);

	scene.updateSubtreePostOrder(1.0f / 60.0f);
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofClear(0);

	cam.begin();

	scene.render();

	if (drawDebug) {
		scene.renderDebug();
	}

	cam.end();

	if (bShowControls)
	{
		ofSetColor(255, 255, 255, 128);
		ofFill();
		ofDrawRectangle(0, ofGetWindowHeight()-50, ofGetWindowWidth(), 50);

		float y = ofGetWindowHeight()-35;
		ofSetColor(0);
		ofDrawBitmapString("Frame Rate: " + ofToString(ofGetFrameRate()), 5, y);
		y+=15;
		ofDrawBitmapString("Scale: " + ofToString(grayScott->getScale().x), 5, y);

		ofVec2f p(ofGetWindowWidth()-280, 10);
		ofSetColor(0);
		ofFill();
		ofDrawRectangle(p.x, p.y, 270, 200);
		ofSetColor(255);
		ofDrawBitmapString("Preset: "+ofToString(currentPreset), p.x+10, p.y+20);
		ofDrawBitmapString(controls.str(), p.x+10, p.y+40);

		gui.draw();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (key == 'd') {
		drawDebug = !drawDebug;
	}
	else if (key == '=') {
		scale *= 2;
	}
	else if (key == '-') {
		scale /= 2;
	}
	else if (key == OF_KEY_RIGHT) {
		translate.x -= 100;
	}
	else if (key == OF_KEY_LEFT) {
		translate.x += 100;
	}
	else if (key == OF_KEY_UP) {
		translate.y += 100;
	}
	else if (key == OF_KEY_DOWN) {
		translate.y -= 100;
	}
	else if (key == ' ') {
		grayScott->seedGrid();
		grayScott->clearDiffusionMap();
	}
	else if (key >= 48 &&
			 key <= 58) {
		setPreset(key-48);
		loadPreset(key-48);
	}
	else if (key == 'r') {
		loadPreset(currentPreset);
	}
	else if (key == 's') {
		savePreset(currentPreset);
	}
	else if (key == 'h') {
		bShowControls = !bShowControls;
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

void ofApp::setPreset(int i)
{
	currentPreset = i;
}

void ofApp::loadPreset(int i)
{
	gui.loadFromFile("presets/set"+ofToString(i)+".xml");
}

void ofApp::savePreset(int i)
{
	gui.saveToFile("presets/set"+ofToString(i)+".xml");
}
