/**
 * @file DXInputManager.h
 * @authors Ching-Yen Lin
 * @date 9/1/19
 * @brief This file has methods to take input commands from keyboard,mouse and gamepad.
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */

#include "../Source/pch.h"
#include "DXInputManager.h"



DXInputManager::DXInputManager()
	:mDirectInput(0), mKeyboard(0), mMouse(0)
{
	mGamepad = make_unique<DirectX::GamePad>();
}

bool DXInputManager::Initialize(HINSTANCE hinstance, HWND hwnd, int screenW, int screenH)
{
	mHWND = hwnd;
	HRESULT result; 
	mScreenWidth = screenW;
	mScreenHeight = screenH;
	mMouseX = 0;
	mMouseY = 0;
	mMouseDeltaX = 0;
	mMouseDeltaY = 0;
	memset(mCurrentKState, 0, 256 * sizeof(unsigned char));
	memset(mPreviousKState, 0, 256 * sizeof(unsigned char));
	memset(mCurrentMState, 0, 4 * sizeof(unsigned char));
	memset(mPreviousMState, 0, 4 * sizeof(unsigned char));
	
	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&mDirectInput, NULL);
	//there are other device can create
	//c_dfDIMouse, c_dfDIMouse2, c_dfDIKeyboard, c_dfDIJoystick, or c_dfDIJoystick2
	//create keyboard
	result = mDirectInput->CreateDevice(GUID_SysKeyboard, &mKeyboard, NULL);
	result = mKeyboard->SetDataFormat(&c_dfDIKeyboard);
	result = mKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	//focus Keyboard
	result = mKeyboard->Acquire();
	
	//mouse
	result = mDirectInput->CreateDevice(GUID_SysMouse, &mMouse, NULL);
	result = mMouse->SetDataFormat(&c_dfDIMouse);
	result = mMouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	//focus Mouse
	result = mMouse->Acquire();
	return true;
}

DXInputManager::~DXInputManager()
{
	if (mKeyboard)
	{
		mKeyboard->Unacquire();
		mKeyboard->Release();
		mKeyboard = 0;
	}
	if (mMouse)
	{
		mMouse->Unacquire();
		mMouse->Release();
		mMouse = 0;
	}
	if (mDirectInput)
	{
		mDirectInput->Release();
		mDirectInput = 0;
	}
	mGamepad.release();
}

DXInputManager * DXInputManager::GetInstance()
{
	if (!mInstance)
	{
		mInstance = new DXInputManager();
	}
	return mInstance;
}

void DXInputManager::Destroy()
{
	delete mInstance;
}

void DXInputManager::UnacquireKeybroad()
{
	mKeyboard->Unacquire();
}

void DXInputManager::AcquireKeybroad()
{
	if(mKeyboard)
		mKeyboard->Acquire();
}

void DXInputManager::Update(float deltaTime)
{
	HRESULT result;
	//update mouse
	memcpy(mPreviousMState, mMouseState.rgbButtons, 4 * sizeof(unsigned char));
	result = mMouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mMouseState);
	memcpy(mCurrentMState, mMouseState.rgbButtons, 4 * sizeof(unsigned char));
	if (FAILED(result))
	{
		mMouseDeltaX = 0;
		mMouseDeltaY = 0;
		// If the mouse lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			result=mMouse->Acquire();
		}
	}
	if (SUCCEEDED(result))
	{
		mMouseDeltaX = mMouseState.lX;
		mMouseDeltaY = mMouseState.lY;
		POINT pos;
		if (HWND active_window = ::GetForegroundWindow())
		{
			if (active_window == mHWND || ::IsChild(active_window, mHWND))
			{
				if (::GetCursorPos(&pos) && ::ScreenToClient(mHWND, &pos))
				{
					mMouseX = pos.x;
					if(pos.x < 0)mMouseX = 0;
					if (pos.x > mScreenWidth)mMouseX = mScreenWidth;
					mMouseY = pos.y;
					if (pos.y < 0)mMouseY = 0;
					if (pos.y > mScreenHeight)mMouseY = mScreenHeight;
				}
			}
		}
	}
	
	//update mKeyboard
	memcpy(mPreviousKState, mCurrentKState, 256 * sizeof(unsigned char));
	result = mKeyboard->GetDeviceState(sizeof(mCurrentKState), (LPVOID)&mCurrentKState);
	if (FAILED(result))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			mKeyboard->Acquire();
		}
	}
	mGamepad->Resume();
	mPreviousGState = mCurrentGState;
	mCurrentGState = mGamepad->GetState(0);
}

