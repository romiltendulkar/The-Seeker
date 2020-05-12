/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: GameState.h
Purpose: Header for the GameState class. This class shall be used to maintain the current state
of the game. i.e.:- Pause, Play, Menu states.
Platform: Windows 10, x64
Project: romil.tendulkar, GAM550
Author: Romil Tendulkar, romil.tendulkar, 60001218
Creation date: 26/09/2019
- End Header --------------------------------------------------------*/

#include "../pch.h"
#include "PlayState.h"
#include "..\GameObject.h"
#include "..\..\Manager\ObjectFactoryManager.h"
#include "..\..\Manager\AudioManager.h"
#include "..\..\Manager\ImGUIManager.h"
#include "..\..\Manager\MemoryManager.h"
#include "..\..\Manager\PuzzleManager.h"
#include "../../Manager/UIManager.h"
#include "..\..\Manager\EventManager.h"
#include "..\..\Manager\ParticleManager.h"
#include "..\Entity\Player.h"



PlayState::PlayState(int type) : GameState(GAME_STATE_TYPE::PLAY_STATE)
{
	if (type == 0)
	{
		mPType = PROT_TYPE::RPG;
	}
	if (type == 1)
	{
		mPType = PROT_TYPE::STEALTH;
	}
	if (type == 2)
	{
		mPType = PROT_TYPE::RUNNER;
	}
}

PlayState::~PlayState()
{
	//destroy the managers
}

void PlayState::Init(GameStateManager* _owner, std::string level)
{
	//set the initial values for everything
	MemoryManager::GetInstance()->mpOwner = this;
	ObjectFactoryManager::GetInstance()->LoadLevel(level);
	mCurrentLevel = level;
	if (mNextLevel.empty())
	{
		mNextLevel = mCurrentLevel;
	}
	//AudioManager::GetInstance()->LoadLoopingAudio("Background.wav");
	mpOwner = _owner;
	MemoryManager::GetInstance()->mProtType = mPType;
	PuzzleManager::GetInstance()->Initialize();
	//things like load level go here
}

void PlayState::CleanUp()
{
	// reset camera
	// reset graphics window width and height


	//reset anything else thats created
	MemoryManager::GetInstance()->Destroy();
}

void PlayState::UpdateState()
{
	//mPM->Update();
}

void PlayState::Reset(std::string _level)
{
	//set time aswell
	CleanUp();
	//call init
}

