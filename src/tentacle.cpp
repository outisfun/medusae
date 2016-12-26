#include "tentacle.h"

void Tentacle::setup(float _phi, float _rTentacles, float _wf, float _zvar1, float _zvar2, float _zvar3){

	//The constructor accepts phi: angle of the tentacle in relationship to the drawing's centre; rTentacles - radius of the circle on which the tentacles are located; and wf - random wave factor for each

	phi_tent = _phi;
	rTentacles = _rTentacles;
	wf = _wf;

	zvar1 = _zvar1;
	zvar2 = _zvar2;
	zvar3 = _zvar3;

	nV = 180; //number of vertical nodes for each tentacle

	r = 20; //radius of the tentacle itself
	
	tmesh.setMode(OF_PRIMITIVE_POINTS);
	tred.setMode(OF_PRIMITIVE_LINES);
	tcom.setMode(OF_PRIMITIVE_LINES);

	//Determining the origin point in relation to the whole

		x_orig = cos(phi_tent)*rTentacles;
		y_orig = sin(phi_tent)*rTentacles;
		z_orig = 0;

	//The location of all tentacles in largeR is calculated; not really necessary: just submit the phi and largeR

	for (int j=0; j<nV; j++) {
		float th_temp = (j*PI)/(2*nV);
		//cout<<"th temp tentacle here= "<<th_temp<<endl;
		th.push_back(th_temp);
	}

	//Constructing the skeleton of the tentacle

	for (int j=0; j<nV; j++) {

		float x_temp = cos(phi_tent)*sin(th[j])*rTentacles;
		float y_temp = sin(phi_tent)*sin(th[j])*rTentacles;
		float z_temp = cos(th[j])*rTentacles;

		//origin of point determined, as well as its coordinates in angles
		ofVec2f crd_temp(phi_tent,j);
		ofVec3f vert_temp(x_temp,y_temp,z_temp);

		tmesh.addVertex(vert_temp);
		tcom.addVertex(vert_temp);

		crd.push_back(crd_temp);

		offsetsmesh.push_back(ofVec3f(ofRandom(0,100*x_temp), ofRandom(0,100*y_temp), ofRandom(0,100000)));

		//cout<<vert_temp.x<<" "<<vert_temp.y<<" "<<vert_temp.z<<endl;
		ofPushMatrix();
		ofTranslate(x_temp,y_temp,z_temp); //move to the center of the tentacle section

		//Constructing the actual tentacle
			
		float ang = ofRandom(-30,30);
		float rr = ofMap(j,0,nV,1,3);

				ofPushMatrix();
				ofRotateZ(ang);
				ofVec3f red1(rr,0,0);
				ofVec3f red2(-rr,0,0);
				tcom.addVertex(red1);
				tcom.addVertex(red2);
				//ofVec3f red3(0,rr,0);
				//ofVec3f red4(0,-rr,0);
				tred.addVertex(red1);
				tred.addVertex(red2);
				
				ofVec2f crd_red1(ang,j);
				crdred.push_back(crd_red1);
				ofPopMatrix();

		ofPopMatrix();
		
		
	}

	//Determining the lines of the tentacle mesh
	
	int numVertsRed = tred.getNumVertices();
	for (int a=0; a<numVertsRed; ++a) {
		int ac = floor(a/2);
		for (int b=a+1; b<numVertsRed; ++b) {
			int bc = floor(b/2);
			if (fabs(crdred[ac].y - crdred[bc].y)<=1) {
				tred.addIndex(a);
				tred.addIndex(b);
			}
		}
	}

	int numVertsCom = tcom.getNumVertices();
	for (int a=0; a<numVertsCom; a=a+1) {
		int ac = floor(a/3);
		for (int b=a+1; b<numVertsCom; ++b) {
			int bc = floor(b/3);
			if (abs(ac-bc)==1) {
				tcom.addIndex(a);
				tcom.addIndex(b);
			}
		}
	}

}

