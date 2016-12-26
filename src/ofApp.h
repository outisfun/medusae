#pragma once

#include "ofMain.h"

#include "jfHead.h"
#include "tentacle.h"
#include "tube.h"
#include "muscle.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		

	//variables for the jellyfish!!!

	ofMesh jellyfishB;

	float rB; //radius of the head/body


	int numTentacles; //number of horizontal nodes
	float rTentacles; //radius of tentacles together

	int nVB; //number of vertical nodes
	float wfB; //wave factor body

	vector<float> phiB; //PHI angles
	vector<float> thB; //TH angles
	vector<ofVec2f> crd; //storing the coordinates for the vertices; for example, to vertice[0] correspond phi[0] and th[0]; ver[1] -> ph[0], th[1];

	float c; //speed
	float t; //time
	
	Tube tube;
	JFHead head1;
	Muscle musc1;

	//Tentacle tube;

	float i,j,k;

	ofEasyCam easyCam;
	ofCamera cam;
	
	vector<Tentacle> tentacles;
	vector<Tube> tubes;

	int numOfTentacles;

	ofVec3f loc;
	ofVec3f vel;
	ofVec3f dir; 

	ofVec3f dirXZ;

	float xz;

	float PHI;
	float TH;

	vector<ofVec3f> directions;
	vector<ofVec3f> locations;
	vector<ofVec2f> angles;

	ofImage img;

	//int snapCounter;

};
