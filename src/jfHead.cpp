#include "jfHead.h"

void JFHead::setup(){

	jfh.setMode(OF_PRIMITIVE_LINES);
	jfh2.setMode(OF_PRIMITIVE_LINES);
	jfh_low.setMode(OF_PRIMITIVE_LINES);
	jfhfill.setMode(OF_PRIMITIVE_LINES);

	//time=0;
	wf = 1.5;

	nH = 30;
	nV = 55;

	r = 700;
	r_low = 90;

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
		for (int j=0; j<nV; j++) {
			float x_temp = cos(phi[i])*sin(th[j])*r;
			float y_temp = sin(phi[i])*sin(th[j])*r;
			float z_temp = cos(th[j])*r;

			ofVec3f vert_temp(x_temp,y_temp,z_temp);
			ofVec2f crd_temp(i,j);

			crd.push_back(crd_temp);
			offsets.push_back(ofVec3f(ofRandom(0,100000), ofRandom(0,100000), ofRandom(0,100000)));
			jfh.addVertex(vert_temp);
			jfh2.addVertex(vert_temp); //adding exactly the same number of vertices to jfh2;

			jfhfill.addVertex(vert_temp); //the even numbers of the jfhfill vertices are the upper surface 0 2 4 etc
			jfhfill.addVertex(vert_temp); //the odd numbers of the jfhfill vertices are the lower surface 1 3 5 etc
		}
	}

	for (int i=0; i<nH; i++) {
		for (int j=0; j<2; j++) {

			float angle = i*(2*PI/nH);

			float x_temp = cos(angle)*r_low;
			float y_temp = sin(angle)*r_low;
			float z_temp = j*80;

		ofVec3f vert_temp(x_temp,y_temp,z_temp);

		jfh_low.addVertex(vert_temp);
		}
	}

	float connectionDistance = 120;

	int numVerts = jfh.getNumVertices();
	/*
	for (int a=0; a<numVerts; ++a) {
		ofVec3f verta = jfh.getVertex(a);
		for (int b=a+1; b<numVerts; ++b) {
			ofVec3f vertb = jfh.getVertex(b);
			float distance = verta.distance(vertb);
			if (distance <= connectionDistance) {
				jfh.addIndex(a);
				jfh.addIndex(b);
				jfh2.addIndex(a);
				jfh2.addIndex(b);
			}
		}
	}*/

	for (int a=0; a<numVerts; ++a) {
		for (int b=a+1; b<numVerts; ++b) {
			if ((fabs(crd[a].y - crd[b].y)<=1)&&((fabs(crd[a].x - crd[b].x)<=1)||(fabs(crd[a].x - crd[b].x)==nH-1))) {
				jfh.addIndex(a);
				jfh.addIndex(b);
				jfh2.addIndex(a);
				jfh2.addIndex(b);
			}
		}
	}
	
	int numVertsFill = jfhfill.getNumVertices();
	/*
	for (int a=0; a<numVertsFill; a=a+2) {
		for (int b=a+1; b<numVertsFill; b=b+2) {
			if (abs(a-b)<=5) {
				jfhfill.addIndex(a);
				jfhfill.addIndex(b);
			}
		}
	}*/

	for (int a=0; a<numVertsFill; a=a+2) {
		for (int b=a+1; b<numVertsFill; b=b+2) {
			if ((abs((a%nV)-(b%nV))<=3)&&((fabs(floor(a/nV)-floor(b/nV))==1)||(fabs(floor(a/nV)-floor(b/nV))==nH-1))) {
				jfhfill.addIndex(a);
				jfhfill.addIndex(b);
			}
		}
	}


}

//--------------------------------------------------------------
void JFHead::update(float _c){
	int numVerts = jfh.getNumVertices(); //the number of vertices

	float c = _c;

	for (int i=0; i<numVerts; ++i) {
		ofVec3f vert; // = jellyfishB.getVertex(i); //copy of the vertex
 
		float time = ofGetElapsedTimef();

		int ph_crd = crd[i].x;
		float ph_vert = phi[ph_crd];

		int th_crd = crd[i].y;
		float th_vert = th[th_crd];

		float zvar1 = 0.25;
		float zvar2 = 0.1; //pointiness, varying of length

		//Sine wave

		vert.x = (r + 0.15*r*cos(wf*th_vert + PI - c*time))*sin(th_vert)*cos(ph_vert)/(1 + 2*cos(PI/2-th_vert));
		vert.y = (r + 0.15*r*cos(wf*th_vert + PI - 1.3*c*time))*sin(th_vert)*sin(ph_vert)/(1 + 2*cos(PI/2-th_vert));
		vert.z = (zvar1*r + zvar2*r*cos(th_vert - c*time))*cos(th_vert);

		//some displacement for flavor!

		ofVec3f timeOffsets = offsets[i];
		float timeScale = 1.0;
		float displacementScale = ofMap(th_crd,0,nV,0,15);
 
		vert.x += (ofSignedNoise(time*timeScale + timeOffsets.x)) * displacementScale;
		vert.y += (ofSignedNoise(time*timeScale + timeOffsets.y)) * displacementScale;
		vert.z += (ofSignedNoise(time*timeScale + timeOffsets.z)) * displacementScale;

		jfh.setVertex(i, vert);
		jfhfill.setVertex(2*i, vert);
	}

	for (int i=0; i<numVerts; ++i) {
		ofVec3f vert; // = jellyfishB.getVertex(i); //copy of the vertex
 
		float time = ofGetElapsedTimef();

		int ph_crd = crd[i].x;
		float ph_vert = phi[ph_crd];

		int th_crd = crd[i].y;
		float th_vert = th[th_crd];

		float zvar1 = 0.12;
		float zvar2 = 0.03; //pointiness, varying of length

		//Sine wave

		vert.x = (r + 0.15*r*cos(wf*th_vert + PI - c*time))*sin(th_vert)*cos(ph_vert)/(1 + 2*cos(PI/2-th_vert));
		vert.y = (r + 0.15*r*cos(wf*th_vert + PI - 1.3*c*time))*sin(th_vert)*sin(ph_vert)/(1 + 2*cos(PI/2-th_vert));
		vert.z = (zvar1*r + zvar2*r*cos(th_vert - c*time))*cos(th_vert);

		//some displacement for flavor!

		ofVec3f timeOffsets = offsets[i];
		float timeScale = 1.0;
		float displacementScale = ofMap(th_crd,0,nV,0,15);
 
		vert.x += (ofSignedNoise(time*timeScale + timeOffsets.x)) * displacementScale;
		vert.y += (ofSignedNoise(time*timeScale + timeOffsets.y)) * displacementScale;
		vert.z += (ofSignedNoise(time*timeScale + timeOffsets.z)) * displacementScale;

		jfh2.setVertex(i, vert);
		jfhfill.setVertex(2*i+1, vert);
	}

	int numVertsLow = jfh_low.getNumVertices(); //the number of vertices
	/*
	for (int i=0; i<nH; i++) {

		int i_low = i*2;
		int i_jfh = i*nV + nV - 1;

		ofVec3f vert;
		ofVec3f vert_jfh = jfh.getVertex(i_jfh);

		vert.x = vert_jfh.x;
		vert.y = vert_jfh.y;
		vert.z = vert_jfh.z;

		jfh_low.setVertex(i_low, vert);

	}*/
}


//--------------------------------------------------------------
void JFHead::draw(){

        ofPushMatrix();
			
			ofTranslate(0,0,-100);
            jfh.draw();
			jfh2.draw();

			ofSetColor(0,255,255,7);
			jfhfill.draw();

        ofPopMatrix();
}