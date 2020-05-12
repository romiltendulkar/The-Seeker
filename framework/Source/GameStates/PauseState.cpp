#include "..\pch.h"
#include "PauseState.h"


extern GraphicsManager* mGfx;
extern DXInputManager* mIM;

PauseState::PauseState() : GameState(PAUSE_STATE)
{
}

PauseState::~PauseState()
{
}

void PauseState::Init(GameStateManager* _owner, std::string level)
{
}

void PauseState::CleanUp()
{
}

void PauseState::Pause()
{
}

void PauseState::Resume()
{
}

void PauseState::HandleInput()
{
}

void PauseState::UpdateState()
{
}

void PauseState::Draw()
{
}

void PauseState::Reset(std::string _level)
{
}