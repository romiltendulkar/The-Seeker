/**
 * @file ResourceManager.h
 * @authors Pratyush Gawai
 * @date 9/14/2019
 * @brief This file has methods to load JSON files, 3D models and 2D textures.
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#pragma once
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "../Source/pch.h"
#include "../../Dependencies/jsoncpp/json/json.h"


class Shape;
class DXTexture;
class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	
	static ResourceManager* GetInstance();

	void Destroy();

	/**
	This function loads the basic 3D model or complex 3D model based on the parameters specified
	shapeName = Defines the shape
	modelName = Defines whats the complex model name.
	*/
	Shape* LoadShape(std::string shapeName,std::string modelName);

	/**
		This function loads JSON file. If it exists, give back the exist copy, else Create the JSON data,parse it and load it
		jsonName = Takes the input file.
	*/
	Json::Value LoadJson(std::string jsonName);

	/**
	This function loads texture file. If it exists, give back the exist copy, else Create the JSON data,parse it and load it
	texName = Takes the input file.
	*/
	DXTexture* LoadTexture(std::string texName);

	std::unordered_map<std::string, Shape*>LShape;
	std::unordered_map<std::string, Json::Value>LJsonData;
	std::unordered_map<std::string, DXTexture*>LTexture;
	/**
This function parses the JSON data and returns the root
jsonName = file to be loaded.
*/
	Json::Value CreateJsonData(std::string jsonName);
private:
	static ResourceManager* mInstance;


};

#endif