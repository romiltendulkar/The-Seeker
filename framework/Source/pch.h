/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: pch.h
Purpose: Pre-compiled header.
Platform: Windows 10, x64
Project: romil.tendulkar, GAM550
Author: Romil Tendulkar, romil.tendulkar, 60001218
Creation date: 22/09/2019
- End Header --------------------------------------------------------*/

#pragma once


//data structures

#include <windows.h>
#include <stdlib.h>
#include <stdint.h>
#include <exception>
#include <stdio.h>
#include <sstream>
#include<thread>
#include<mutex>
#include<atomic>
#include <condition_variable>
#include <list>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include<queue>
#include<functional>
#include <random>
#include <string> 
#include <cstring>
#include <time.h> 

#include "CommonStates.h"
#include "DDSTextureLoader.h"
#include "DirectXHelpers.h"
#include "Effects.h"
#include "GamePad.h"
#include "GeometricPrimitive.h"
#include "GraphicsMemory.h"
#include "Keyboard.h"
#include "Model.h"
#include "Mouse.h"
#include <dinput.h>
#include "PostProcess.h"
#include "PrimitiveBatch.h"
#include "ScreenGrab.h"
#include "SimpleMath.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "VertexTypes.h"
#include "WICTextureLoader.h"
#include "Graphics\ReadData.h"


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>



#include <ft2build.h>
#include FT_FREETYPE_H

using namespace DirectX;
using namespace SimpleMath;

std::wstring StringtoWstring(std::string in);

#define DEGREE_TO_RADIAN 0.0174532925f
#define RELEASE_COM(x) { if(x){ x->Release(); x = 0; } }
#define GBUFFER_TEXTURE_SLOT_BEGIN 0
#define SHADOW_TEXTURE_SLOT_BEGIN 6
#define COLOR_TEXTURE_SLOT_BEGIN 7

#define WIN32_LEAN_AND_MEAN
#define INPUT_IDLE 0
#define INPUT_STARTED 1
#define INPUT_TAKEN 2
#define N_FUNCTIONS = 2;//number of functions that I can have in my job pool.

//#define DIRECTINPUT_VERSION 0x0800
#pragma comment(lib, "dinput8.lib")


