#pragma once

#include "ofMain.h"

class Muscle {
	public:
		void setup();
		void update(float _c);
		void draw(ofVec3f _l);

	ofMesh muscle; //the head of the jellyfish

	float r; //radius of the head/body
	int nH; //number of horizontal nodes
	int nV; //number of vertical nodes
	float wf; //wave factor body

	vector<float> phi; //PHI angles
	vector<float> th; //TH angles
	vector<ofVec2f> crd; //storing the coordinates for the vertices; for example, to vertice[0] correspond phi[0] and th[0]; ver[1] -> ph[0], th[1];

	vector<ofVec3f>offsets;

};