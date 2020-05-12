/*------------------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.

File Name:    main

Purpose:
Main entry point to the application
Language:   C++

Platform:

Compiler:-  Visual Studio 2017 - Visual C++ 14.1


Hardware Requirements:-		Display monitor, keyboard.

Operating System:-		Microsoft Windows 8.1+


Author: Ching-Yen Lin.

Student login: c.lin

Student ID: 60000818

Creation date:

---------------------------------------------------------*/
#include "pch.h"
#include "Engine.h"
// Turn on high power graphics for AMD and NVidia
extern "C"
{
	__declspec(dllexport) int           AmdPowerXpressRequestHighPerformance = 1;
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	Engine* System;
	bool result;

	//if (AllocConsole())
	//{
	//	FILE* file;
	//	freopen_s(&file, "CONOUT$", "wt", stdout);
	//	freopen_s(&file, "CONOUT$", "wt", stderr);
	//	freopen_s(&file, "CONOUT$", "wt", stdin);
	//}
	// Create the system object.
	System = new Engine;
	if (!System)return 0;


	// Initialize and run the system object.
	result = System->Initialize(2);
	if (result)System->Run();


	// Shutdown and release the system object.
	System->Shutdown();
	delete System;
	System = 0;

	return 0;
}