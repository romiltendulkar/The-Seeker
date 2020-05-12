/**
 * @file DXInputManager.h
 * @authors Ching-Yen Lin
 * @date 9/1/19
 * @brief This file has methods to take input commands from keyboard,mouse and gamepad.
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */

#ifndef DXINPUTMANGER_H
#define DXINPUTMANGER_H

///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#include "../Source/pch.h"
using namespace std;
enum GAMEPADKEY
{
	GAMEPAD_X = 0,
	GAMEPAD_Y,
	GAMEPAD_A,
	GAMEPAD_B,
	GAMEPAD_UP,
	GAMEPAD_DOWN,
	GAMEPAD_LEFT,
	GAMEPAD_RIGHT,
	GAMEPAD_START,
	GAMEPAD_MENU,
	GAMEPAD_RIGHTSHOULDER,
	GAMEPAD_LEFTSHOULDER
};
class DXInputManager
{
public:
	/**
	all pointer point to null
	*/
	DXInputManager();

	/**
	set up mouse, keybroad and controller
	*/
	bool Initialize(HINSTANCE, HWND, int, int);

	/**
	 delete all pointer
	*/
	~DXInputManager();

	/**
	This function return Manager itself
	*/
	static DXInputManager* GetInstance();
	void Destroy();

	void UnacquireKeybroad();
	void AcquireKeybroad();
	/**
	This function updates the state of every mouse, keyboard or controller key to the latest.
	deltaTime = framtime taken.
	*/
	void Update(float rdeltaTime);

	/**
	Reset the Screen size for detection.
	*/
	void SetScreenSize(int,int);
	
	/**
	Gets Mouse location.
	*/
	void GetMouseLocation(int&, int&);
	
	/**
	Gets change in mouse position.
	*/
	void GetMouseDelta(int&, int&);

	/**
	Checks if key of keybroad is pressed.
	*/
	bool isKeyPressed(unsigned int);

	/**
	Checks if key of keybroad is released.
	*/
	bool isKeyRelease(unsigned int);

	/**
	Checks if key of keybroad is triggered.
	*/
	bool isKeyTrigger(unsigned int);

	/**
	Checks if mouse key is pressed
	*/
	bool isMousePressed(unsigned int);

	/**
	Checks if mousekey is released.
	*/
	bool isMouseRelease(unsigned int);

	/**
	Checks if mousekey is triggered.
	*/
	bool isMouseTrigger(unsigned int);

	/**
	Gets state of mousewheel
	*/
	LONG GetMouseWheel();

	/**
	Checks if gamepad key is pressed
	*/
	bool GamepadPressed(GAMEPADKEY);

	/**
	Checks if gamepad key is released
	*/
	bool GamepadRelease(GAMEPADKEY);

	/**
	Checks if gamepad key is triggered
	*/
	bool GamepadTrigger(GAMEPADKEY);

	/**
	Stores states of left thumbsticks
	*/
	void GamepadLeftThumbSticks(float&, float&);

	/**
	Stores states of right thumbsticks
	*/
	void GamepadRightThumbSticks(float&, float&);
		
	/**
	Stores states of left triggers.
	*/
	float GamepadLeftTriggers();

	/**
	Stores states of right thumbsticks
	*/
	float GamepadRightTriggers();

	/**
	Sets vibration of gamepad
	*/
	bool SetGamepadVibration(float, float);


private:
	int mScreenWidth, mScreenHeight;
	HWND mHWND;
	IDirectInput8* mDirectInput;
	IDirectInputDevice8* mKeyboard;
	unsigned char mPreviousKState[256];
	unsigned char mCurrentKState[256];
	
	IDirectInputDevice8* mMouse;
	unsigned char mCurrentMState[4];
	unsigned char mPreviousMState[4];
	DIMOUSESTATE mMouseState;
	int mMouseX, mMouseY;
	int mMouseDeltaX, mMouseDeltaY;

	unique_ptr<DirectX::GamePad> mGamepad;
	DirectX::GamePad::State mPreviousGState;
	DirectX::GamePad::State mCurrentGState;

	static DXInputManager* mInstance;
};
#endif // !DXINPUTMANGER_H