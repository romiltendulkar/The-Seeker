/**
 * @file UIWindow.cpp
 * @authors  Romil Tendulkar
 * @date 01/20/2020
 * @brief This is the class to create the window for the UI the UI will be housed
		within this window

 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#include "../Source/pch.h"
#include "UIWindow.h"

UIWindow::UIWindow(int width, int height, std::string name) : 
	UIElement(width, height,name, UIType::UI_WINDOW)
{

}

UIWindow::~UIWindow()
{

}

void UIWindow::Update(float deltatime)
{

}

void UIWindow::Serialize()
{

}