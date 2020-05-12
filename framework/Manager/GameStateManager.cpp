/**
 * @file GameStateManager.h
 * @authors  Romil Tendulkar, Supreeth Rao Pejawar
 * @date 11/29/2019
 * @brief This class will handle the GameStates during runtime

 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */


#include "../Source/pch.h"
#include "GameStateManager.h"
#include "../Manager/GraphicsManager.h"
#include "../Manager/TimerManager.h"
#include "../Manager/DXInputManager.h"
#include "../Manager/GameObjectManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/MemoryManager.h"
#include "../Manager/PhysicsManager.h"
#include "../Manager/GameObjectManager.h"
#include "../Manager/ImGUIManager.h"
#include "../Manager/ObjectFactoryManager.h"
#include "../Manager/CollisionManager.h"
#include "../Manager/LevelEditor.h"
#include "../Manager/AudioManager.h"
#include "../Source/GameObject.h"
#include "../Source/GameStates/GameState.h"

GameStateManager::GameStateManager()
{
}
void GameStateManager::Initialize(Engine *pOwner)
{
	this->pOwner = pOwner;
	mStates = std::stack<GameState*>();
	pCurrentState = nullptr;
}

GameStateManager::~GameStateManager()
{
	
}

void GameStateManager::ChangeState(GameState* state, std::string str)
{
	pCurrentState = state;
	if (!mStates.empty()) 
	{
		mStates.top()->CleanUp();
		mStates.pop();
	}

	// store and init the new state
	mStates.push(state);
	mStates.top()->Init(this, str);

}

void GameStateManager::PushState(GameState* state, std::string str)
{
	// pause current state
	if (!mStates.empty()) {
		mStates.top()->Pause();
	}

	// store and init the new state
	mStates.push(state);
	pCurrentState = mStates.top();
	mStates.top()->Init(this, str);

}

void GameStateManager::PopState()
{
	// cleanup the current state
	if (!mStates.empty()) {
		mStates.top()->CleanUp();
		mStates.pop();
	}

	// resume previous state
	if (mStates.size() > 0) {
		pCurrentState = mStates.top();
		mStates.top()->Resume();

	}
}

void GameStateManager::Reset()
{
	PopState();
	pCurrentState->Init(this);
}

void GameStateManager::Draw()
{
	if (mStates.size() > 0)
	{
		
		mStates.top()->Draw();
		
	}
}
GameStateManager* GameStateManager::GetInstance()
{
	if (mInstance == nullptr)
	{
		mInstance = new GameStateManager();
	}
	return mInstance;
}
void GameStateManager::Destroy()
{
	delete mInstance;
}