#include "../../pch.h"
#include "UIObject.h"
using namespace std;

UIObject::UIObject(float sizeX, float sizeY, float TranslateX, float TranslateY, bool needDraw, bool IsRoot)
	:mOutLine(new Quad(), GraphicsManager::GetInstance()->GetDirectXHandler()->GetDevice(),
		GraphicsManager::GetInstance()->GetDirectXHandler()->GetDeviceContext(),true)
{
	mSize = Vector2(sizeX, sizeY);
	mTr = MyTranslate(TranslateX, 0, TranslateY);
	mOutLine.SetMaterialID(5);
	mOutLine.SetDiffuse(0.3, 0.3, 0.3);
	mNeedDraw = needDraw;
	mIsRoot = IsRoot;
}


UIObject::~UIObject()
{
	if (mContent)
		delete mOutLine.pShape;
		delete mContent;
	for (auto p : instances)
	{
		delete p.first;
	}
}

void UIObject::Draw(Matrix & objectTr)
{
	if (!mNeedDraw) return;
	mOutLine.SetObjectTr(MyMul(mTr, objectTr));
	mOutLine.pShape->LSimpleVertexData[0].vertex = Vector3(0, 0, 0);
	//mOutLine.pShape->LSimpleVertexData[0].vertexTexture = Vector2(mSize.y / mSize.x, 0);//0 0

	mOutLine.pShape->LSimpleVertexData[1].vertex = Vector3(0, 0, 0 - mSize.y);
	//mOutLine.pShape->LSimpleVertexData[1].vertexTexture = Vector2(0, mSize.x / mSize.y);//0 1

	mOutLine.pShape->LSimpleVertexData[2].vertex = Vector3(0 + mSize.x, 0, 0);
	//mOutLine.pShape->LSimpleVertexData[2].vertexTexture = Vector2(mSize.x/mSize.y, 0);//1 0

	mOutLine.pShape->LSimpleVertexData[3].vertex = Vector3(0 + mSize.x, 0, 0 - mSize.y);
	//mOutLine.pShape->LSimpleVertexData[3].vertexTexture = Vector2(mSize.x / mSize.y,1 );//1 1
	//ObjectInfo& t= mOutLine.GetObjectInfo();
	//t.TextureSize.z = mSize.y / mSize.x;
	//t.TextureSize.w = mSize.x / mSize.y;

	if (!mIsRoot)
	{
		if (mOutLineDraw)
		{
			mOutLine.DynamicDraw2D(GraphicsManager::GetInstance()->pObjectBuff,
				GraphicsManager::GetInstance()->pModelBuff,
				GraphicsManager::GetInstance()->pBoneBuff);
		}
	}

		
	if(mContent)
		mContent->Draw(MyMul(mTr, objectTr));
	auto i = instances.begin();
	while (i != instances.end())
	{
		(*i).first->Draw(MyMul((*i).second, MyMul(mTr, objectTr)));
		i++;
	}

}


void UIObject::SetSize(float x, float y)
{
	mSize = Vector2(x, y);
}

void UIObject::SetTr(int index, Matrix Tr)
{
	instances[index].second = Tr;
}
