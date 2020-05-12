/**
 * @file Engine.cpp
 * @authors Ching-Yen Lin, Pratyush Gawai, Vineet Amarjeet Dogra
 * @date 9/1/19
 * @brief Encapsulates all managers.
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */

#include "pch.h"
#include "Engine.h"


#include "../Manager/GraphicsManager.h"
#include "../Manager/TimerManager.h"
#include "../Manager/DXInputManager.h"
#include "../Manager/GameObjectManager.h"
#include "../Manager/ResourceManager.h"

#include "../Manager/PhysicsManager.h"
#include "../Manager/GameObjectManager.h"
#include "../Manager/ImGUIManager.h"
#include "../Manager/ObjectFactoryManager.h"
#include "../Manager/CollisionManager.h"
#include "../Manager/LevelEditor.h"
#include "../Manager/AudioManager.h"
#include "../Manager/UIManager.h"
#include "../Manager/ParticleManager.h"
#include "GameObject.h"
#include "../Source/GameStates/GameState.h"
#include "../Manager/GameStateManager.h"
#include "GameStates/PlayState.h"
#include "../Manager/EventManager.h"
#include "../Manager/PuzzleManager.h"

Engine::Engine() {
	mAppPaused = false;
	mMinimized = false;
	mMaximized = false;
	mResizing = false;
}

std::thread* InputManagerThread = nullptr;
mutex mu;
mutex Array_Locker;
std::condition_variable cond;

//GameStateManager* pGSM;
PlayState* statetemp;



Engine::~Engine() 
{}
GraphicsManager* GraphicsManager::mInstance = nullptr;
DXInputManager* DXInputManager::mInstance = nullptr;
GameObjectManager* GameObjectManager::mInstance = nullptr;
ResourceManager* ResourceManager::mInstance = nullptr;
TimerManager* TimerManager::mInstance = nullptr;
ObjectFactoryManager* ObjectFactoryManager::mInstance = nullptr;
MemoryManager* MemoryManager::mInstance = nullptr;
PhysicsManager* PhysicsManager::mInstance = nullptr;
CollisionManager* CollisionManager::mInstance = nullptr;
LevelEditor* LevelEditor::mInstance = nullptr;
ImGUIManager* ImGUIManager::mInstance = nullptr;
AudioManager* AudioManager::mInstance = nullptr;
EventManager* EventManager::mInstance = nullptr;
UIManager* UIManager::mInstance = nullptr;
PuzzleManager* PuzzleManager::mInstance = nullptr;
ParticleManager* ParticleManager::mInstance = nullptr;
GameStateManager* GameStateManager::mInstance = nullptr;
#define WINDOWS_WIDTH_SIZE 16
#define WINDOWS_HEIGHT_SIZE 39
#define WINDOWS_WIDTH_SIZE 16
#define WINDOWS_HEIGHT_SIZE 39


