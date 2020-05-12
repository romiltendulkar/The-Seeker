/////////////////////////////////////////////////////////////////////////
/**
 * @file GameState.h
 * @authors  Romil Tendulkar, Supreeth Rao Pejawar
 * @date 24/09/2019
 * @brief The base class for the Game States, 
	pointers for this class will be used to store the objects for the various States

 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
 /////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef GAME_STATE_H
#define GAME_STATE_H
#endif // !GAME_STATE_H

//move these to the pch file later.
#include "..\\..\\Manager\TimerManager.h"
#include "..\\..\\Manager\DXInputManager.h"
#include "..\\..\\Manager\GraphicsManager.h"
#include "..\..\Manager\GameObjectManager.h"
#include "..\\..\\Manager\PhysicsManager.h"
#include "..\\..\\Manager\CollisionManager.h"
#include "..\\..\\Manager\ResourceManager.h"
#include "..\\..\\Manager\GraphicsManager.h"
#include "..\\..\\Manager\AudioManager.h"
#include"..\..\Manager\GameStateManager.h"

//This class is not to be initialized, its the virtual base class for
// creating game states.


/////////////////////////////////////////////////////////////////////////
// State Type Enum
/////////////////////////////////////////////////////////////////////////

enum GAME_STATE_TYPE
{
	PLAY_STATE,
	PAUSE_STATE,
	LOADING_STATE,
	GAME_OVER,
	QUIT_STATE,
	NUM_STATE
};


/////////////////////////////////////////////////////////////////////////
// Class Definition
/////////////////////////////////////////////////////////////////////////

class GameState
{
	/////////////////////////////////////////////////////////////////////////
	//	Functions
	/////////////////////////////////////////////////////////////////////////

public:
	bool windowspause = false;

	/**
		Constructor for the GameState class sets default values for variables and 
		sets the State type to Play State
	*/
	GameState()
	{
		mpOwner = nullptr;
		mComplete = false;
		mPaused = false;
		mChangeState = false;
		mStateType = PLAY_STATE;
	}


	/**
		Constructor for the GameState class sets default values for variables and
		sets the State type to to the type passed

		\param state | the type of state that this Game State will be
	*/
	GameState(GAME_STATE_TYPE state)
	{
		mpOwner = nullptr;
		mComplete = false;
		mPaused = false;
		mChangeState = false;
		mStateType = state;
	}

	/**
		Virtual Destructor for the Game State Class
	*/
	virtual ~GameState() {};
	
	/**
		This function returns the type of the State

		\return The type of the Game State is returned
	*/
	GAME_STATE_TYPE GetStateType() { return mStateType; };

	/**
		Initializes the Game State telling it which level is to be loaded
		and which Game State Manager is handling this state

		\param owner | The pointer to the GameStateManager that is handling this State
		\param level | The name of the file that has the information for the level to be loaded
	*/
	virtual void Init(GameStateManager* owner, std::string level = "") { };
	virtual void Reset(std::string _level = "") {};
	virtual void UpdateState() = 0;
	virtual void Draw() = 0;
	virtual void HandleInput() {}
	virtual void CleanUp() {}
	virtual void Pause() {}
	virtual void Resume() {}

	/////////////////////////////////////////////////////////////////////////
	// Variables
	/////////////////////////////////////////////////////////////////////////
	GAME_STATE_TYPE mStateType;
	GameStateManager* mpOwner;
	bool mComplete, mPaused;
	bool mDone = false;
	bool mChangeState;
	
};