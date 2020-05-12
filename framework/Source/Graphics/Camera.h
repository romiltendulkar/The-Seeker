

#ifndef CAMERA_H
#define CAMERA_H
#include "../pch.h"
#include "../Util/Math.h"

class Camera
{
public:
	Camera();
	~Camera();

	/**
	Set Position
	*/
	void SetPosition(float x, float y, float z);

	/**
	Get Position
	*/
	void GetPosition(float&, float&, float&);
	Vector3 GetPosition();
	/**
	Set Movement
	*/
	void SetMovement(float x, float y, float z);

	/**
	Set Pan
	*/
	void SetPan(float);

	/**
	Set Tilt
	*/
	void SetTilt(float);

	/**
	Set Roll
	*/
	void SetRoll(float);

	/**
	Get Pan Tilt Roll
	*/
	void GetPanTiltRoll(float&, float&,float&);

	/**
	Get View Matrix
	*/
	void GetViewMatrix(Matrix&);

	/**
	Get 3rd Person View
	*/
	void Get3rdPersonView(Matrix &);
	void CameraShack(float duration, float magnitude);
	void CameraShackFrame(int duration,float magnitude);
	Vector3 GetCameraShackValue(float dt);
	Vector3 GetCameraShackFrameValue(int framenum);
	float mTx;
	float mTy;
	float mZoom;
	float mTotalTime=0.0f;
	int mTotalFrame=0;
	Matrix mRotateMatrix;
	bool isShack = false;
private:
	float mMagnitude;
	float mTotalTimebase = 0.0f;
	int mTotalFramebase = 0;
	Vector3 originalPosTH;
	Vector3 originalPosFR;
	/**
	Compute View Matrix
	*/
	void ComputeViewMatrix();

	/**
	Compute Rotate Matrix
	*/
	void ComputeRotateMatrix();

	/**
	Compute 3rdPerson View Matrix
	*/
	void Compute3rdPersonViewMatrix();
	float mPan, mTilt, mRoll;
	float mPosX, mPosY, mPosZ;
	Matrix mViewMatrix;
	Matrix m3PViewMatrix;
};
#endif 1

