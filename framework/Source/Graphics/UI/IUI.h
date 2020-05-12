#pragma once
#include "../../pch.h"
#include "../../../Manager/GraphicsManager.h"
class IUI
{
public:

	IUI();
	virtual void Draw(Matrix& = Matrix()) {}
	virtual ~IUI();
};

class UIText : public IUI
{
public:
	string mText;
	string mNewText;
	float mSize;
	float mBoundary;
	float mPosX;
	float mPosY;
	Vector3 mColor;
	DXObject *mObject;
	UIText(float posX, float posY,float size, float boundary=0);
	~UIText();
	void SetText(string);
	void SetSize(float );
	void Draw(Matrix& =Matrix());
	

};
class UIHP : public IUI
{
public:
	double mRatio;
	float mSize;
	float mPosX;
	float mPosY;
	float mBoundaryX;
	float mBoundaryY;
	Vector3 mColor;
	DXObject *mObject;
	UIHP(float posX, float posY, float mBoundaryX, float mBoundaryY );
	~UIHP();
	void SetHP(float);
	void Draw(Matrix& = Matrix());
	Vector3 lowHealth;
	Vector3 hightHealth;
};
class UIButton: public IUI
{
public:
	float mSize;
	float mPosX;
	float mPosY;
	float mBoundaryX;
	float mBoundaryY;
	bool isSeleted = false;
	string mFileName;
	int Type = -1;
	DXObject *mObject;
	DXObject *mTextureObject;
	vector<DXTexture *> textureList;
	UIButton(float posX, float posY, float mBoundaryX, float mBoundaryY);
	void Draw(Matrix& = Matrix());
	void SeletedIO();
	void Active();
	void InActive();
	void Seleted(bool r);
	void setType(int r);
	~UIButton();
	int hitTime = -1;
	void SetTexture(DXTexture * rtexture);


};

class UITexture : public IUI
{
public:
	float mPosX;
	float mPosY;
	float mBoundaryX;
	float mBoundaryY;
	DXObject *mObject;
	bool alpha;
	UITexture(float posX, float posY, float mBoundaryX, float mBoundaryY);
	void HasAlpha(bool a);
	void Draw(Matrix& = Matrix());
	~UITexture();
	void SetTexture(DXTexture * rtexture);
};