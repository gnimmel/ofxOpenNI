#include "ofxSimpleOpenNI.h"
#include <iostream>

ofxSimpleOpenNI::ofxSimpleOpenNI()
	:initialized(false) 
	,is_recording(false)
	,is_using_recording(false)
	,start_recording_when_user_is_detected(false)
	,max_users(10)
	,record_file_name("KinectRecording.oni")
	,enabled_visuals(V_ALL)
{	
	user_pixels.resize(max_users);
}

void ofxSimpleOpenNI::initWithRecording(string file) {
	if(initialized) {
		printf("Hey we're simple; we cannot be initialized twice.\n");
		return;
	}
	ni_context.setupUsingRecording(file);
	ni_context.toggleRegisterViewport();
	ni_image.setup(&ni_context);
	ni_depth.setup(&ni_context);
	ni_user.setup(&ni_context);
	ni_user.setUseMaskPixels(true);
	ni_user.setSmoothing(0.1f);
	is_using_recording = true;
}

void ofxSimpleOpenNI::initWithXML() {
	if(!ni_context.setupUsingXMLFile()) {
		return;
	}
	
	ni_context.toggleRegisterViewport();
	ni_user.setup(&ni_context);
	ni_user.setSmoothing(0.99f);
	ni_user.setUseMaskPixels(true);
	ni_image.setup(&ni_context);
	ni_depth.setup(&ni_context);
	ni_recorder.setup(&ni_context,ONI_STREAMING);
	initialized = true;
}

void ofxSimpleOpenNI::startRecording() {
	if(is_using_recording) {
		printf("Cannot record when we are already using a recording.\n");
		return;
	}
	if(is_recording) {
		printf("Already recording.\n");
		return;
	}
	if(!initialized) {
		printf("We're not initialized with initWithXML so we cannot start recording.\nTry to initialize using initWithXML()\n");
		return;
	}
	ni_recorder.startRecord(record_file_name);
	is_recording = true;
}

void ofxSimpleOpenNI::stopRecording() {
	is_recording = false;
	ni_recorder.stopRecord();
}


void ofxSimpleOpenNI::toggleRecording(string recordFile) {	
	record_file_name = recordFile;
	if(is_recording) {
		stopRecording();
	}
	else {
		startRecording();
	}
}

void ofxSimpleOpenNI::update() {
	ni_context.update();
	ni_image.update();
	ni_user.update();
	ni_depth.update();
	
	// auto start recording when a user is found.
	if(	start_recording_when_user_is_detected 
		&& !is_recording
		&& (ni_user.getNumberOfTrackedUsers() > 0)
	)
	{
		startRecording();
	}
}

void ofxSimpleOpenNI::draw(float x, float y, float width, float height) {
	if(!initialized && !is_using_recording) {
		ofDrawBitmapString("OpenNI not initialized. Make sure to copy the config and lib directory\nfrom the example to bin/data/openni.\n\nIn bin/data/openni you should have two subdirectories:\n- lib\n- config\n", 20,20);
	}
	
	glColor3f(1,1,1);
	
	if(enabled_visuals & V_IMAGE) {
		drawImage(x,y,width,height);
	}
	
	if(enabled_visuals & V_DEPTH) {
		drawDepth(width,0,320,240);
	}
	
	if(enabled_visuals & V_SKELETON) {
		drawUsers(0,0,width,height);
		drawActiveUserIDs();
	}
	
	if(enabled_visuals & V_MASKS) {
		drawAllUserPixels();
	}
	
	if(is_recording) {
		float s = (1.0 + sin(ofGetElapsedTimef() * 2.4)) * 0.5;
		s *=  255;
		ofSetColor(s, 255-s,0);
		ofCircle(ofGetWidth()-20, 20, 10);	
	}
}

void ofxSimpleOpenNI::drawImage(float x, float y, float width, float height) {
	ni_image.draw(x,y,width,height);
}

void ofxSimpleOpenNI::drawDepth(float x, float y, float width, float height) {
	ni_depth.draw(x,y,width,height);
}

void ofxSimpleOpenNI::drawUsers(float x, float y, float width, float height) { // @todo use position
	ni_user.draw();
}

// call this function -after- a call to ofxSimpleOpenNI::draw() if you only 
// want to see the the pixels of the user.
void ofxSimpleOpenNI::drawUserPixels(int userID) {
	if(userID > max_users) {
		printf("Trying to draw user pixels for user %d but the max num of users is %d\n", max_users, userID);
		return;
	}
	
	ofSetColor(255,255,255);	
	glPushAttrib(GL_ALL_ATTRIB_BITS);
		glEnable(GL_BLEND);
		glBlendFunc(GL_DST_COLOR, GL_ZERO);
		glColor3f(1,1,1);
		unsigned char* pixels = ni_user.getUserPixels(userID);
		user_pixels[userID].setFromPixels(pixels, ni_user.getWidth(), ni_user.getHeight(), OF_IMAGE_GRAYSCALE);
		user_pixels[userID].draw(0,0,640,480);	
		glDisable(GL_BLEND);
	glPopAttrib();
}


void ofxSimpleOpenNI::drawActiveUserIDs() {
	ni_user.drawActiveUserIDs();
}

void ofxSimpleOpenNI::drawPixelsInDepthRange(float near, float far) {
//	depth_range_img.setFromPixels(
}

unsigned char* ofxSimpleOpenNI::getPixelsInDepthRange(float near, float far) {
	return ni_depth.getDepthPixels(near, far);
}

void ofxSimpleOpenNI::startRecordingWhenUserIsDetected() {
	start_recording_when_user_is_detected = true;
}

// passing 0 to ofxUserGenerator::getUserPixels() will draw all user pixels
void ofxSimpleOpenNI::drawAllUserPixels() {
	drawUserPixels(0);
}

unsigned char* ofxSimpleOpenNI::getAllUserPixels() {
	return ni_user.getUserPixels(0);
}

// do not use opengl to blend color and mask but just merge them in a loop.
void ofxSimpleOpenNI::getAllMergedUserPixels(unsigned char* rgb, unsigned char* rgba) {
	unsigned char* mask = ni_user.getUserPixels(); // @todo when do we clean this?
	unsigned char* pixels = ni_image.getPixels(); // @todo when do we clean this
	int width = 640;
	int height = 480;
	
	for(int j = 0; j < height; ++j) {
		for(int i = 0; i < width; ++i) {
			int color_dx = (j*width*3)+(i*3);
			int mask_dx = (j*width)+i;
			int result_dx = (j*width*4)+(i*4);
			if(mask[mask_dx] == 0) {
				rgba[result_dx] = rgb[color_dx] = 0;
				rgba[result_dx+1] = rgb[color_dx+1] = 0;
				rgba[result_dx+2] = rgb[color_dx+2] = 0;
				rgba[result_dx+3] = 0;
			}
			else {
				rgba[result_dx] = rgb[color_dx] = pixels[color_dx];
				rgba[result_dx+1] = rgb[color_dx + 1] = pixels[color_dx + 1];
				rgba[result_dx+2] = rgb[color_dx + 2] = pixels[color_dx + 2];
				rgba[result_dx+3] = 255;	
			}
		}
	}
}
