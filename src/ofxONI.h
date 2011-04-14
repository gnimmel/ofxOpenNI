#pragma once

#include "ofMain.h"
#include "ofxOpenNIContext.h"
#include "ofxImageGenerator.h"
#include "ofxUserGenerator.h"

/** 
 * TODO: implement all features in this helper class.
 *
 * This class can be used as a simple interface to 
 * all of the other classes. This class is intented to
 * make starting up ofxOpenNI a bit easier.
 *
 */
class ofxONI {
public:
	ofxONI();
	void initWithRecording(string recorded_file);
	void initWithXML();
	void update();
	void draw(); // draws all initialized video
	void drawImage(float x = 0, float y = 0, float width = 640, float height = 480);
	void drawDepth(float x = 650, float y = 0, float width = 320, float height = 240);
	void drawUsers(); // TODO: add coordinates.
	
	ofxOpenNIContext& getContext();
	ofxDepthGenerator& getDepthGen();
	ofxImageGenerator& getImageGen();
	ofxUserGenerator& getUserGen();
	
	ofxOpenNIContext context;
	ofxImageGenerator image;
	ofxDepthGenerator depth;
	ofxUserGenerator user;
private:
	
};