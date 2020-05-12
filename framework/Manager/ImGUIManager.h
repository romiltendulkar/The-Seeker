/**
 * @file ImGUIManager.h
 * @authors Pratyush Gawai
 * @date 10/2/19
 * @brief This file contains methods to draw Imgui debug display.
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */

#ifndef IMGUI_MANAGER_H
#define IMGUI_MANAGER_H

#include "../Source/pch.h"

#include"LevelEditor.h"
//imgui files to include
#include "../Dependencies/ImGUI/imgui.h"
#include "../Dependencies/ImGUI/imgui_impl_win32.h"
#include "../Dependencies/ImGUI/imgui_impl_dx11.h"

class ImGUIManager
{
public:

	ImGUIManager();
	~ImGUIManager();
	ImGUIManager(const ImGUIManager &obj) {}

	static ImGUIManager* GetInstance();

	/**
		This function initializes Imgui context.
	*/
	void Initialize(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* context);//call this in engine init

	/**
		Destroys ImGui context.
	*/
	void Destroy();
	void Update(float rdeltaTime);

	/**
		Draws ImGui windows.
	*/
	void DrawWindows();//call this in graphics manager
	void DrawDebugWindows();
	void DrawLevelEditorWindow();
	void Resize(int width, int height);
	/**
		Draws a symbol where it is called and displays string when hovered over it, used to create hints.
	*/
	static void HelpMarker(const char* desc);
	void DrawCharStats();
	void DrawCompletion();
	void ImGUIManager::NeedCompletion();

	//void PrefabList();
	bool show_demo_window = false;
	bool show_another_window = false;
	bool english;
	bool portuguese;
	std::string filename;
	ImGuiIO io;
	Json::Value mSelectedAdd;

	int width, height;
	int selectedkey;
private:
	static ImGUIManager* mInstance;
	std::string mPrefabPath = "Resource/levels/";
};

#endif
	 