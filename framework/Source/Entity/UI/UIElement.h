/**
 * @file UIElement.h
 * @authors  Romil Tendulkar
 * @date 01/20/2020
 * @brief This is the base class for the various UI Elements that might be created

 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */

#pragma once

#include "..\..\pch.h"
#include "..\Entity.h"

class Body;
class Transform;

enum class UIType
{
	UI_WINDOW
};

class UIElement : public Entity
{
public:
	UIElement() : Entity(ENTITY_TYPE::UIELEMENT)
	{
		mHeight = mWidth = 0;
		ElementName = "Unnamed UI Element";
	}
	UIElement(int width, int height, std::string name, UIType type) : Entity(ENTITY_TYPE::UIELEMENT)
	{
		mWidth = width;
		mHeight = height;
		ElementName = name;
		mElementType = type;
	}
	~UIElement();

	virtual void Update(float Deltatime);
	UIType GetUIType()
	{
		return mElementType;
	}


	/**
		This function changes the name of the UI Element

		\param name | string for the new name of the element
	*/
	void ChangeName(std::string name)
	{
		ElementName = name;
	}

	/**
		This function changes the width and height of the UI Element

		\param width | the new width of the UI element
		\param height | the new height of the UI element
	*/
	void ChangeDimensions(int width, int height)
	{
		mWidth = width;
		mHeight = height;
	}

public:
	int mWidth, mHeight;
	std::string  ElementName;
	UIType mElementType;
	Body* pBd;
	Transform* pTr;
};