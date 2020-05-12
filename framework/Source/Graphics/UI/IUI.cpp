#include "../../pch.h"
#include "IUI.h"
#include "../../../Manager/UIManager.h"
#include "../../../Manager/GameStateManager.h"
#include "../../GameStates/PlayState.h"
#include "../../../Manager/MemoryManager.h"
IUI::IUI()
{
}


IUI::~IUI()
{
}

UIText::UIText(float posX, float posY, float size, float boundary)
	:IUI()
{
	mPosX = posX;
	mPosY = posY;
	mSize = size;
	mBoundary = boundary;
	auto q = new Quad();
	q->mDefaultDiffuseColor = Vector3(1, 1, 1);
	mObject = new DXObject(q, GraphicsManager::GetInstance()->GetDirectXHandler()->GetDevice(),
	GraphicsManager::GetInstance()->GetDirectXHandler()->GetDeviceContext(), true);

	mObject->SetMaterialID(4);

}

UIText::~UIText()
{
	if (mObject)
		delete mObject->pShape;
		delete mObject;
}

void UIText::SetText(string text)
{
	if (mText == text)
		return;
	mNewText.clear();
	mText = text;
	auto& charlist = GraphicsManager::GetInstance()->Characters;
	float tempx = mPosX, tempy = mPosY;
	float buff=(charlist[wchar_t(' ')].Advance >> 6) * mSize;
	for (int i = 0; i < mText.size(); i++)
	{
		char c = mText[i];
		if (c == '\n')
		{
			for (float w = tempx; w < mBoundary; w+= buff)
			{
				mNewText += ' ';
			}
			tempx = mPosX;
			continue;
		}
		bool conti = false;
		if (c == ' ')
		{
			float check = tempx;
			if (tempx == mPosX)
			{
				conti = 1;
			}
			else
			{
				for (int j = i + 1; j < mText.size(); j++)
				{
					if (mText[j] != ' '&&mText[j] != '\n')
					{
						check += (charlist[wchar_t(c)].Advance >> 6) * mSize;
						if (check > mBoundary)
						{
							for (float w = tempx; w < mBoundary; w += buff)
							{
								mNewText += ' ';
							}
							conti = 1;
							break;
						}
					}
					else
						break;
				}
			}
		}
		if (conti)
		{
			tempx = mPosX;
			continue;
		}
		mNewText += c;
		auto& mychar = charlist[wchar_t(c)];
		tempx += (mychar.Advance >> 6) * mSize;
		if (tempx > mBoundary)
		{
			tempx = mPosX;
		}
	}
}

void UIText::SetSize(float size)
{
	mSize = size;
}

void UIText::Draw(Matrix& tr)
{
	auto FrontQ = mObject->pShape;
	auto& charlist = GraphicsManager::GetInstance()->Characters;
	mObject->SetObjectTr(tr);
	float tempx= mPosX, tempy= mPosY;
	
	for (int i = 0; i < mNewText.size(); i++)
	{
		char c= mNewText[i];
		if (c == '\n')
		{
			tempx = mPosX;
			tempy -= 42 * mSize;
			continue;
		}
		auto& mychar= charlist[wchar_t(c)];
		float xpos = tempx + mychar.Bearing.x * mSize;
		float ypos = tempy - (mychar.Size.y - mychar.Bearing.y) * mSize;

		float w = mychar.Size.x * mSize;
		float h = mychar.Size.y * mSize;
		FrontQ->LSimpleVertexData[0].vertex = Vector3(xpos, 0, ypos);
		FrontQ->LSimpleVertexData[1].vertex = Vector3(xpos, 0, ypos + h);
		FrontQ->LSimpleVertexData[2].vertex = Vector3(xpos+ w, 0, ypos);
		FrontQ->LSimpleVertexData[3].vertex = Vector3(xpos+ w, 0, ypos + h);
		mObject->SetTexture(DIFFUSE_TEXTURE, mychar.Texture);
		mObject->DynamicDraw2D(GraphicsManager::GetInstance()->pObjectBuff,
			GraphicsManager::GetInstance()->pModelBuff,
			GraphicsManager::GetInstance()->pBoneBuff);
		tempx += (mychar.Advance >> 6) * mSize;
		if (tempx > mBoundary)
		{
			tempx = mPosX;
			tempy -= 42*mSize;
		}
	}
}

