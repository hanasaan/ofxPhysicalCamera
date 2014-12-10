#include "ofMain.h"
#include "ofxPhysicalCamera.h"

class ofApp : public ofBaseApp{
	ofxPhysicalCamera cam;
public:
    void setup()
    {
        ofSetVerticalSync(true);
        ofSetFrameRate(60);
		
		ofxCv::Calibration calib;
		calib.load("calib.yml");
		cam.setCameraIntrinsics(calib.getUndistortedIntrinsics());
		cam.setGlobalPosition(ofVec3f(50, 200, 100));
		cam.lookAt(ofVec3f());
    }
	
    void draw()
    {
        ofClear(0);
		cam.beginWithCameraViewport();
		ofDrawGrid(100);
		ofDrawAxis(100);
		cam.end();
		
		ofDrawBitmapStringHighlight(ofToString(cam.getCameraFov()), 10, 20);
    }
};

//========================================================================
int main( ){
	ofSetupOpenGL(1920,1080,OF_WINDOW);			// <-------- setup the GL context
    
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());
    
}
