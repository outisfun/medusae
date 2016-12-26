#pragma once

#include "ofMain.h"

class Tentacle {
	public:
		void setup(float _phi, float _rTentacles, float _wf, float _zvar1, float _zvar2, float _zvar3);
		void update(float _c, vector<ofVec3f> _locs, vector<ofVec3f> _dirs);
		void draw();

	ofMesh tmesh; //the tentacle tube
	ofMesh tred; 
	ofMesh tcom;

	int ti;
	int tj;

	float r; //radius of the tentacle tube
	
	//For each tentacle:

	float rTentacles; //radius of the large circle to which tentacles are attached
	float phi_tent; //angle of the tentacle
	float wf; //wave factor body

	float x_orig, y_orig, z_orig; //Origin point of each tentacle

	int nH; //number of horizontal nodes
	int nV; //number of vertical nodes

	float zvar1, zvar2, zvar3;
	

	vector<float> phi; //PHI angles
	vector<float> th; //TH angles

	vector<ofVec2f> crd; //storing the coordinates for the vertices; for example, to vertice[0] correspond phi[0] and th[0]; ver[1] -> ph[0], th[1];
	vector<ofVec2f> crdred;

	vector<Tentacle> tentacles;
	vector<ofVec3f> offsets;
	vector<ofVec3f> offsetsmesh;


};