/**
	Initializes all the managers
*/
bool Engine::Initialize(int type)
{
	GTYPE = type;
	//pGSM = new GameStateManager(this);
	statetemp = new PlayState(GTYPE);
	/*if (AllocConsole())
	{
		FILE* file;
		freopen_s(&file, "CONOUT$", "wt", stdout);
		freopen_s(&file, "CONOUT$", "wt", stderr);
		freopen_s(&file, "CONOUT$", "wt", stdin);

	}*/
	//Engine::IOcheck.store(INPUT_IDLE);
	Init_Threads(1);//create 4 threads.
	int screenWidth, screenHeight;
	bool result;

	screenWidth = 0;
	screenHeight = 0;

	InitializeWindows(screenWidth, screenHeight);
	if (TimerManager::GetInstance() == nullptr ||
		GraphicsManager::GetInstance() == nullptr ||
		DXInputManager::GetInstance() == nullptr ||
		GameObjectManager::GetInstance() == nullptr ||
		ObjectFactoryManager::GetInstance() == nullptr ||
		ResourceManager::GetInstance() == nullptr ||
		PhysicsManager::GetInstance() == nullptr ||
		MemoryManager::GetInstance() == nullptr ||
		CollisionManager::GetInstance() == nullptr ||
		ImGUIManager::GetInstance() == nullptr ||
		LevelEditor::GetInstance() == nullptr ||
		AudioManager::GetInstance() == nullptr ||
		EventManager::GetInstance() == nullptr||
		PuzzleManager::GetInstance() == nullptr ||
		UIManager::GetInstance() == nullptr ||
		GameStateManager::GetInstance() == nullptr ||
		ParticleManager::GetInstance() == nullptr)

	{
		return false;
	}
	GameStateManager::GetInstance()->Initialize(this);
	RECT d;
	GetClientRect(mHWND, &d);
	DXInputManager::GetInstance()->Initialize(mHinstance, mHWND, d.right, d.bottom);
	GraphicsManager::GetInstance()->FULL_SCREEN = true;
	result = GraphicsManager::GetInstance()->Initialize(d.right, d.bottom, mHWND);
	
	if (!result)
	{
		return false;
	}
	UIManager::GetInstance()->Initialize();
	
	ParticleManager::GetInstance()->Initialize(GraphicsManager::GetInstance()->GetDirectXHandler()->GetDevice(), GraphicsManager::GetInstance()->GetDirectXHandler()->GetDeviceContext());
	//ObjectFactoryManager::GetInstance()->LoadLevel("Level1");

	ImGUIManager::GetInstance()->Initialize(mHWND, GraphicsManager::GetInstance()->GetDirectXHandler()->GetDevice(), GraphicsManager::GetInstance()->GetDirectXHandler()->GetDeviceContext());

	//Engine::AddJob(std::bind(&Engine::LoadL, this));

	//if (type == 0)
	//{
	//	GameStateManager::GetInstance()->PushState(statetemp, "Level1");
	//}
	//if (type == 1)
	//{
	//	GameStateManager::GetInstance()->PushState(statetemp, "Level2");
	//}
	//if (type == 2)
	//{
	//	GameStateManager::GetInstance()->PushState(statetemp, "Level3");
	//}
	//GameStateManager::GetInstance()->PushState(statetemp, "Level3");

	GameStateManager::GetInstance()->PushState(statetemp, "IntroLevel");

	return true;
}

/**
   This functions initializes the thread vector.
*/
void Engine::Init_Threads(int n)
{
	threads.reserve(n);//reserve how many threads you want to use.
	for (int i = 0; i < n; ++i)
	{
		threads.emplace_back(std::bind(&Engine::Kick, this, i));//I am using 'i' just to see which thread is being executed.
	}
}

//void Engine::invoke()
//{
//	Engine::IOcheck.store(INPUT_STARTED);
//	if (Engine::IOcheck.load() != INPUT_IDLE)
//	{
//		InputManagerThread = new std::thread(&Engine::CallInputManager, this);
//	}
//	Engine::IOcheck.store(INPUT_TAKEN);
//		//InputManagerThread.join();
//}

/**
	Frees all the managers.
*/
void Engine::Shutdown()
{

	/*while (!states.empty()) {
		states.top()->CleanUp();
		states.pop();
	}*/

	{
		std::unique_lock<std::mutex> l(lock);
		shutdown = true;
		cv.notify_all();//wake em all up, shutdown is in effect.
	}
	for (auto& thread : threads)
	{
		thread.join();
	}


	GraphicsManager::GetInstance()->Destroy();
	TimerManager::GetInstance()->Destroy();
	DXInputManager::GetInstance()->Destroy();
	GameObjectManager::GetInstance()->Destroy();
	ObjectFactoryManager::GetInstance()->Destroy();
	PhysicsManager::GetInstance()->Destroy();
	ResourceManager::GetInstance()->Destroy();
	MemoryManager::GetInstance()->Destroy();
	ImGUIManager::GetInstance()->Destroy();
	LevelEditor::GetInstance()->Destroy();
	AudioManager::GetInstance()->Stop();
	UIManager::GetInstance()->Destroy();
	GameStateManager::GetInstance()->Destroy();
	// Shutdown the window.
	ShutdownWindows();

	return;
}
/**
	Update loop for the engine.
*/
void Engine::Run()
{
	
	GameStateManager::GetInstance()->Draw();
	return;
	
}
/**
	Calculates frame stats.
*/
void Engine::CalculateFrameStats()
{
	static int frameCout = 0;
	static float timeElapsed = 0.0f;
	frameCout++;
	if ((TimerManager::GetInstance()->getTotalTime() - timeElapsed) >= 0.0f)
	{
		float fps = (float)frameCout;
		float mspf = 1000.0f / fps;
		std::ostringstream outs;
		outs.precision(6);
		outs << GraphicsManager::GetInstance()->GetDeviceName() << " "
			<< "FPS: " << fps << "    "
			<< "Frame Time: " << mspf << " (ms)";
		std::string s(outs.str());
		SetWindowText(mHWND, s.c_str());
		frameCout = 0;
		timeElapsed += 1.0f;
	}
}


