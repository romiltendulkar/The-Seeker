#pragma once

#include "GameState.h"
#include "../Engine.h"


class PauseState : public GameState
{
public:
	PauseState();
	~PauseState();
	void Init(GameStateManager* _owner, std::string level = "");
	void CleanUp();

	void Pause();
	void Resume();
	static PauseState* GetInstance() {
		return &mPauseState;
	}

	void HandleInput();
	void UpdateState();
	void Draw();
	void Reset(std::string _level = "");

private:
	static PauseState mPauseState;
};