UIHP::UIHP(float posX, float posY, float BoundaryX, float BoundaryY)
	:IUI()
{
	mRatio = double(BoundaryX);
	mPosX = posX; mPosY = posX; mBoundaryX = BoundaryX; mBoundaryY = BoundaryY;
	auto q = new Quad();
	q->mDefaultDiffuseColor = Vector3(0.80, 0.42, 0.42);
	mObject = new DXObject(q, GraphicsManager::GetInstance()->GetDirectXHandler()->GetDevice(),
		GraphicsManager::GetInstance()->GetDirectXHandler()->GetDeviceContext(), true);
	lowHealth = Vector3(0.80, 0.42, 0.42);
	hightHealth = Vector3(0.20, 0.8, 0.42);
	mObject->SetDiffuse(0.20, 0.8, 0.42);
	mObject->SetMaterialID(5);
}

UIHP::~UIHP()
{
	if (mObject)
		delete mObject->pShape;
		delete mObject;
}

void UIHP::SetHP(float size)
{
	mSize = size;
}

void UIHP::Draw(Matrix& tr)
{
	mBoundaryX = mSize / 100.0f*mRatio;

	auto color= TranslationLERP(lowHealth, hightHealth, mSize / 100.0f);
	mObject->SetDiffuse(color.x, color.y, color.z);
	mObject->SetObjectTr(tr);
	auto HPQ = mObject->pShape;
	HPQ->LSimpleVertexData[0].vertex = Vector3(mPosX, 0, -mPosY);
	HPQ->LSimpleVertexData[1].vertex = Vector3(mPosX, 0, -(mPosY + mBoundaryY));
	HPQ->LSimpleVertexData[2].vertex = Vector3(mPosX + mBoundaryX, 0, -mPosY);
	HPQ->LSimpleVertexData[3].vertex = Vector3(mPosX + mBoundaryX, 0, -(mPosY + mBoundaryY));

	if (mSize > 0)
	{
		mObject->DynamicDraw2D(GraphicsManager::GetInstance()->pObjectBuff,
			GraphicsManager::GetInstance()->pModelBuff,
			GraphicsManager::GetInstance()->pBoneBuff);
	}
}
UIButton::UIButton(float posX, float posY, float BoundaryX, float BoundaryY)
	:IUI()
{
	mPosX = posX; mPosY = posX; mBoundaryX = BoundaryX; mBoundaryY = BoundaryY;
	auto q = new Quad();
	q->mDefaultDiffuseColor = Vector3(0.80, 0.42, 0.42);
	mObject = new DXObject(q, GraphicsManager::GetInstance()->GetDirectXHandler()->GetDevice(),
		GraphicsManager::GetInstance()->GetDirectXHandler()->GetDeviceContext(), true);
	mObject->SetMaterialID(8);
	//mObject->SetDiffuse(0.20, 0.8, 0.42);
}
UIButton::~UIButton()
{
	if (mObject)
		delete mObject->pShape;
		delete mObject;
}
void UIButton::Draw(Matrix& tr)
{
	mObject->SetObjectTr(tr);
	auto Q = mObject->pShape;
	Q->LSimpleVertexData[0].vertex = Vector3(mPosX, 0, mPosY);
	Q->LSimpleVertexData[1].vertex = Vector3(mPosX, 0, (mPosY + mBoundaryY));
	Q->LSimpleVertexData[2].vertex = Vector3(mPosX + mBoundaryX, 0, mPosY);
	Q->LSimpleVertexData[3].vertex = Vector3(mPosX + mBoundaryX, 0, (mPosY + mBoundaryY));
	if(Type ==7)
	{
		SetTexture(textureList[GraphicsManager::GetInstance()->FULL_SCREEN]);
	}
	mObject->DynamicDraw2D(GraphicsManager::GetInstance()->pObjectBuff,
		GraphicsManager::GetInstance()->pModelBuff,
		GraphicsManager::GetInstance()->pBoneBuff);
	
}
void UIButton::SeletedIO()
{
	isSeleted = !isSeleted;
	if(isSeleted)
		mObject->SetMaterialID(9);
	else
		mObject->SetMaterialID(8);
}
void UIButton::Seleted(bool r)
{
	if (r)
		mObject->SetMaterialID(9);
	else
		mObject->SetMaterialID(8);
}
void UIButton::setType(int r)
{
	Type = r;
}
void UIButton::Active()
{
	
	switch (Type)
	{
	case 0:
		UIManager::GetInstance()->IOShowIngameMenu();
		GameStateManager::GetInstance()->pCurrentState->mPaused = false;
		cout << "mPaused\n";
		break;
	case 1:
		//UIManager::GetInstance()->IOShowIngameMenu();
		hitTime++;
		if (hitTime >= 0)
		{
			if (hitTime >= textureList.size())
			{
				hitTime = -1;
				UIManager::GetInstance()->LoadHalfScreen(nullptr, 0);
				GameStateManager::GetInstance()->pCurrentState->mDone = true;
				return;
			}
			UIManager::GetInstance()->LoadHalfScreen(textureList[hitTime], 1);
		}
		break;
	case 2:
		hitTime++;
		if (hitTime >= 0)
		{
			if (hitTime >= textureList.size())
			{
				hitTime = -1;
				UIManager::GetInstance()->LoadHalfScreen(nullptr, 0);
				return;
			}
			UIManager::GetInstance()->LoadHalfScreen(textureList[hitTime], 1);
		}
		break;
	case 3:
		UIManager::GetInstance()->LoadHalfScreen(nullptr, 0);
		UIManager::GetInstance()->LoadFullScreen(nullptr, 0);
		UIManager::GetInstance()->IOShowMain(0);
		MemoryManager::GetInstance()->mpOwner->ChangeLevel("Level3", false);

		break;
	case 4:
		UIManager::GetInstance()->LoadHalfScreen(nullptr, 0);
		UIManager::GetInstance()->LoadFullScreen(nullptr, 0);
		MemoryManager::GetInstance()->mpOwner->ChangeLevel("CreditsLevel", false);
		//MemoryManager::GetInstance()->mpOwner->ChangeLevel("Level3", false);

		//UIManager::GetInstance()->IOShowMain(0);
		break;
	case 5:
		hitTime++;
		if (hitTime >= 0)
		{
			if (hitTime >= textureList.size())
			{
				hitTime = -1;
				UIManager::GetInstance()->LoadHalfScreen(nullptr, 0);
				UIManager::GetInstance()->LoadFullScreen(nullptr, 0);
				MemoryManager::GetInstance()->mpOwner->ChangeLevel("CreditsLevel", false);
				break;
			}
			UIManager::GetInstance()->LoadHalfScreen(textureList[hitTime], 1);
		}
		break;
	case 6:
		hitTime++;
		if (hitTime >= 0)
		{
			if (hitTime >= textureList.size())
			{
				hitTime = -1;
				UIManager::GetInstance()->LoadHalfScreen(nullptr, 0);
				UIManager::GetInstance()->LoadFullScreen(nullptr, 0);
				UIManager::GetInstance()->IOShowHP(0);
				UIManager::GetInstance()->IOShowDialogue(0);
				UIManager::GetInstance()->IOShowIngameMenu(0);
				UIManager::GetInstance()->IOShowGoal(0);
				MemoryManager::GetInstance()->mpOwner->ChangeLevel("MenuLevel", false);
				UIManager::GetInstance()->IOShowStat(0);
				break;
			}
			UIManager::GetInstance()->LoadHalfScreen(textureList[hitTime], 1);
		}
		break;
	case 7:
		GraphicsManager::GetInstance()->FULL_SCREEN = !GraphicsManager::GetInstance()->FULL_SCREEN;
		SetTexture(textureList[GraphicsManager::GetInstance()->FULL_SCREEN]);
		GraphicsManager::GetInstance()->Fullscreen(GraphicsManager::GetInstance()->FULL_SCREEN);
		break;
	default:
		break;
	}
}
void UIButton::InActive()
{
	switch (Type)
	{
	case 1:
		if (hitTime > -1)
		{
			hitTime--;
		}
		if (hitTime < 0)
		{
			hitTime = -1;
			UIManager::GetInstance()->LoadHalfScreen(nullptr, 0);
			return;
		}
		break;
	case 2:
		if (hitTime > -1)
		{
			hitTime--;
		}
		if (hitTime < 0)
		{
			if (hitTime >= textureList.size())
			{
				hitTime = -1;
				UIManager::GetInstance()->LoadHalfScreen(nullptr, 0);
				return;
			}
			UIManager::GetInstance()->LoadHalfScreen(textureList[hitTime], 1);
		}
		break;
	case 5:
		if (hitTime > -1)
		{
			hitTime--;
		}
		if (hitTime < 0)
		{
			if (hitTime >= textureList.size())
			{
				hitTime = -1;
				UIManager::GetInstance()->LoadHalfScreen(nullptr, 0);
				return;
			}
			UIManager::GetInstance()->LoadHalfScreen(textureList[hitTime], 1);
		}
		break;
	case 6:
		if (hitTime > -1)
		{
			hitTime--;
		}
		if (hitTime < 0)
		{
			if (hitTime >= textureList.size())
			{
				hitTime = -1;
				UIManager::GetInstance()->LoadHalfScreen(nullptr, 0);
				return;
			}
			UIManager::GetInstance()->LoadHalfScreen(textureList[hitTime], 1);
		}
		break;
	default:
		break;
	}
}
void UIButton::SetTexture(DXTexture * rtexture)
{
	mObject->SetTexture(DIFFUSE_TEXTURE,rtexture);
}
UITexture::UITexture(float posX, float posY, float BoundaryX, float BoundaryY)
{
	mPosX = posX; mPosY = posX; mBoundaryX = BoundaryX; mBoundaryY = BoundaryY;
	auto q = new Quad();
	q->mDefaultDiffuseColor = Vector3(0.80, 0.42, 0.42);
	mObject = new DXObject(q, GraphicsManager::GetInstance()->GetDirectXHandler()->GetDevice(),
		GraphicsManager::GetInstance()->GetDirectXHandler()->GetDeviceContext(), true);
	mObject->SetMaterialID(8);
}
void UITexture::HasAlpha(bool a)
{
	alpha = a;
	if(alpha)
		mObject->SetMaterialID(3);
	else
		mObject->SetMaterialID(8);
}
void UITexture::Draw(Matrix& tr)
{
	mObject->SetObjectTr(tr);
	auto Q = mObject->pShape;
	Q->LSimpleVertexData[0].vertex = Vector3(mPosX, 0, -mPosY);
	Q->LSimpleVertexData[1].vertex = Vector3(mPosX, 0, -(mPosY + mBoundaryY));
	Q->LSimpleVertexData[2].vertex = Vector3(mPosX + mBoundaryX, 0, -mPosY);
	Q->LSimpleVertexData[3].vertex = Vector3(mPosX + mBoundaryX, 0, -(mPosY + mBoundaryY));
	Q->LSimpleVertexData[0].vertexTexture = Vector2(0,1);
	Q->LSimpleVertexData[1].vertexTexture = Vector2(0,0);
	Q->LSimpleVertexData[2].vertexTexture = Vector2(1, 1);
	Q->LSimpleVertexData[3].vertexTexture = Vector2(1, 0);
	mObject->DynamicDraw2D(GraphicsManager::GetInstance()->pObjectBuff,
		GraphicsManager::GetInstance()->pModelBuff,
		GraphicsManager::GetInstance()->pBoneBuff);

}
UITexture::~UITexture()
{
	if (mObject)
		delete mObject->pShape;
	delete mObject;
}
void UITexture::SetTexture(DXTexture * rtexture)
{
	mObject->SetTexture(DIFFUSE_TEXTURE, rtexture);
}