void DXInputManager::SetScreenSize(int screenW, int sscreenH)
{
	mScreenWidth = screenW;
	mScreenHeight = sscreenH;
}

void DXInputManager::GetMouseLocation(int & mX, int & mY)
{
	mX = mMouseX;
	mY = mMouseY;
	return;
}

void DXInputManager::GetMouseDelta(int & mX, int & mY)
{
	mX = mMouseDeltaX;
	mY = mMouseDeltaY;
	return;
}

bool DXInputManager::isKeyPressed(unsigned int key)
{
	if (key >= 256)
	{
		return false;
	}
	if (mCurrentKState[key] & 0x80)
	{
		return true;
	}
	return false;
}

bool DXInputManager::isKeyRelease(unsigned int key)
{
	if (key >= 256)
	{
		return false;
	}
	if (!mCurrentKState[key] && mPreviousKState[key])
	{
		return true;
	}
	return false;
}

bool DXInputManager::isKeyTrigger(unsigned int key)
{
	if (key >= 256)
	{
		return false;
	}
	if (mCurrentKState[key] && !mPreviousKState[key])
	{
		return true;
	}
	return false;
}

bool DXInputManager::isMousePressed(unsigned int key)
{

	if (key >= 4)
	{
		return false;
	}
	if (mCurrentMState[key] & 0x80)
	{
		return true;
	}
	return false;
}

bool DXInputManager::isMouseRelease(unsigned int key)
{
	if (key >= 4)
	{
		return false;
	}
	if (!mCurrentMState[key] && mPreviousMState[key])
	{
		return true;
	}
	return false;
}

bool DXInputManager::isMouseTrigger(unsigned int key)
{
	if (key >= 4)
	{
		return false;
	}
	if (mCurrentMState[key] && !mPreviousMState[key])
	{
		return true;
	}
	return false;
}

LONG DXInputManager::GetMouseWheel()
{
	return mMouseState.lZ;
}

bool DXInputManager::GamepadPressed(GAMEPADKEY rkey)
{
	if (!mCurrentGState.connected)
	{
		return false;
	}
	switch (rkey)
	{
	case GAMEPAD_X:
		return mCurrentGState.buttons.x;
		break;
	case GAMEPAD_Y:
		return mCurrentGState.buttons.y;
		break;
	case GAMEPAD_A:
		return mCurrentGState.buttons.a;
		break;
	case GAMEPAD_B:
		return mCurrentGState.buttons.b;
		break;
	case GAMEPAD_UP:
		return mCurrentGState.IsDPadUpPressed();
		break;
	case GAMEPAD_DOWN:
		return mCurrentGState.IsDPadDownPressed();
		break;
	case GAMEPAD_LEFT:
		return mCurrentGState.IsDPadLeftPressed();
		break;
	case GAMEPAD_RIGHT:
		return mCurrentGState.IsDPadRightPressed();
		break;
	case GAMEPAD_START:
		return mCurrentGState.buttons.start;
		break;
	case GAMEPAD_MENU:
		return mCurrentGState.buttons.back;
		break;
	case GAMEPAD_RIGHTSHOULDER:
		return mCurrentGState.IsLeftShoulderPressed();
		break;
	case GAMEPAD_LEFTSHOULDER:
		return mCurrentGState.IsRightShoulderPressed();
		break;
	default:
		false;
	}
	return false;
}

