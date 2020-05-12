/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: PlayState.h
Purpose: Header for the PlayState class. This class shall be used to maintain the current state
of the game. i.e.:- Pause, Play, Menu states.
Platform: Windows 10, x64
Project: romil.tendulkar, GAM550
Author: Romil Tendulkar, romil.tendulkar, 60001218
Creation date: 26/09/2019
- End Header --------------------------------------------------------*/

#pragma once
#include "GameState.h"
#include "..\..\Manager\PhysicsManager.h"
#include "..\..\\Manager\CollisionManager.h"

enum class PROT_TYPE
{
	RPG,
	STEALTH,
	RUNNER
};


class PlayState : public GameState
{
public:
	PlayState(int type);
	~PlayState();
	void Init(GameStateManager* _owner, std::string level = "");
	void CleanUp();
	void UpdateState();
	void Draw();
	void SetType(PROT_TYPE type);
	/*static PlayState* GetInstance() 
	{
		return &mPlayState;
	};*/
	virtual void Reset(std::string _level = "");
	bool debug=false;
	bool mChangingLevel = false;

	void ChangeLevel(std::string levelname, bool retainplayer);
	std::string mNextLevel;
	std::string mCurrentLevel;

private:
	//static PlayState mPlayState;
	std::vector<std::string> levels;
	PROT_TYPE mPType;
	bool mPlayerRetention = false;
	
};