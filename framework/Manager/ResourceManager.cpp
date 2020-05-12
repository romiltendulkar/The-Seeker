/**
 * @file ResourceManager.h
 * @authors Pratyush Gawai
 * @date 9/14/2019
 * @brief This file has methods to load JSON files, 3D models and 2D textures.
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */

#include "../Source/pch.h"
#include "ResourceManager.h"
#include "GraphicsManager.h"
#include "../Source/Graphics/Shape.h"
#include "../Source/Graphics/DXTexture.h"


//load all resources : shape, textures, materials, sounds
#define RADIUS 32 //Default radius size -  smoothest size anything higher than this makes no difference
ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
	for (auto &x : LShape)
	{
		delete x.second;
	}
	LShape.clear();
}

ResourceManager * ResourceManager::GetInstance()
{
	if (!mInstance)
	{
		mInstance = new ResourceManager();
	}
	return mInstance;
}

void ResourceManager::Destroy()
{
	delete mInstance;
}

Shape * ResourceManager::LoadShape(std::string shapeName,std::string modelName)
{
	Shape* mpShape = nullptr;
	if (shapeName == "Obj" || shapeName == "FBX")
	{
		mpShape = LShape[modelName];
	}
	else
	{
		mpShape = LShape[shapeName];
	}


	if (mpShape)
	{
		return mpShape;//if it exist, grabs it from the map, and return it
	}

	if (mpShape==nullptr)
	{
		if (shapeName == "Triangle")
		{
			Triangle* T = new Triangle();
			mpShape = T;
			LShape[shapeName] = mpShape;

		}
		if (shapeName == "Quad")
		{
			Quad* Q = new Quad();
			mpShape = Q;
			LShape[shapeName] = mpShape;
		}
		if (shapeName == "Cylinder")
		{
			Cylinder* C = new Cylinder(RADIUS);
			mpShape = C;
			LShape[shapeName] = mpShape;

		}
		if (shapeName == "Disk")
		{
			Disk* D = new Disk(RADIUS);
			mpShape = D;
			LShape[shapeName] = mpShape;
		}
		if (shapeName == "Sphere")
		{
			Sphere* S = new Sphere(RADIUS);
			mpShape = S;
			LShape[shapeName] = mpShape;
		}
		if (shapeName == "Box")
		{
			Box* B = new Box(VEC3(1,1,1),VEC3(-1,-1,-1));
			mpShape = B;
			LShape[shapeName] = mpShape;
		}
		if (shapeName == "Torus")
		{
			Torus* B = new Torus(RADIUS);
			mpShape = B;
			LShape[shapeName] = mpShape;
		}
		if (shapeName == "Obj")
		{
			std::string modelfile = "./Resource/Model/"+modelName;   //./Resource/Model/" + shapeName; this is file+model name
			OBJ* O = new OBJ(modelfile);
			mpShape = O;
			LShape[modelName] = mpShape;
		}
		if (shapeName == "FBX")
		{
			std::string modelfile = "./Resource/Model/" + modelName;   //./Resource/Model/" + shapeName; this is file+model name
			FBX* O = new FBX(modelfile);
			mpShape = O;
			LShape[modelName] = mpShape;
		}
		if (shapeName == "Particles")
		{
			Particles* P = new Particles();
			mpShape = P;
			LShape[shapeName] = mpShape;
		}
		
	}
	return mpShape;
}

Json::Value ResourceManager::LoadJson(std::string jsonName)
{
	Json::Value jsonData = LJsonData[jsonName];

	if (!jsonData.isNull())
	{
		return jsonData;
	}

	jsonData = CreateJsonData(jsonName);

	if (jsonData)
	{
		LJsonData[jsonName] = jsonData;
	}

	return jsonData;
}

DXTexture * ResourceManager::LoadTexture(std::string texName)
{
	DXTexture* mtex = nullptr;
	mtex = LTexture[texName];

	if (mtex)
	{
		return mtex;
	}
	
	if (mtex == nullptr)
	{
		std::string texpath = "./Resource/Image/Textures/" + texName;
		std::wstring finaltexpath = StringtoWstring(texpath);
		mtex = new DXTexture();
		mtex->Initialize(GraphicsManager::GetInstance()->GetDirectXHandler()->GetDevice(), GraphicsManager::GetInstance()->GetDirectXHandler()->GetDeviceContext(), finaltexpath.c_str());
	}


	return mtex;
}

Json::Value ResourceManager::CreateJsonData(std::string jsonName)
{
	std::string filePath = "./Resource/Levels/";
	filePath += jsonName+".json";

	std::ifstream file(filePath);
	std::string fileString;
	std::getline(file, fileString, (char)EOF);

	Json::CharReaderBuilder builder;
	Json::CharReader* reader = builder.newCharReader();
	Json::Value root;

	std::string errorString;

	if (!reader->parse(fileString.c_str(), fileString.c_str() + fileString.size(), &root, &errorString))
	{
		std::cout << "Error: " << errorString << std::endl;
	}
	/*if (!file)
	{
		std::cout << "Unable to open file" << filePath << std::endl;
		return nullptr;
	}
*/
	//Get data from JSON file
	//std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	return root;

}