void PlayState::Draw()
{
	MSG msg;
	bool  result;


	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	TimerManager::GetInstance()->Reset();
	// Loop until there is a quit message from the window or the user.
	mDone = false;
	float fps = 0;
	UIManager::GetInstance()->LoadIngameUI();
	UIManager::GetInstance()->LoadIngameMenu();
	while (!mDone)
	{
		if (!mChangingLevel)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			if (msg.message == WM_QUIT)
			{
				mDone = true;
				PhysicsManager::GetInstance()->Stop = true;
			}

			TimerManager::GetInstance()->StartTime();

			mpOwner->pOwner->deltaTime = TimerManager::GetInstance()->getDeltaTime();

			// If windows signals to end the application then exit out.
			//cout << mpOwner->pOwner->deltaTime << endl;

			DXInputManager::GetInstance()->Update(mpOwner->pOwner->deltaTime);
			if (!mPaused)
			{
				mpOwner->pOwner->Frame(mpOwner->pOwner->deltaTime);
				if (!(mCurrentLevel == "Level4" || mCurrentLevel == "Level3"))
				{
					(*MemoryManager::GetInstance()->ObjectPool)[2].pCr->usable = 0;
				}
				else
				{
					(*MemoryManager::GetInstance()->ObjectPool)[2].pCr->usable = 1;
				}
				GameObjectManager::GetInstance()->Update(mpOwner->pOwner->deltaTime);
				UIManager::GetInstance()->mPause = false;
			}


			//mpOwner->pOwner->CallInputManager();

		//if (mPType == PROT_TYPE::STEALTH)
			UIManager::GetInstance()->IOShowGoal(0);
		if ((mCurrentLevel == "Level4" || mCurrentLevel == "Level3"))
		{
			GraphicsManager::GetInstance()->GetCamera()->SetRoll(0);
			GraphicsManager::GetInstance()->GetCamera()->SetPan(0);
			GraphicsManager::GetInstance()->GetCamera()->SetTilt(66);
			GraphicsManager::GetInstance()->GetCamera()->mZoom = 41;
			UIManager::GetInstance()->IOShowGoal(1);
		}
		else if(mCurrentLevel == "MenuLevel")
		{
			GraphicsManager::GetInstance()->GetCamera()->SetRoll(180);
			GraphicsManager::GetInstance()->GetCamera()->SetTilt(30);
			GraphicsManager::GetInstance()->GetCamera()->mZoom = 30;
		}
		//if (mPType == PROT_TYPE::RUNNER|| mPType== PROT_TYPE::RPG)
		//{
		//	GraphicsManager::GetInstance()->GetCamera()->SetPan(0);
		//	GraphicsManager::GetInstance()->GetCamera()->SetTilt(41);
		//	GraphicsManager::GetInstance()->GetCamera()->mZoom = 41;
		//}

			//auto zoomin = DXInputManager::GetInstance()->GamepadLeftTriggers();
			//GraphicsManager::GetInstance()->setCameraDistance(-zoomin);

			//float x, y;
			//DXInputManager::GetInstance()->GamepadRightThumbSticks(y, x);
			//GraphicsManager::GetInstance()->setCameraRotation(0.0f, x * 2, y * 2, 3.0f);
			//auto zoomout = DXInputManager::GetInstance()->GamepadRightTriggers();
			//GraphicsManager::GetInstance()->SetCameraDistance(zoomout);

			//auto zoomin = DXInputManager::GetInstance()->GamepadLeftTriggers();
			//GraphicsManager::GetInstance()->SetCameraDistance(-zoomin);

		//demo the input controll camera
		//auto dis = DXInputManager::GetInstance()->GetMouseWheel();
		//GraphicsManager::GetInstance()->SetCameraDistance(float(dis / 50));
		//if (DXInputManager::GetInstance()->isMousePressed(0) == true)
		//{
		//	int dX, dY;

		//	DXInputManager::GetInstance()->GetMouseDelta(dX, dY);//stop mouse input if want capture mouse is true, to stop camera update on interacting with imgui windows
		//	GraphicsManager::GetInstance()->SetCameraRotation(0.0f, (float)dY, (float)dX, 3.0f);
		//}
		//if (DXInputManager::GetInstance()->isKeyPressed(DIK_Q) == true)
		//{
		//	GraphicsManager::GetInstance()->SetCameraMovement(r_x_q, r_y, r_z, sensitivity);
		//}
		//if (DXInputManager::GetInstance()->isKeyPressed(DIK_E) == true)
		//{
		//	GraphicsManager::GetInstance()->SetCameraMovement(r_x_e, r_y, r_z, sensitivity);
		//}
		//if (DXInputManager::GetInstance()->isKeyPressed(DIK_UP) == true)
		//{
		//	GraphicsManager::GetInstance()->SetCameraMovement(m_x_w, m_y_w, m_z_w, m_sensitivity);
		//}
		//if (DXInputManager::GetInstance()->isKeyPressed(DIK_DOWN) == true)
		//{
		//	GraphicsManager::GetInstance()->SetCameraMovement(m_x_s, m_y_s, m_z_s, m_sensitivity);
		//}
		//if (DXInputManager::GetInstance()->isKeyPressed(DIK_LEFT) == true)
		//{
		//	GraphicsManager::GetInstance()->SetCameraMovement(m_x_a, m_y_a, m_z_a, m_sensitivity);
		//}
		//if (DXInputManager::GetInstance()->isKeyPressed(DIK_RIGHT) == true)
		//{
		//	GraphicsManager::GetInstance()->SetCameraMovement(m_x_d, m_y_d, m_z_d, m_sensitivity);
		//}
		//if (DXInputManager::GetInstance()->isKeyTrigger(DIK_ESCAPE) == true)
		//{
		//	mDone = true;
		//}
		if (mPaused)windowspause = 0;

		Player* pl = static_cast<Player*>((*MemoryManager::GetInstance()->ObjectPool)[2].pEn);
		if ((mCurrentLevel == "Level4" || mCurrentLevel == "Level3") &&(DXInputManager::GetInstance()->isKeyTrigger(DIK_ESCAPE) == true 
			|| DXInputManager::GetInstance()->GamepadTrigger(GAMEPAD_MENU)|| windowspause
			))
		{
			//if(!(windowspause&&mPaused))
			{
				UIManager::GetInstance()->mPause = true;
				UIManager::GetInstance()->IOShowIngameMenu();
				mPaused = UIManager::GetInstance()->mPause;
				//windowspause = 0;
			}

		}
		if ((pl->mWinCounter > 0 || pl->mLoseCounter > 0))
		{
			mPaused = false;
		}
		//if ((mCurrentLevel == "Level4" || mCurrentLevel == "Level3") && windowspause)
		//{
		//	UIManager::GetInstance()->mPause = true;
		//	UIManager::GetInstance()->IOShowIngameMenu(1);
		//	mPaused = UIManager::GetInstance()->mPause;
		//}
		//if (DXInputManager::GetInstance()->isKeyTrigger(DIK_F2) == true)
		//{
		//	UIManager::GetInstance()->~UIManager();
		//	UIManager::GetInstance()->LoadIngameUI();
		//	UIManager::GetInstance()->LoadIngameMenu();
		//	mPaused = 0;
		//}
		if ((mCurrentLevel=="Level4"||mCurrentLevel == "Level3")&&(DXInputManager::GetInstance()->isKeyTrigger(DIK_TAB) == true|| DXInputManager::GetInstance()->GamepadTrigger(GAMEPAD_START)))
		{
			//mPaused = !mPaused;
			//GraphicsManager::GetInstance()->mPaused = mPaused;

				UIManager::GetInstance()->IOShowGoal();
				UIManager::GetInstance()->IOShowStat();
			}
			/*if (DXInputManager::GetInstance()->isKeyTrigger(DIK_I))
			{
				debug = !debug;
				if (!debug)
				{
					DXInputManager::GetInstance()->AcquireKeybroad();
				}

				else
				{
					DXInputManager::GetInstance()->UnacquireKeybroad();
				}
				GraphicsManager::GetInstance()->SetDebug(debug);

			}*/
			//if (DXInputManager::GetInstance()->isKeyTrigger(DIK_6))
			//{
			//	//unsigned int numObj = MemoryManager::GetInstance()->current_position;
			//	//for (unsigned int i = 0; i < numObj; ++i)

			//	(*MemoryManager::GetInstance()->ObjectPool).clear();

			//	ObjectFactoryManager::GetInstance()->LoadLevel("Level1");
			//}
			/*if (DXInputManager::GetInstance()->isKeyTrigger(DIK_O) == true)
			{
				GraphicsManager::GetInstance()->mCompletion = !GraphicsManager::GetInstance()->mCompletion;
			}*/

			//CollisionManager::GetInstance()->ResetGrid();
			UIManager::GetInstance()->Update();

			if (!mPaused)
				ParticleManager::GetInstance()->Update(mpOwner->pOwner->deltaTime);
			GraphicsManager::GetInstance()->Update(mpOwner->pOwner->deltaTime);
			fps = TimerManager::GetInstance()->EndTime();
		}
		else
		{
			Player* Retain = new Player();
			int left = 3;
			if (mPlayerRetention)
			{
				Player* pl = static_cast<Player*>((*MemoryManager::GetInstance()->ObjectPool)[2].pEn);
				left = pl->life;
				Retain->CopyStats(pl);
			}
			UIManager::GetInstance()->SetDialogue("");
			MemoryManager::GetInstance()->Reallocate();
			
			ObjectFactoryManager::GetInstance()->LoadLevel(mNextLevel);
			mCurrentLevel = mNextLevel;
			mChangingLevel = false;
			MemoryManager::GetInstance()->mpOwner = this;

			if (mNextLevel == "Level3")
			{
				PuzzleManager::GetInstance()->Initialize();
			}

			if (mPlayerRetention)
			{
				Player* pl = static_cast<Player*>((*MemoryManager::GetInstance()->ObjectPool)[2].pEn);
				pl->CopyStats(Retain);
				pl->life=left;

				pl->pCr->ammo_check = 0;
				pl->pCr->usable = true;
				pl->pCr->lock_check = false;
				pl->pCr->lock_key = -1;
				pl->pCr->auto_lock_key = 0;
				pl->pCr->rolling = false;
				pl->pCr->auto_check = false;
			}
			delete Retain;

			mPlayerRetention = false;
			if (mNextLevel == "Level3" || mNextLevel == "Level4")
			{
				UIManager::GetInstance()->IOShowDialogue(1);
				UIManager::GetInstance()->IOShowHP(1);
			}
		}

	}
	
}

void PlayState::SetType(PROT_TYPE type)
{
	mPType = type;
}

void PlayState::ChangeLevel(std::string levelname, bool retainplayer)
{
	mNextLevel = levelname;
	mPlayerRetention = retainplayer;
	mChangingLevel = true;
	mPaused = 0;
	windowspause = 0;
}
