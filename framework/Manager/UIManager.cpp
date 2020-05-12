#include "../Source/pch.h"
#include "UIManager.h"
#include "TimerManager.h"
#include "ResourceManager.h"
#include "GraphicsManager.h"
#include "DXInputManager.h"
#include "../Source/Entity/Player.h"
UIManager::UIManager()
{

}

void UIManager::Initialize()
{
	mScreenW = 800;
	mScreenH = 600;
	mFullScreenUIO = new UIObject(1920*2, 1080*2, -960, 540, 1, 0);
	mFullScreenUIO->mOutLineDraw = 0;
	mFullScreenUIO->mContent = new UITexture(0, 0, 1920*2, 1080*2);;
	mFullScreenUIO->mNeedDraw = 0;
	mHalfScreenUIO = new UIObject(1920, 1080, -480, 270, 1, 0);
	mHalfScreenUIO->mOutLineDraw = 0;
	mHalfScreenUIO->mContent = new UITexture(0, 0, 1920, 1080);;
	mHalfScreenUIO->mNeedDraw = 0;

}
void UIManager::LoadIngameUI()
{
	Json::Value root = ResourceManager::GetInstance()->CreateJsonData("UI/IngameUI");
	if (!root.isNull())
	{
		Json::Value root1 = root["InGameUI"];
		Json::Value  root2 = root1[0];
		if (root2.isMember("Group"))
		{
			Group(root2["Group"],nullptr);
			IngameUI = mUIScene.size() - 1;
		}
	}
	int w, h;
	GraphicsManager::GetInstance()->Getsize(w,h);
	Resize(w, h);
	IOShowStat();
	IOShowGoal();
	IOShowStat();
	IOShowGoal();
}
void UIManager::LoadIngameMenu()
{
	Json::Value root = ResourceManager::GetInstance()->CreateJsonData("UI/IngameMenu");

	if (!root.isNull())
	{
		Json::Value root1 = root["IngameMenu"];
		Json::Value  root2 = root1[0];
		if (root2.isMember("Group"))
		{
			Group(root2["Group"], nullptr);
			IngameMenuIdex = mUIScene.size() - 1;
		}
	}
	int w, h;
	GraphicsManager::GetInstance()->Getsize(w, h);
	Resize(w, h);

}
void UIManager::LoadMainMenu()
{
	if (MainMenuIdex == -1)
	{
		Json::Value root = ResourceManager::GetInstance()->CreateJsonData("UI/MainMenu");
		if (!root.isNull())
		{
			Json::Value root1 = root["MainMenu"];
			Json::Value  root2 = root1[0];
			if (root2.isMember("Group"))
			{
				Group(root2["Group"], nullptr);
				MainMenuIdex = mUIScene.size() - 1;
			}
		}
		mUIScene[MainMenuIdex]->mNeedDraw = 1;
		int w, h;
		GraphicsManager::GetInstance()->Getsize(w, h);
		Resize(w, h);
	}
	
}
void UIManager::LoadFullScreen(DXTexture *tx, bool yn)
{

	if (mFullScreenUIO)
	{
		auto uitx = static_cast<UITexture*>(mFullScreenUIO->mContent);
		if (tx)
		{
			uitx->SetTexture(tx);
		}
		mFullScreenUIO->mNeedDraw = yn;
	}
}
void UIManager::LoadHalfScreen(DXTexture* tx, bool yn,bool f)
{
	front = f;
	if (mHalfScreenUIO)
	{
		auto uitx = static_cast<UITexture*>(mHalfScreenUIO->mContent);
		if (tx)
			uitx->SetTexture(tx);
		mHalfScreenUIO->mNeedDraw = yn;
	}
}
void UIManager::Group(Json::Value const & data, UIObject* root)
{
	bool isroot=false, needDraw = false,  hasItems = false, OutLineDraw = true;
	string Type;
	float SizeW, SizeH, PositionX, PositionY;
	if (data.isMember("isRoot"))
	{
		isroot = data["isRoot"].asBool();
	}
	if (data.isMember("needDraw"))
	{
		needDraw = data["needDraw"].asBool();
	}
	if (data.isMember("OutLineDraw"))
	{
		OutLineDraw = data["OutLineDraw"].asBool();
	}
	if (data.isMember("Size"))
	{
		SizeW = data["Size"][0].asFloat();
		SizeH = data["Size"][1].asFloat();
	}
	if (data.isMember("Position"))
	{
		PositionX = data["Position"][0].asFloat();
		PositionY = data["Position"][1].asFloat();
	}

	if (data.isMember("Type"))
	{
		if (root)
		{
			auto temp = new UIObject(SizeW, SizeH, PositionX, PositionY, needDraw);
			temp->mOutLineDraw = OutLineDraw;
			Type = data["Type"].asString();
			if (Type == "Goal")
			{
				mGoalUI = new UIText(SizeW*0.1, -SizeH*0.1, 0.8, SizeW*0.9);
				temp->mContent = mGoalUI;
				mGoalUIO = temp;
			}
			else if (Type == "Stat")
			{
				mStatUI = new UIText(SizeW*0.1, -SizeH * 0.1, 0.8, SizeW*0.9);
				temp->mContent = mStatUI;
				mStatUIO = temp;
			}
			else if (Type == "Dialogue")
			{
				mDialogueUI = new UIText(SizeW*0.2, -SizeH * 0.3, 0.8, SizeW*0.95);
				temp->mContent = mDialogueUI;
				mDialogueUIO = temp;
			}
			else if (Type == "Button")
			{
				auto tempb = new UIButton(0, 0, SizeW, SizeH);
				temp->mContent = tempb;
				auto Texture = data["Texture"].asString();
				auto Textureid=ResourceManager::GetInstance()->LoadTexture(Texture);
				tempb->SetTexture(Textureid);
				int type = 0;
				if (data.isMember("FunctionType"))
				{
					type = data["FunctionType"].asInt();
					tempb->Type = type;
				}
				//if (type == 2|| type == 1)
				{
					if (data.isMember("TextureList"))
					{
						for (int x = 0; x < data["TextureList"].size(); x++)
						{
							auto Textureid2 = ResourceManager::GetInstance()->LoadTexture(data["TextureList"][x].asString());
							if (Textureid2)
								tempb->textureList.push_back(Textureid2);
						}
					}
				}
				if (type == 7)
				{
					if (!GraphicsManager::GetInstance()->FULL_SCREEN)
					{
						tempb->SetTexture(tempb->textureList[0]);
					}
					else
					{
						tempb->SetTexture(tempb->textureList[1]);
					}
				}
			}
			else if (Type == "Texture")
			{
				auto tempb = new UITexture(0, 0, SizeW, SizeH);
				temp->mContent = tempb;
				auto Texture = data["Texture"].asString();
				bool alpha;
				if (data.isMember("Alpha"))
				{
					alpha = data["Alpha"].asBool();
					tempb->HasAlpha(alpha);
				}
				auto Textureid = ResourceManager::GetInstance()->LoadTexture(Texture);
				tempb->SetTexture(Textureid);
			}
			else if (Type == "HP")
			{
				mHPUI = new UIHP(4, 4, 592, 27);
				temp->mContent = mHPUI;
				mHPUIO = temp;
			}
			else
			{
				return;
			}
			if (data.isMember("Color"))
			{
				temp->mOutLine.SetDiffuse(data["Color"][0].asFloat(), data["Color"][1].asFloat(), data["Color"][2].asFloat());
			}
			root->add(temp, Matrix());
			if (data.isMember("Items"))
			{
				for (int i = 0; i < data["Items"].size(); i++)
				{
					Items(data["Items"][i], temp);
				}
			}
		}
	}
	else if (data.isMember("Items"))
	{
		mUIScene.push_back(new UIObject(SizeW, SizeH, PositionX, PositionY, needDraw, isroot));
		for (int i = 0; i < data["Items"].size(); i++)
		{
			Items(data["Items"][i], mUIScene[mUIScene.size()-1]);
		}
	}
}
void UIManager::Items(Json::Value const & data, UIObject* root)
{
	if (data.isMember("Group"))
	{
		Group(data["Group"], root);
	}
}
void UIManager::Resize(float screenW, float screenH)
{
	for (auto &p : mUIScene)
	{
		p->mTr = MyMul(MyScale(screenW / mScreenW, 0, screenH / mScreenH), MyTranslate(-screenW / 2.0f, 0, screenH / 2.0f)); //
	}
}


