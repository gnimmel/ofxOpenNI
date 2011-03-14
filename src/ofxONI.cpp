#include "ofxONI.h"
ofxONI::ofxONI() {
}

// start ofxOpenNI with a recorded oni file.
void ofxONI::initWithRecording(string recorded_file) {
	ofxOpenNIXML xml;
	xml.addLicense("PrimeSense","0KOIk2JeIBYClPWVnMoRKn5cdY4=");
	xml.setLogging(true,false,0);
	xml.addLoggingMask("ALL",true);
	//"roxlu_posing.oni"
	xml.addRecording(recorded_file);
	xml.addUserNode();
	
	context.setupUsingXMLObject(xml);
	
	depth.setup(&context);
	user.setup(&context, &depth);
	image.setup(&context);
}


void ofxONI::update() {
	context.update();
	user.update();
}

void ofxONI::draw() {
	drawImage();
	drawDepth();
	drawUsers();
}

void ofxONI::drawImage(float x, float y, float width, float height) {
	image.draw(x,y,width,height);
}

void ofxONI::drawDepth(float x, float y, float width, float height) {
	depth.draw(x,y,width,height);
}

// TODO: add coordinates.
void ofxONI::drawUsers() {
	user.draw();
}
	
