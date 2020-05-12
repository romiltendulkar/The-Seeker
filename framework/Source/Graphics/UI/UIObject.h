#pragma once
#include "../../pch.h"
#include "IUI.h"
#include "../DXObject.h"
class UIObject
{
public:
	UIObject(float sizeX, float sizeY, float TranslateX, float TranslateY, bool NeedDraw = false, bool IsRoot = false);
	~UIObject();
	
	std::vector<std::pair<UIObject*, Matrix>> instances;
	void Draw(Matrix& objectTr);

	void add(UIObject* m, Matrix tr = Matrix()) { instances.push_back(std::make_pair(m, tr)); }
	void SetSize(float,float);
	DXObject mOutLine;
	void SetTr(int,Matrix);

	Matrix mTr;
	Vector2 mSize;
	bool mNeedDraw = false;
	bool mOutLineDraw = true;
	bool mIsRoot = false;
	IUI *mContent;

};

