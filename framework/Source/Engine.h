/**
 * @file Engine.h
 * @authors Ching-Yen Lin, Pratyush Gawai, Vineet Amarjeet Dogra
 * @date 9/1/19
 * @brief Encapsulates all managers.
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */

#ifndef ENGINE_H_
#define ENGINE_H_


#include "pch.h"
class GameState;
class Engine
{
public:
	Engine();
	~Engine();

	typedef void (Engine::*callback_function)(void);
	callback_function Job;
	float deltaTime = 0.f;

	/**
		Initializes all the managers
	*/
	bool Initialize(int type);//init

	/**
	   This functions initializes the thread vector.
	*/
	void Init_Threads(int n);

	/**
		Update loop for the engine.
	*/
	void Run();//update

	/**
		Frees all the managers.
	*/
	void Shutdown();//destroy
	void HandleInputs();
	void Update();
	void Draw();
	//void HandleEvents() for event manager

	/**
	Calculates frame stats.
	*/
	void CalculateFrameStats();
	////////////////////////////////////////////////
	
	/**
	   This functions calls the input manager.
	*/
	void CallInputManager();

	/**
	   This functions calls the physics manager.
	*/
	void CallPhysicsManager();

	/**
	   This functions adds job to the job queue.
	*/
	void AddJob(std::function <void(void)>);
	void LoadL();
	void invoke();

	bool Frame(float rdeltaTime);
	/**
	Checks which key is pressed or released.
	*/
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
	bool joined = false;
	
	void ResetMemoryManager();
protected:
	std::mutex lock;
	std::condition_variable cv;
	bool shutdown;
	std::queue <std::function <void(void)>> jobs;
	std::vector <std::thread> threads;
	
	/**
	   This functions either runs a job on a particular thread if the job queue is not empty or makes the thread sleep till there is a job. It also makes the threads join the main thread in
	   case there is a shutdown call.
	*/
	void Kick(int);

private:
	
	std::atomic_int IOcheck;
	int thread_counter = 0;
	std::thread ArrayOfThreads[6];
	void (Engine::* JobList[10])() = { NULL };
	unsigned int m_TaskQueueCount = 0;
	void Wait();
	//bool Frame();

	/**
		Initalizes the windows.
	*/
	void InitializeWindows(int&, int&);

	/**
		Shuts down windows
	*/
	void ShutdownWindows();

private:
	LPCSTR mApplicationName;
	HINSTANCE mHinstance;
	HWND mHWND;
	bool mFullScreen;
	bool mAppPaused;
	bool mMinimized;
	bool mMaximized;
	bool mResizing;
	

	unsigned int GTYPE;
	/*GraphicsManager* mGraphics;
	TimerManager* mTimer;
	DXInputManager* mInput;*/
};
/**
	Windows event.
*/
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static Engine* ApplicationHandle = 0;

#endif