/**
 * @file LevelEditor.cpp
 * @author Supreeth Rao Pejawar
 * @date 10/31/19
 * @brief This file has functions related to creating a level editor
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#include "../Source/pch.h"
#include "LevelEditor.h"
#include"ObjectFactoryManager.h"
#include "..\Source\Components\Body.h"



LevelEditor::LevelEditor()
{
	levelEditoris_on = true;
	key = -1;
}

LevelEditor::~LevelEditor()
{
}


LevelEditor* LevelEditor::GetInstance()
{
	if (!mInstance)
	{
		mInstance = new LevelEditor();
	}
	return mInstance;
}

void LevelEditor::hightlightObject(Asset* pAs)
{
	pAs->obj->SetDiffuse(0, 0, 0);
}


void LevelEditor::createObject(const char* object, unsigned int i)
{
	//ObjectFactoryManager::GetInstance()->CreateRunTimeObject(object, pBody);

}


void LevelEditor::Destroy()
{
	delete mInstance;
}