bool DXInputManager::GamepadRelease(GAMEPADKEY rkey)
{
	if (!mPreviousGState.connected)
	{
		return false;
	}
	switch (rkey)
	{
	case GAMEPAD_X:
		return !GamepadPressed(rkey) && mPreviousGState.buttons.x;
		break;
	case GAMEPAD_Y:
		return !GamepadPressed(rkey) && mPreviousGState.buttons.y;
		break;
	case GAMEPAD_A:
		return !GamepadPressed(rkey) && mPreviousGState.buttons.a;
		break;
	case GAMEPAD_B:
		return !GamepadPressed(rkey) && mPreviousGState.buttons.b;
		break;
	case GAMEPAD_UP:
		return !GamepadPressed(rkey) && mPreviousGState.IsDPadUpPressed();
		break;
	case GAMEPAD_DOWN:
		return !GamepadPressed(rkey) && mPreviousGState.IsDPadDownPressed();
		break;
	case GAMEPAD_LEFT:
		return !GamepadPressed(rkey) && mPreviousGState.IsDPadLeftPressed();
		break;
	case GAMEPAD_RIGHT:
		return !GamepadPressed(rkey) && mPreviousGState.IsDPadRightPressed();
		break;
	case GAMEPAD_START:
		return !GamepadPressed(rkey) && mPreviousGState.buttons.start;
		break;
	case GAMEPAD_MENU:
		return !GamepadPressed(rkey) && mPreviousGState.buttons.back;
		break;
	case GAMEPAD_RIGHTSHOULDER:
		return !GamepadPressed(rkey) && mPreviousGState.IsLeftShoulderPressed();
		break;
	case GAMEPAD_LEFTSHOULDER:
		return !GamepadPressed(rkey) && mPreviousGState.IsRightShoulderPressed();
		break;
	default:
		false;
	}
}

bool DXInputManager::GamepadTrigger(GAMEPADKEY rkey)
{
	if (!mPreviousGState.connected)
	{
		return false;
	}
	switch (rkey)
	{
	case GAMEPAD_X:
		return GamepadPressed(rkey) && !mPreviousGState.buttons.x;
		break;
	case GAMEPAD_Y:
		return GamepadPressed(rkey) && !mPreviousGState.buttons.y;
		break;
	case GAMEPAD_A:
		return GamepadPressed(rkey) && !mPreviousGState.buttons.a;
		break;
	case GAMEPAD_B:
		return GamepadPressed(rkey) && !mPreviousGState.buttons.b;
		break;
	case GAMEPAD_UP:
		return GamepadPressed(rkey) && !mPreviousGState.IsDPadUpPressed();
		break;
	case GAMEPAD_DOWN:
		return GamepadPressed(rkey) && !mPreviousGState.IsDPadDownPressed();
		break;
	case GAMEPAD_LEFT:
		return GamepadPressed(rkey) && !mPreviousGState.IsDPadLeftPressed();
		break;
	case GAMEPAD_RIGHT:
		return GamepadPressed(rkey) && !mPreviousGState.IsDPadRightPressed();
		break;
	case GAMEPAD_START:
		return GamepadPressed(rkey) && !mPreviousGState.buttons.start;
		break;
	case GAMEPAD_MENU:
		return GamepadPressed(rkey) && !mPreviousGState.buttons.back;
		break;
	case GAMEPAD_RIGHTSHOULDER:
		return GamepadPressed(rkey) && !mPreviousGState.IsLeftShoulderPressed();
		break;
	case GAMEPAD_LEFTSHOULDER:
		return GamepadPressed(rkey) && !mPreviousGState.IsRightShoulderPressed();
		break;
	default:
		false;
	}
	return false;
}

void DXInputManager::GamepadLeftThumbSticks(float& axisx, float& axisy)
{
	axisx = mCurrentGState.thumbSticks.leftX;
	axisy = mCurrentGState.thumbSticks.leftY;
}

void DXInputManager::GamepadRightThumbSticks(float& axisx, float& axisy)
{
	axisx = mCurrentGState.thumbSticks.rightX;
	axisy = mCurrentGState.thumbSticks.rightY;
}

float DXInputManager::GamepadLeftTriggers()
{
	return mCurrentGState.triggers.left;
}

float DXInputManager::GamepadRightTriggers()
{
	return mCurrentGState.triggers.right;
}

bool DXInputManager::SetGamepadVibration(float rleftMotor, float rrightMotor)
{
	return mGamepad->SetVibration(0, rleftMotor, rrightMotor);
}