/**
   This functions adds job to the job queue.
*/
void Engine::AddJob(std::function <void(void)> func)
{
	std::unique_lock <std::mutex> l(lock);//place a job in the job queue.
	jobs.emplace(std::move(func));
	cv.notify_one();
}

/**
   This functions either runs a job on a particular thread if the job queue is not empty or makes the thread sleep till there is a job. It also makes the threads join the main thread in 
   case there is a shutdown call.
*/

void Engine::Kick(int n)
{
	std::cout << "Thread NUMBER: " << n << " is executing" << std::endl;
	std::function<void(void)> job;
	while (1)//za infinite loop.
	{
		{
			std::unique_lock<std::mutex>l(lock);
			while (jobs.empty())//dont shutdown but there are no jobs, so sleep and wait.
			{
				if (shutdown)
				{
					break;
				}
				else
				{
					cv.wait(l);
				}
			}
			if (jobs.empty()) //nothing to do, stop here.
			{
				return;
			}
			//if we have a job to do:
			job = std::move(jobs.front());
			jobs.pop();//remove the job after shifting the function pointer to "job".
		}
		job();//execute the job on THIS thread, the thread that called the kick function.
	}
}
/**
   This functions calls the input manager.
*/
void Engine::CallInputManager()
{
	if (!LevelEditor::GetInstance()->toggleis_on)
	{
		DXInputManager::GetInstance()->Update(deltaTime);
	}
}
/**
   This functions calls the physics manager.
*/
void Engine::CallPhysicsManager()
{
	PhysicsManager::GetInstance()->Update(deltaTime);
}
void Engine::LoadL()
{
	ObjectFactoryManager::GetInstance()->LoadLevel("sss");
}
bool Engine::Frame(float rdeltaTime)
{
	bool result;
	//Engine::AddJob(std::bind(&Engine::CallInputManager, this));
	//Engine::AddJob(std::bind(&Engine::CallPhysicsManager, this));
	PhysicsManager::GetInstance()->Update(deltaTime);
	
	//DXInputManager::GetInstance()->Update();
	//PhysicsManager::GetInstance()->Update();
	// Do the frame processing for the graphics object.
	//result = GraphicsManager::GetInstance()->Update(rdeltaTime);
	//CalculateFrameStats();
	/*if (!result)
	{
		return false;
	}*/

	return true;
}
/**
	Checks which key is pressed or released.
*/
LRESULT CALLBACK Engine::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		// Check if a key has been pressed on the keyboard.
	case WM_KEYDOWN:
	{
		// If a key is pressed send it to the input object so it can record that state.
		return 0;
	}

	// Check if a key has been released on the keyboard.
	case WM_KEYUP:
	{
		// If a key is released then send it to the input object so it can unset the state for that key.
		return 0;
	}
	case WM_SYSKEYDOWN:
	{
		if (wparam == 0xd)
		{
			GraphicsManager::GetInstance()->FULL_SCREEN = !GraphicsManager::GetInstance()->FULL_SCREEN;
		}
		break;
	}
	case WM_ACTIVATE:
	{
		if(WA_INACTIVE == wparam)
			if (GameStateManager::GetInstance()->pCurrentState)
			{
				GameStateManager::GetInstance()->pCurrentState->windowspause = 1;
				//cout << "!!!!!\n";
			}
		//if(WA_ACTIVE == wparam)
		//	if (GameStateManager::GetInstance()->pCurrentState)
		//	{
		//		cout << "????\n";
		//		GameStateManager::GetInstance()->pCurrentState->windowspause = 0;
		//	}
		break;
	}
	case WM_SIZE:
	{
		 int screen_width=LOWORD(lparam);
		 int screen_height = HIWORD(lparam);
		 if (GraphicsManager::GetInstance()->GetDirectXHandler())
		 {
			 if (GraphicsManager::GetInstance()->GetDirectXHandler()->GetDevice())
			 {
				 if (wparam == SIZE_MINIMIZED)
				 {
					 mAppPaused = true;
					 mMinimized = true;
					 mMaximized = false;
				 }
				 else if (wparam == SIZE_MAXIMIZED)
				 {
					 mAppPaused = false;
					 mMinimized = false;
					 mMaximized = true;
					 GraphicsManager::GetInstance()->Resize(screen_width, screen_height);
					 ImGUIManager::GetInstance()->Resize(screen_width, screen_height);
					 UIManager::GetInstance()->Resize(screen_width, screen_height);
					 DXInputManager::GetInstance()->SetScreenSize(screen_width, screen_height);

				 }
				 else if (wparam == SIZE_RESTORED)
				 {

					 // Restoring from minimized state?
					 if (mMinimized)
					 {
						 mAppPaused = false;
						 mMinimized = false;
						 GraphicsManager::GetInstance()->Resize(screen_width, screen_height);
						 ImGUIManager::GetInstance()->Resize(screen_width, screen_height);
						 UIManager::GetInstance()->Resize(screen_width, screen_height);
						 DXInputManager::GetInstance()->SetScreenSize(screen_width, screen_height);

					 }

					 // Restoring from maximized state?
					 else if (mMaximized)
					 {
						 mAppPaused = false;
						 mMaximized = false;
						 GraphicsManager::GetInstance()->Resize(screen_width, screen_height);
						 ImGUIManager::GetInstance()->Resize(screen_width, screen_height);
						 UIManager::GetInstance()->Resize(screen_width, screen_height);
						 DXInputManager::GetInstance()->SetScreenSize(screen_width, screen_height);

					 }
					 else if (mResizing)
					 {
					 }
					 else // API call such as SetWindowPos or mSwapChain->SetFullscreenState.
					 {
						 GraphicsManager::GetInstance()->Resize(screen_width, screen_height);
						 ImGUIManager::GetInstance()->Resize(screen_width, screen_height);
						 UIManager::GetInstance()->Resize(screen_width, screen_height);
						 DXInputManager::GetInstance()->SetScreenSize(screen_width, screen_height);

					 }
				 }
			 }
		 }
		 return 0;
	}
	/*case WM_ACTIVATEAPP:
	{
		if (wparam == FALSE)
		{
			GameStateManager::GetInstance()->pCurrentState->windowspause = 1;
		}
		else if(wparam == TRUE)
		{
			GameStateManager::GetInstance()->pCurrentState->windowspause = 0;
		}
		break;
	}*/
	// Any other messages send to the default message handler as our application won't make use of them.
	default:
	{
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
	}
}
/**
	Initalizes the windows.
*/
void Engine::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wcex;
	DEVMODE dmScreenSettings;
	int result = SetProcessDPIAware();
	int posX, posY;
	wcex.cbSize = sizeof(WNDCLASSEX);

	// Get an external pointer to this object.
	ApplicationHandle = this;

	// Get the instance of this application.
	mHinstance = GetModuleHandle(NULL);

	// Give the application a name.
	mApplicationName = "The Seeker";

	// Setup the windows class with default settings.
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = mHinstance;
	wcex.hIcon = LoadIcon(mHinstance, IDI_WINLOGO);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.lpszMenuName = IDI_WINLOGO;
	wcex.lpszClassName = mApplicationName;
	wcex.hIconSm = wcex.hIcon;



	// Register the window class.
	RegisterClassEx(&wcex);

	// Determine the resolution of the clients desktop screen.
	//display max size 
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);
	mFullScreen = true;
	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (mFullScreen)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenWidth = 800;
		screenHeight = 600;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	mHWND = CreateWindowEx(WS_EX_APPWINDOW, mApplicationName, mApplicationName,
		WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,
		posX, posY, screenWidth, screenHeight, NULL, NULL, mHinstance, NULL);
	//WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(mHWND, SW_SHOW);
	SetForegroundWindow(mHWND);
	SetFocus(mHWND);

	// Hide the mouse cursor.
	//ShowCursor(false);

	return;
}
/**
	Shuts down windows
*/
void Engine::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if (mFullScreen)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(mHWND);
	mHWND = NULL;

	// Remove the application instance.
	UnregisterClass(mApplicationName, mHinstance);
	mHinstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;

	return;
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam); //extern call to not mess up default windows settings 
/**
	Windows event.
*/
LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	/*if (ImGui_ImplWin32_WndProcHandler(hwnd, umessage, wparam, lparam))
		return true;*/

	switch (umessage)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}
	default:
	{
		ImGui_ImplWin32_WndProcHandler(hwnd, umessage, wparam, lparam);
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
	}
}

void Engine::ResetMemoryManager()
{
	MemoryManager::GetInstance()->Destroy();

	MemoryManager::GetInstance();
}
