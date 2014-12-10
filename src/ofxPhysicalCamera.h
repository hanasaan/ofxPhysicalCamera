#pragma once

#include "ofMain.h"
#include "ofxCv.h"

// some codes are from satoruhiga's ofxInteractivePrimitives
// https://github.com/satoruhiga/ofxInteractivePrimitives

class ofxPhysicalCamera : public ofCamera
{
public:
	void setCameraIntrinsics(const ofxCv::Intrinsics& undistortedIntrinsics) {
		imageSize = undistortedIntrinsics.getImageSize();
		ofRectangle vp(0, 0, imageSize.width, imageSize.height);
		const cv::Mat& camera_matrix = undistortedIntrinsics.getCameraMatrix();
		
		calcClipPlanes(vp);
		const float near_dist = getNearClip();
		const float far_dist = getFarClip();
		const float fx = camera_matrix.at<double>(0, 0);
		const float fy = camera_matrix.at<double>(1, 1);
		const float cx = camera_matrix.at<double>(0, 2);
		const float cy = camera_matrix.at<double>(1, 2);
		
		ofMatrix4x4 m;
		m.makeFrustumMatrix(near_dist * (-cx) / fx,
							near_dist * (vp.width - cx) / fx,
							near_dist * (cy - vp.height) / fy,
							near_dist * (cy) / fy,
							near_dist,
							far_dist);
		projection = m;
		focalLength = fy;
	}
	
	virtual void begin(ofRectangle viewport = ofGetCurrentViewport()) {
		if(!isActive) ofPushView();
		isActive = true;
		ofViewport(viewport.x,viewport.y,viewport.width,viewport.height);
		ofSetOrientation(ofGetOrientation(),vFlip);
		
		ofSetMatrixMode(OF_MATRIX_PROJECTION);
		ofLoadMatrix( projection );
		ofSetMatrixMode(OF_MATRIX_MODELVIEW);
		ofLoadMatrix( getModelViewMatrix() );
	}
	
	void beginWithCameraViewport(ofVec2f shiftXY = ofVec2f()) {
		ofRectangle rect(shiftXY.x,shiftXY.y,imageSize.width,imageSize.height);
		begin(rect);
	}
	
	ofMatrix4x4 getProjectionMatrix(ofRectangle viewport = ofGetCurrentViewport()) const {
		return projection;
	}
	
	ofMatrix4x4 getModelViewProjectionMatrix(ofRectangle viewport = ofGetCurrentViewport()) const {
		return getModelViewMatrix() * projection;
	}
	
	float getCameraFov() const {
		return ofRadToDeg(2.0f * atanf((imageSize.height * 0.5f) / focalLength));
	}
	
protected:
	void setFov(float f) {}
	void setLensOffset(const ofVec2f & lensOffset) {}
	void setAspectRatio(float aspectRatio) {}
	void setForceAspectRatio(bool forceAspectRatio) {}
	
	ofMatrix4x4 projection;
	float focalLength;
	cv::Size imageSize;
};
