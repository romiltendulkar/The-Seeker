#pragma once
#include "../Source/pch.h"
#include "../Source/Graphics/UI/UIObject.h"
class UIText;
class UIHP;
class UIManager
{
public:
	UIManager();
	void Initialize();
	void LoadIngameUI();
	void LoadIngameMenu();
	void LoadMainMenu();
	void LoadFullScreen(DXTexture*,bool);
	void LoadHalfScreen(DXTexture*, bool,bool =true);
	void Resize(float screenW, float screenH);
	~UIManager();
	void SetStats(std::string);
	void SetDialogue(std::string);
	void SetGoal(std::string);
	static UIManager* GetInstance();
	bool Update(float deltaTime=0);
	void Draw();
	std::vector<UIObject*>& GetUIScene();
	void Destroy();
	void IOShowStat() { if (mStatUIO&&!mPause) mShowStat = mStatUIO->mNeedDraw = !mShowStat; }
	void IOShowStat(bool b) { if (mStatUIO && !mPause) mShowStat = mStatUIO->mNeedDraw = b; }
	void IOShowMain(bool b);
	void IOShowGoal() { if (mGoalUIO && !mPause) mShowGoal = mGoalUIO->mNeedDraw = !mShowGoal; }
	void IOShowGoal(bool b) { if (mGoalUIO && !mPause) mShowGoal = mGoalUIO->mNeedDraw = b; }
	void IOShowIngameMenu();
	void IOShowIngameMenu(bool b);
	void IOShowHP() {if(mHPUIO && !mPause) mShowHP = mHPUIO->mNeedDraw = !mShowHP; }
	void IOShowHP(bool b) { if (mHPUIO && !mPause) mShowHP = mHPUIO->mNeedDraw = b; }
	void IOShowDialogue(bool b) { if (mDialogueUIO && !mPause) mDialogue = mDialogueUIO->mNeedDraw = b; }
	bool mPause = false;

private:
	bool front = true;
	static UIManager* mInstance;
	std::vector<UIObject*> mUIScene;
	std::vector<UIButton*> mButtonlist;
	float mScreenW;
	float mScreenH;
	UIText* mStatUI;
	UIText* mGoalUI;
	UIText* mDialogueUI;
	UIHP* mHPUI;
	UIObject* mStatUIO;
	UIObject* mGoalUIO;
	UIObject* mHPUIO;
	UIObject* mDialogueUIO;
	UIObject* mFullScreenUIO;
	UIObject* mHalfScreenUIO;
	int IngameMenuIdex=-1;
	int MainMenuIdex = -1;
	int IngameUI = -1;
	int seleted = -1;
	bool mShowStat = false;
	bool mShowGoal = false;
	bool mDialogue = true;
	bool mShowHP = true;
	bool mShowIngameMenu = false;
	void Group(Json::Value const & data, UIObject* root);
	void Items(Json::Value const & data, UIObject* root);
};

