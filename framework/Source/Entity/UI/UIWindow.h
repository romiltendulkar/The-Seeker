/**
 * @file UIWindow.h
 * @authors  Romil Tendulkar
 * @date 01/20/2020
 * @brief This is the class to create the window for the UI the UI will be housed
		within this window

 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */

#pragma once
#include "..\..\pch.h"
#include "UIElement.h"

class UIWindow : public UIElement
{
public:
	UIWindow(int width, int height, std::string name);
	~UIWindow();
	void Update(float deltatime);
	void Serialize();
};