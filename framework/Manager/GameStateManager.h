/**
 * @file GameStateManager.h
 * @authors  Romil Tendulkar, Supreeth Rao Pejawar
 * @date 11/29/2019
 * @brief This class will handle the GameStates during runtime

 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */

#pragma once
#include "../Source/Engine.h"

class GameState;

class GameStateManager
{
public:
	/**
	take the engine pointer and stack the Game State
	*/
	GameStateManager();
	void Initialize(Engine *pOwner);
	~GameStateManager();

	/**
	Change game sate
	*/
	void ChangeState(GameState* state, std::string str = "");

	/**
	add game state in the stack
	*/
	void PushState(GameState* state, std::string str = "");

	/**
	pop game state in the stack
	*/
	void PopState();

	/**
	Reset game state in the stack
	*/
	void Reset();

	/**
	run game state in the stack
	*/
	void Draw();

	/**
	tell how many game state in the stack
	*/
	size_t StateSize() { return mStates.size(); }
	static GameStateManager* GameStateManager::GetInstance();
	void GameStateManager::Destroy();
	Engine* pOwner;
	static GameStateManager* mInstance;
	GameState* pCurrentState;

private:
	std::stack<GameState*> mStates;
	
	
};

