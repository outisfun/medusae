#pragma once

#include "ofMain.h"

class Tube {
	public:
		void setup();
		void update(float _c);
		void draw(ofVec3f _loc,  ofVec2f _angles);

	ofMesh jfh; //the head of the jellyfish

	float r; //radius of the head/body
	float r_low; //radius of the lower head

	int nH; //number of horizontal nodes
	int nV; //number of vertical nodes
	float wf; //wave factor body

	vector<float> phi; //PHI angles
	vector<float> th; //TH angles
	vector<ofVec2f> crd; //storing the coordinates for the vertices; for example, to vertice[0] correspond phi[0] and th[0]; ver[1] -> ph[0], th[1];

	vector<ofVec3f>offsets;

};