UIManager::~UIManager()
{
	for (auto p : mUIScene)
	{
		delete p;
	}
	mUIScene.clear();
	IngameMenuIdex = -1;
	MainMenuIdex = -1;
	IngameUI = -1;
	seleted = -1;
	mShowStat = false;
	mShowGoal = false;
	mShowHP = true;
	mShowIngameMenu = false;

}
void UIManager::SetStats(std::string s)
{
	if (mStatUI)
		mStatUI->SetText(s);
}
void UIManager::SetDialogue(std::string s)
{
	if (mDialogueUI)
		mDialogueUI->SetText(s);
}
void UIManager::SetGoal(std::string s)
{
	if (mGoalUI)
		mGoalUI->SetText(s);
}
UIManager* UIManager::GetInstance()
{
	if (mInstance == nullptr)
	{
		mInstance = new UIManager();
	}
	return mInstance;
}
bool UIManager::Update(float deltaTime)
{
	//no draw
	Player* mp;
	mp = static_cast<Player*>((*MemoryManager::GetInstance()->ObjectPool)[2].pEn);
	string s = "";
	//s += "Body Armor:\n ";
	//s += mp->mEBody->getArmorName();
	//s += "\nWeapon: \n ";
	//s += mp->mEWeapon->getWepName();
	//s += "\nDef: ";
	//s += to_string(int(mp->mEBody->getDefense()));
	/*s += " Dmg: ";
	s += to_string(int(mp->mEWeapon->getDamage()));*/
	/*s += "\nCrit Chance: \n";
	s += to_string(int(mp->mEWeapon->getCritChance()));*/
	/*s += "\nDodge Chance: \n";
	s += to_string(int(mp->mEBody->getDodgeChance()));*/
	/*s += "\nEffect: \n ";
	s += mp->mEWeapon->mWeaponPerk->getEffectDescription();*/

	s += "\nFPS: \n ";
	auto sa = 1.0f/TimerManager::GetInstance()->getDeltaTime();
	s += to_string(sa);
	SetStats(s);
	//SetDialogue("AAAAAAAAAAAAAAAAAAAAAA");
	//SetGoal("Objective:\n Beat the Boss");
	if(mHPUI)
		mHPUI->SetHP(mp->health);

	if (mButtonlist.size()>0&& seleted>-1)
	{
		if (!(mFullScreenUIO->mNeedDraw || mHalfScreenUIO->mNeedDraw))
		{
			if ((DXInputManager::GetInstance()->isKeyTrigger(DIK_W) == true)
				|| DXInputManager::GetInstance()->GamepadTrigger(GAMEPAD_UP))
			{
				seleted--;
			}
			if ((DXInputManager::GetInstance()->isKeyTrigger(DIK_S) == true)
				|| DXInputManager::GetInstance()->GamepadTrigger(GAMEPAD_DOWN))
			{
				seleted++;
			}
		}
		/*else
		{
			GameStateManager::GetInstance()->pCurrentState->mPaused = true;
		}*/
		for (int i = 0; i < mButtonlist.size(); i++)
		{
			mButtonlist[i]->Seleted(false);
		}
		if (seleted >= mButtonlist.size())
		{
			seleted = seleted % mButtonlist.size();
		}
		mButtonlist[seleted]->Seleted(true);
		if ((DXInputManager::GetInstance()->isKeyTrigger(DIK_RETURN) == true)
			|| DXInputManager::GetInstance()->GamepadTrigger(GAMEPAD_A))
		{
			mButtonlist[seleted]->Active();
		}
		else if ((DXInputManager::GetInstance()->isKeyTrigger(DIK_ESCAPE) == true)
			|| DXInputManager::GetInstance()->GamepadTrigger(GAMEPAD_B))
		{
			mButtonlist[seleted]->InActive();
		}
	}
	return true;
}
void UIManager::Draw()
{
	if (mHalfScreenUIO->mNeedDraw&&!front)
		mHalfScreenUIO->Draw(Matrix());
	auto i = mUIScene.begin();
	while (i != mUIScene.end())
	{
		(*i)->Draw(Matrix());
		i++;
	}
	if(mFullScreenUIO->mNeedDraw)
		mFullScreenUIO->Draw(Matrix());
	if (mHalfScreenUIO->mNeedDraw&&front)
		mHalfScreenUIO->Draw(Matrix());
	//mFullScreenUIO->Draw(Matrix());
	//mHalfScreenUIO->Draw(Matrix());
}
void UIManager::IOShowMain(bool b)
{
	if (MainMenuIdex != -1)
	{
		if ((mFullScreenUIO->mNeedDraw || mHalfScreenUIO->mNeedDraw))
		{
			mPause = true;
			return;
		}
		mShowIngameMenu = mUIScene[MainMenuIdex]->mNeedDraw = b;
		mPause = mShowIngameMenu;
		seleted = -1;
		if (mShowIngameMenu)
		{
			seleted = 0;
			for (int i = 0; i < mButtonlist.size(); i++)
			{
				mButtonlist[i]->Seleted(false);
			}
			mButtonlist.clear();
			for (int i = 0; i < mUIScene[MainMenuIdex]->instances.size(); i++)
			{
				auto& temp = mUIScene[MainMenuIdex]->instances[i].first;
				mButtonlist.push_back(static_cast<UIButton*>(temp->mContent));
			}
			if (seleted >= mButtonlist.size())
			{
				seleted = seleted % mButtonlist.size();
			}

			mButtonlist[seleted]->Seleted(true);
		}
		/*else
		{
			mButtonlist.clear();
		}*/
	}
}
void UIManager::IOShowIngameMenu() {
	if (IngameMenuIdex != -1)
	{
		if ((mFullScreenUIO->mNeedDraw || mHalfScreenUIO->mNeedDraw))
		{
			mPause = true;
			return;
		}
		mShowIngameMenu = mUIScene[IngameMenuIdex]->mNeedDraw = !mShowIngameMenu;
		mPause = mShowIngameMenu;
		seleted = -1;
		if (mShowIngameMenu)
		{
			seleted = 0;
			for (int i = 0; i < mButtonlist.size(); i++)
			{
				mButtonlist[i]->Seleted(false);
			}
			mButtonlist.clear();
			for (int i = 0; i < mUIScene[IngameMenuIdex]->instances.size(); i++)
			{
				auto& temp = mUIScene[IngameMenuIdex]->instances[i].first;
				mButtonlist.push_back(static_cast<UIButton*>(temp->mContent));
			}
			if (seleted >= mButtonlist.size())
			{
				seleted = seleted % mButtonlist.size();
			}

			mButtonlist[seleted]->Seleted(true);
		}
		else
		{
			mButtonlist.clear();
		}
	}
}
void UIManager::IOShowIngameMenu(bool b)
{
	if (IngameMenuIdex != -1)
	{
		if ((mFullScreenUIO->mNeedDraw || mHalfScreenUIO->mNeedDraw))
		{
			mPause = true;
			return;
		}
		mShowIngameMenu = mUIScene[IngameMenuIdex]->mNeedDraw = b;
		mPause = mShowIngameMenu;
		seleted = -1;
		if (mShowIngameMenu)
		{
			seleted = 0;
			for (int i = 0; i < mButtonlist.size(); i++)
			{
				mButtonlist[i]->Seleted(false);
			}
			mButtonlist.clear();
			for (int i = 0; i < mUIScene[IngameMenuIdex]->instances.size(); i++)
			{
				auto& temp = mUIScene[IngameMenuIdex]->instances[i].first;
				mButtonlist.push_back(static_cast<UIButton*>(temp->mContent));
			}
			if (seleted >= mButtonlist.size())
			{
				seleted = seleted % mButtonlist.size();
			}

			mButtonlist[seleted]->Seleted(true);
		}
		else
		{
			mButtonlist.clear();
		}
	}
}
std::vector<UIObject*>& UIManager::GetUIScene()
{
	return mUIScene;
}
void UIManager::Destroy()
{
	delete mInstance;
}