//--------------------------------------------------------------
void Tentacle::update(float _c, vector<ofVec3f> _locs, vector<ofVec3f> _dirs){

	//Tmesh is the skeleton of the tentacle; to it, forces are added, and then tred is drawn as the lattice

	int numVerts = tmesh.getNumVertices(); //the number of vertices
	int numRed = tred.getNumVertices(); //the number of vertices

	//locs and dirs are arrays containing the positions and directions for the medusa's trajectory
	float c = _c;
	vector<ofVec3f> locs = _locs;
	vector<ofVec3f> dirs = _dirs;
	
	float con = 2;

	for (int i=0; i<numVerts; i++) {
		ofVec3f vert; // = jellyfishB.getVertex(i); //copy of the vertex
 
		ofVec3f dir = dirs[i];
		ofVec3f loc = locs[i];

		float time = ofGetElapsedTimef();

		float ph_vert = crd[i].x;

		int th_crd = crd[i].y;
		float th_vert = th[th_crd];

		//float zvar1 = 40; //length
		//float zvar2 = 9; //pointiness, varying of length
		float res_zvar3 = ofMap(i,0,numVerts,0.2,zvar3); //wave amplitude

		vert.x = (r + res_zvar3*r*cos(wf*th_vert + PI - c*time))*sin(th_vert)*cos(ph_vert)/(1 + 2*cos(PI/2-th_vert));
		vert.y = (r + res_zvar3*r*cos(wf*th_vert + PI - c*time))*sin(th_vert)*sin(ph_vert)/(1 + 2*cos(PI/2-th_vert));
		vert.z = -(zvar1*r + zvar2*r*cos(th_vert - c*time))*cos(th_vert);

		//making the tentacles follow the medusa's trajectory

		float reli = ofMap(i, 0, numVerts, 0.4, 1);

		vert.x += (dir.x*250)*reli;
		vert.y += (dir.y*250)*reli;
		vert.z += (dir.z*250)*reli;

		//some displacement for flavor
		
		ofVec3f timeOffsets = offsetsmesh[i];
		float timeScale = ofMap(i,0,nV,1,0.1);
		float displacementScale = ofMap(i,0,nV,15,0);

		vert.x += (ofSignedNoise(time*timeScale + timeOffsets.x)) * displacementScale;
		vert.y += (ofSignedNoise(time*timeScale + timeOffsets.y)) * displacementScale;
		vert.z += (ofSignedNoise(time*timeScale + timeOffsets.z)) * displacementScale;


		tmesh.setVertex(i, vert);
	}

	//Updating the lattice (tred) according to the skeleton

	for (int i=0; i<numVerts; i++) { //REDS
		
		int c1 = 2*i;
		int c2 = 2*i+1;

		int com = 3*i;

		float ang = crdred[i].x;
		float j = crdred[i].y;

		float rr = ofMap(j,0,nV,1,18);

		ofVec3f vert_old = tmesh.getVertex(j);

		float x_red1 = vert_old.x + (cos(ang)*rr);
		float x_red2 = vert_old.x - (cos(ang)*rr);
		float y_red1 = vert_old.y + (sin(ang)*rr);
		float y_red2 = vert_old.y - (sin(ang)*rr);
		//float y_red = vert_old.y;
		float z_red = vert_old.z;

		ofVec3f vert1(x_red1, y_red1, z_red);
		ofVec3f vert2(x_red2, y_red2, z_red);

		tred.setVertex(c1, vert1);
		tred.setVertex(c2, vert2);

		tcom.setVertex(com, vert_old);
		tcom.setVertex(com+1, vert1);
		tcom.setVertex(com+2, vert2);

	}


}

//--------------------------------------------------------------
void Tentacle::draw(){

        ofPushMatrix();

            ofTranslate(x_orig, y_orig, z_orig);

			//tred.draw();
			//ofSetColor(255,0,0,15);
			tcom.draw();

        ofPopMatrix();
}