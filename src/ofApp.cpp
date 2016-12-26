#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofEnableSmoothing();
    //ofBackground(0);


	cam.setPosition(ofVec3f(900, 900, -200));

	cam.lookAt(ofVec3f(0,0,-200), ofVec3f(0,0,1));

	//snapCounter = 0;

	i=1;
	j=1;
	k=1;

	head1.setup();


	loc.set(100,-100,100);
	vel.set(0,1,0);

	//both arrays contain one element in the beginning
	locations.push_back(loc);

	for (int i=0; i<180; i++) {
		locations.push_back(ofVec3f(0,0,0));
	}
	

	directions.push_back(vel);

	for (int i=0; i<180; i++) {
		directions.push_back(ofVec3f(0,0,0));
	}


	//now locations contains 181 elements

	angles.push_back(ofVec2f(0,0));

	for (int i=0; i<180; i++) {
		angles.push_back(ofVec3f(0,0,0));
	}

	//and so does angles

	c=2;
	wfB = 20;
	//

	numTentacles = 8; //num of tentacles
	//nVB = 75;

	rTentacles = 45; //radius of tentacles altogether

	for (int i=0; i<numTentacles; i++) {
		float phi_temp = i*(2*PI)/numTentacles; //the angle at which each tentacle is located
		
		Tentacle tent;

		float wf_tent = ofRandom(8,25); //random wave factor for each tentacle

		float zv1 = ofRandom(35,45);
		float zv2 = ofRandom(8,10);
		float zv3 = ofRandom(4,6);

		tent.setup(phi_temp, rTentacles, wf_tent, zv1, zv2, zv3); //the setup accepts the location of the tentacle,, the radius of the circle on which they are located, as well as wave factor
			//tube.setup(phiB[i], rB, wf_tent);

		tentacles.push_back(tent);
			//tubes.push_back(tube);
	} 

	ofSetFrameRate(30);
}

//--------------------------------------------------------------
void ofApp::update(){

	float time = ofGetElapsedTimef();
	float timeScale = 0.1;
	float displacementScale = 10;
	
	
	vel.x = cos(time/5)*i;
	vel.y = sin(time/5)*j;
	vel.z = sin(time/5)*k; //t*k;

	vel.x += (ofSignedNoise(time*timeScale)) * displacementScale;
	vel.y += (ofSignedNoise(time*timeScale)) * displacementScale;
	vel.z += (ofSignedNoise(time*timeScale)) * (displacementScale/10);

	loc.x += vel.x/4;
	loc.y += vel.y/4;
	loc.z += vel.z/4;

	dir = vel;
	dir.normalize();

	//calculating PHI and TH angles of rotation
	dirXZ.set(dir.x, 0, dir.z);

	xz = sqrt(pow(dir.x,2) + pow(dir.z, 2));

	PHI = asin(dir.x/xz);

	if(dir.z<0) {
		PHI = (PHI/fabs(PHI))*(PI - fabs(PHI));
	}

	TH = -asin(dir.y);

	tube.update(c);
	head1.update(c);
	musc1.update(c);
	/*
	ofVec2f angles_temp(PHI,TH);

	for (int i=0; i<angles.size()-1; i++) {
		angles[i] = angles[i+1];
	}

	angles.erase(angles.end()); //erase last element
	angles.push_back(angles_temp);*/

	ofVec3f dir_temp;
	dir_temp = dir;
	
	for (int i=0; i<directions.size()-1; i++) {
		directions[i] = directions[i+1];
	}

	directions.erase(directions.begin());
	directions.push_back(dir_temp);

	ofVec3f loc_temp;
	loc_temp = loc;
	
	locations.erase(locations.begin());
	locations.push_back(loc_temp);

	for (int i=0; i<numTentacles; i++) {
		tentacles[i].update(c, locations, directions);
		//tubes[i].update(c);
	}

	
}


//--------------------------------------------------------------
void ofApp::draw(){
	
	ofColor centerColor = ofColor(35, 35, 35);
    ofColor edgeColor(0, 0, 0);
    ofBackgroundGradient(centerColor, edgeColor, OF_GRADIENT_CIRCULAR);

	
	cam.begin();

        ofPushMatrix();

		ofRotateY(ofRadToDeg(PHI));
		ofRotateX(ofRadToDeg(TH));

		ofTranslate(loc.x, loc.y, loc.z);

			ofEnableAlphaBlending();
	
			ofSetColor(0,255,255,30);

			for (int i=0; i<numTentacles; i++) {
				tentacles[i].draw();
			}
			
			ofTranslate(dir.x*250, dir.y*250, dir.z*250);

			ofSetColor(0,255,255,39);
			head1.draw();

			ofDisableAlphaBlending();
        ofPopMatrix();

	//ofSaveFrame();
    //easyCam.end();

//img.grabScreen(0,0,1024,768);
//img.saveImage("still-"+ofToString(snapCounter)+".png", OF_IMAGE_QUALITY_WORST);
//snapCounter++;

	cam.end();

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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
