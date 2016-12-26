#include "tube.h"

void Tube::setup(){

	jfh.setMode(OF_PRIMITIVE_LINES);

	//time=0;
	wf = 1.5;

	nH = 12;
	nV = 150;

	r = 570;
	r_low = 300;

	for (int i=0; i<nH; i++) {
		float phi_temp = i*(2*PI)/nH;
		//cout<<"phi temp = "<<phi_temp<<endl;
		phi.push_back(phi_temp);
	}

	for (int j=0; j<nV; j++) {
		float th_temp = (j*PI)/(2*nV);
		//cout<<"th temp = "<<th_temp<<endl;
		th.push_back(th_temp);
	}

	for (int i=0; i<nH; i++) {
		for (int j=12; j<nV; j++) {
			float x_temp = cos(phi[i])*sin(th[j])*r;
			float y_temp = sin(phi[i])*sin(th[j])*r;
			float z_temp = cos(th[j])*r;

		ofVec3f vert_temp(x_temp,y_temp,z_temp);
		ofVec2f crd_temp(i,j);

		crd.push_back(crd_temp);
		offsets.push_back(ofVec3f(ofRandom(0,100000), ofRandom(0,100000), ofRandom(0,100000)));
		jfh.addVertex(vert_temp);
		}
	}
	
	float connectionDistance = 30;
	int numVerts = jfh.getNumVertices();
	
	for (int a=0; a<numVerts; ++a) {
		ofVec3f verta = jfh.getVertex(a);
		for (int b=a+1; b<numVerts; ++b) {
			ofVec3f vertb = jfh.getVertex(b);
			float distance = verta.distance(vertb);
			if (distance <= connectionDistance) {
				jfh.addIndex(a);
				jfh.addIndex(b);
			}
		}
	}

	for (int a=0; a<numVerts; ++a) {
		for (int b=a+1; b<numVerts; ++b) {
			if ((fabs(crd[a].y - crd[b].y)==1)&&(fabs(crd[a].x - crd[b].x)==0)) {
				jfh.addIndex(a);
				jfh.addIndex(b);
			}
		}
	}

	for (int a=0; a<numVerts; ++a) {
		ofVec3f verta = jfh.getVertex(a);
		for (int b=a+1; b<numVerts; ++b) {
			ofVec3f vertb = jfh.getVertex(b);
			float distance = verta.distance(vertb);
			if ((crd[a].x==crd[b].x)&&distance<=360) {
				//jfh.addIndex(a);
				//jfh.addIndex(b);
			}
		}
	}

}

//--------------------------------------------------------------
void Tube::update(float _c){
	int numVerts = jfh.getNumVertices(); //the number of vertices

	float c = _c;

	for (int i=0; i<numVerts; ++i) {
		ofVec3f vert; // = jellyfishB.getVertex(i); //copy of the vertex
 
		float time = ofGetElapsedTimef();

		int ph_crd = crd[i].x;
		float ph_vert = phi[ph_crd];

		int th_crd = crd[i].y;
		float th_vert = th[th_crd];

		float zvar1 = 0.55;
		float zvar2 = 0.1; //pointiness, varying of length

		vert.x = (r + 0.15*r*cos(wf*th_vert + PI - c*time))*sin(th_vert)*cos(ph_vert)/(1 + 2*cos(PI/2-th_vert));
		vert.y = (r + 0.15*r*cos(wf*th_vert + PI - 1.3*c*time))*sin(th_vert)*sin(ph_vert)/(1 + 2*cos(PI/2-th_vert));
		vert.z = (zvar1*r + zvar2*r*cos(th_vert - c*time))*cos(th_vert);

		ofVec3f timeOffsets = offsets[i];

		float timeScale = 1.0;
		float displacementScale = ofMap(th_crd,0,nV,0,35);
		
 
		// A typical design pattern for using Perlin noise uses a couple parameters:
		// ofSignedNoise(time*timeScale+timeOffset)*displacementScale
		//     ofSignedNoise(time) gives us noise values that change smoothly over time
		//     ofSignedNoise(time*timeScale) allows us to control the smoothness of our noise (smaller timeScale, smoother values)
		//     ofSignedNoise(time+timeOffset) allows us to use the same Perlin noise function to control multiple things and have them look as if they are moving independently
		//     ofSignedNoise(time)*displacementScale allows us to change the bounds of the noise from [-1, 1] to whatever we want
		// Combine all of those parameters together, and you've got some nice control over your noise
 
		vert.x += (ofSignedNoise(time*timeScale + timeOffsets.x)) * displacementScale;
		vert.y += (ofSignedNoise(time*timeScale + timeOffsets.y)) * displacementScale;
		vert.z += (ofSignedNoise(time*timeScale + timeOffsets.z)) * displacementScale;

		jfh.setVertex(i, vert);
	}
}


//--------------------------------------------------------------
void Tube::draw(ofVec3f _loc,  ofVec2f _angles){
	
		ofVec3f loc = _loc;
		ofVec3f angles = _angles;

		
		float phi = angles.x;
		float th = angles.y;


        ofPushMatrix();
			
			ofTranslate(0,0,-270);
            jfh.draw();

			/*
				for (int i=0; i<numVerts; ++i) {
					ofVec3f vert = jellyfishB.getVertex(i); // = jellyfishB.getVertex(i); //copy of the vertex
					float vertThickness = ofMap(vert.z,0,600,5,1);
					ofNoFill();
					ofCircle(vert.x, vert.y, vert.z, vertThickness);
				} */

        ofPopMatrix();
}