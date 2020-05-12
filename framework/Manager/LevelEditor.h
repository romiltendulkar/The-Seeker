#pragma once
/**
 * @file LevelEditor.h
 * @author Supreeth Rao Pejawar
 * @date 10/31/19
 * @brief This file is used to declare the functions to be implemented in the cpp file.
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */

#include "../Source/pch.h"
#include "MemoryManager.h"
class Body;
class LevelEditor
{
private:
	static LevelEditor* mInstance;
	Body* pBody = nullptr;
public:
	/**
		 Constructor of the level editor which has the member initialised
	*/
	LevelEditor();

	~LevelEditor();

	unsigned int key;

	/**
		Constructor of the level editor which has the member initialised
	*/
	static LevelEditor* GetInstance();
	void hightlightObject(Asset* pAs);

	/**
		This function takes in object name as aparameter and creates objects when it is called
	*/
	void createObject(const char* object, unsigned int i);

	/**
		This function destroys the instance created bt the level editor class
	*/
	void Destroy();


	
	std::map<unsigned int, Json::Value, std::less<unsigned int>> mStartingObjects;
	std::string mSaveFile;
	bool levelEditoris_on, toggleis_on ;
};

