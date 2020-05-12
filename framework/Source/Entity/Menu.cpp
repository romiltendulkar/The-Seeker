#include "../pch.h"
#include "Menu.h"

#include "../../Manager/DXInputManager.h"
#include "../Components/Transform.h"
#include "../Components/Asset.h"
#include "../Components/Body.h"
#include "../../Manager/UIManager.h"

Menu::Menu() : Entity(ENTITY_TYPE::MENU)
{

}

Menu::~Menu() {}

void Menu::Serialize(Json::Value root, unsigned int position)
{
	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn = this;
	Json::Value dataRoot = root["Transform"];

	//Transform
	if (!dataRoot.isNull())
	{
		Transform* pTr = new Transform();
		pTr->Serialize(dataRoot, position);
	}

	//Asset
	dataRoot = root["Asset"];

	if (!dataRoot.isNull())
	{
		Asset* pAs = new Asset();
		pAs->materialID = -1;
		pAs->Serialize(dataRoot, position);
		this->pAs = pAs;
	}

	//Body
	dataRoot = root["Body"];

	if (!dataRoot.isNull())
	{
		Body* pBd = new Body();
		pBd->Serialize(dataRoot, position);
		this->pBd = pBd;
	}

	//Animation
	dataRoot = root["Animation"];

	if (!dataRoot.isNull())
	{
		Animation* pAn = new Animation();
		pAn->Serialize(dataRoot, position);
		this->pAn = pAn;
	}

	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn = this;
	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn->mEntityName = "Menu";
	key = position;
	this->complete = true;
	UIManager::GetInstance()->LoadFullScreen(nullptr, 0);
	UIManager::GetInstance()->LoadMainMenu();
	UIManager::GetInstance()->IOShowDialogue(0);
	UIManager::GetInstance()->IOShowHP(0);
	UIManager::GetInstance()->IOShowIngameMenu(0);
	UIManager::GetInstance()->IOShowMain(true);

}

void Menu::Update(float deltaTime)
{

}