#ifndef OFXSIMPLEOPENNIH
#define OFXSIMPLEOPENNIH


#include "ofMain.h"
#include "ofxOpenNIContext.h"
#include "ofxImageGenerator.h"
#include "ofxUserGenerator.h"
#include "ofxOpenNIRecorder.h"

/** 
 * 
 * This class can be used as a simple interface to 
 * all of the other classes. This class is intented to
 * make starting up ofxOpenNI a bit easier.
 *
 */

class ofxSimpleOpenNI {
public:
	enum Visuals {
		 V_SKELETON	= (1 << 0)
		,V_DEPTH	= (1 << 1)
		,V_IMAGE	= (1 << 2)
		,V_MASKS	= (1 << 3)
		,V_ALL		= (V_SKELETON | V_DEPTH | V_IMAGE | V_MASKS)
	};
	
	ofxSimpleOpenNI();
	
	void initWithRecording(string recordedFile);
	void initWithXML();
	void update();
	void startRecording();
	void stopRecording();
	void toggleRecording(string filename = "KinectRecording.oni");
	void startRecordingWhenUserIsDetected();
	
	void draw(float x = 0, float y = 0, float width = 640, float height = 480); // draws all initialized video
	
	void drawImage(float x = 0, float y = 0, float width = 640, float height = 480);
	void drawDepth(float x = 650, float y = 0, float width = 320, float height = 240);
	void drawUsers(float x = 0, float y = 0, float width = 320, float height = 240); // TODO: add coordinates.
	void drawUserPixels(int userID = 0);
	void drawAllUserPixels();
	void drawActiveUserIDs();
	unsigned char* getPixelsInDepthRange(float near, float far);
	void drawPixelsInDepthRange(float near, float far);
	unsigned char* getAllUserPixels();
	void getAllMergedUserPixels(unsigned char* rgb, unsigned char* rgba); // do not use opengl to blend color and mask but just merge them in a loop.
	ofxOpenNIContext& getContextRef();
	ofxDepthGenerator& getDepthGenRef();
	ofxImageGenerator& getImageGenRef();
	ofxUserGenerator& getUserGenRef();

	void disableImage();
	void enableImage();
	void disableDepth();
	void enableDepth();
	void disableMasks();
	void enableMasks();
	void disableSkeleton();
	void enableSkeleton();

private:
	int enabled_visuals;
	bool initialized;
	bool start_recording_when_user_is_detected;
	bool is_recording;
	bool is_using_recording;
	string record_file_name;
	ofxOpenNIContext ni_context;
	ofxImageGenerator ni_image;
	ofxDepthGenerator ni_depth;
	ofxUserGenerator ni_user;
	ofxOpenNIRecorder ni_recorder;
	
	vector<ofImage> user_pixels;
	ofImage depth_range_img;
	int max_users;
	
};

inline void ofxSimpleOpenNI::disableImage() {
	enabled_visuals &= ~V_IMAGE;
}
inline void ofxSimpleOpenNI::enableImage() {
	enabled_visuals |= V_IMAGE;
}
inline void ofxSimpleOpenNI::disableSkeleton() {
	enabled_visuals &= ~V_SKELETON;
}
inline void ofxSimpleOpenNI::enableSkeleton() {
	enabled_visuals |= V_SKELETON;
}
inline void ofxSimpleOpenNI::disableMasks() {
	enabled_visuals &= ~V_MASKS;
}
inline void ofxSimpleOpenNI::enableMasks() {
	enabled_visuals |= V_MASKS;
}
inline void ofxSimpleOpenNI::disableDepth() {
	enabled_visuals &= ~V_DEPTH; 
}
inline void ofxSimpleOpenNI::enableDepth() {
	enabled_visuals |= V_DEPTH;
}

#endif
