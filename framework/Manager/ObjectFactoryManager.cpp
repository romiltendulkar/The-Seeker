#include "../Source/pch.h"
#include "ObjectFactoryManager.h"

#include<math.h>

#include "../Manager/ResourceManager.h"

#include "../Source/Components/Component.h"

#include "../Source/Components/Asset.h"
#include "../Source/Components/Transform.h"
#include "../Source/Components/Controller.h"
#include "../Source/Components/Animation.h"
#include "../Source/Components/LightDetail.h"

#include "../Source/Entity/Player.h"
#include "../Source/Entity/Bullet.h"
#include "../Source/Entity/EBullet.h"
#include "../Source/Entity/Platform.h"
#include "../Source/Entity/MovingPlatform.h"
#include "../Source/Entity/Intro.h"
#include "../Source/Entity/Pathfinder.h"

#include "../Source/Entity/LP.h"
#include "../Source/Entity/RP.h"
#include "../Source/Entity/Boss2.h"
#include "../Source/Entity/Stopper.h"
#include "../Source/Entity/Hint.h"
#include "../Source/Entity/ConditionBox.h"
#include "../Source/Entity/Collider.h"
#include "../Source/Entity/Floor.h"
#include "../Source/Entity/Door.h"
#include "../Source/Entity/Block.h"
#include "../Source/Entity/Enemy.h"
#include "../Source/Entity/Right.h"
#include "../Source/Entity/Left.h"
#include "../Source/Entity/HealthBar.h"
#include "../Source/Entity/TimeBar.h"
#include "../Source/Entity/Hint.h"
#include "../Source/Entity/Credits.h"
#include "../Source/Entity/Entity.h"
#include "../Source/Entity/ViewBox.h"
#include "../Source/Entity/SpotBox.h"
#include "../Source/Entity/Padding.h"
#include "../Source/Entity/Gate.h"
#include "../Source/Entity/WEnemy.h"
#include "../Source/Entity/Seeker.h"
#include "../Source/Entity/SeekerBox.h"
#include "../Source/Entity/SpotLight.h"
#include "../Source/Entity/DirectLight.h"
#include "../Source/Entity/PointLight.h"
#include "../Source/Entity/Guidebox.h"
#include "../Source/Entity/PuzzleTile.h"
#include "../Source/Entity/Shooter.h"
#include "../Source/Entity/Charger.h"
#include "../Source/Entity/Pillar.h"
#include "../Source/Entity/Switch.h"
#include "../Source/Entity/Boss1.h"
#include "../Source/Entity/LootDrop.h"
#include "../Source/Entity/Exploder.h"
#include "../Source/Entity/Visible_Box.h"
#include "../Source/Entity/LevelSwitch.h"
#include "../Source/Entity/Menu.h"
#include "../Source/Entity/Tree.h"
#include "../Manager/MemoryManager.h"
#include "ImGUIManager.h"
#include "AudioManager.h"

ObjectFactoryManager::ObjectFactoryManager() 
{
	LEntityNames.push_back("Block");
	LEntityNames.push_back("Boss1");
	LEntityNames.push_back("Charger");
	LEntityNames.push_back("Collider");
	LEntityNames.push_back("DirectLight");
	LEntityNames.push_back("Floor");
	LEntityNames.push_back("Guidebox");
	LEntityNames.push_back("LootDrop");
	LEntityNames.push_back("Pillar");
	LEntityNames.push_back("Platform");
	LEntityNames.push_back("PointLight");
	LEntityNames.push_back("PuzzleTile");
	LEntityNames.push_back("Shooter");
	LEntityNames.push_back("SpotLight");
	LEntityNames.push_back("Switch");
	LEntityNames.push_back("Padding");
	LEntityNames.push_back("Hint");
	LEntityNames.push_back("Stopper");
	LEntityNames.push_back("MovingPlatform");
	LEntityNames.push_back("Tree");
}

ObjectFactoryManager::~ObjectFactoryManager() {}

ObjectFactoryManager* ObjectFactoryManager::GetInstance()
{
	if (!mInstance)
	{
		mInstance = new ObjectFactoryManager();
	}
	return mInstance;
}

void ObjectFactoryManager::LoadLevel(std::string pFileName)
{
	PhysicsManager::GetInstance()->lock_found = 0;
	LWallPositions.clear();
	LLightPositions.clear();
	LBoxPositions.clear();
	LPTPositions.clear();
	LPillarPositions.clear();
	LEnemyPositions.clear();
	LDynamicList.clear();

	if (pFileName == "Level3")
	{
		mMenuAudio = false;
		if (!mAudiostarted)
		{
			AudioManager::GetInstance()->LoadLoopingAudio("Background.wav");
			mAudiostarted = true;
		}
		else
		{
			AudioManager::GetInstance()->Stop();
			AudioManager::GetInstance()->LoadLoopingAudio("Background.wav");
		}
		
		
	}
	else if (pFileName == "Level4")
	{
		mMenuAudio = false;
		if (!mAudiostarted)
		{
			AudioManager::GetInstance()->LoadLoopingAudio("Combat.wav");
			mAudiostarted = true;
		}
		else
		{
			AudioManager::GetInstance()->Stop();
			AudioManager::GetInstance()->LoadLoopingAudio("Combat.wav");
		}
	}
	else
	{
		if (!mMenuAudio)
		{
			mMenuAudio = true;
			if (!mAudiostarted)
			{
				AudioManager::GetInstance()->LoadLoopingAudio("Town music.wav");
				mAudiostarted = true;

			}
			else
			{
				AudioManager::GetInstance()->Stop();
				AudioManager::GetInstance()->LoadLoopingAudio("Town music.wav");
			}
		}		
	}

	unsigned int position = MemoryManager::GetInstance()->Allocate();
	Json::Value root = ResourceManager::GetInstance()->LoadJson(pFileName)[pFileName];

	//if valid JSON file
	if (!root.isNull())
	{
		for (unsigned int i = 0; i < root.size(); ++i)
		{
			//Get the gameobject file
			Json::Value objectRoot = root[i];
			int curKey = MemoryManager::GetInstance()->current_position;

			//Check the datafile
			Json::Value dataFile = objectRoot["DataFile"];
			if (!dataFile.isNull())
			{
				std::string fileroot = dataFile.asString();
				if (fileroot == "Boss2")
				{
					MemoryManager::GetInstance()->bosscout++;
				}
				LoadObject(dataFile.asString(), curKey);
				ReSerialize(objectRoot, curKey);
			}
		}
	}
	int player_x, player_y;
	LEnemyPositions.clear();
	for (unsigned int i = 0; i < MemoryManager::GetInstance()->current_position; i++)
	{
		if ((*MemoryManager::GetInstance()->ObjectPool)[i].pEn->GetEntityType() == ENTITY_TYPE::PLAYER)
		{
			player_x = (*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mPos_x;
			player_y = (*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mPos_y;
			MemoryManager::GetInstance()->maincharacher = i;
			continue;
		}
		if ((*MemoryManager::GetInstance()->ObjectPool)[i].pEn->GetEntityType() == ENTITY_TYPE::GUIDEBOX)
		{
			guide = i;
			continue;
		}
		if ((*MemoryManager::GetInstance()->ObjectPool)[i].pEn->GetEntityType() == ENTITY_TYPE::FLOOR)
		{
			floor_scale_x = (*MemoryManager::GetInstance()->ObjectPool)[i].pTr->mScaleX;
			floor_scale_y = (*MemoryManager::GetInstance()->ObjectPool)[i].pTr->mScaleY;
			floor_x = (*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mPos_x - floor_scale_x / 2;
			floor_y = (*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mPos_y - floor_scale_y / 2;
			continue;
		}
		if ((*MemoryManager::GetInstance()->ObjectPool)[i].pEn->GetEntityType() == ENTITY_TYPE::SHOOTER ||
			(*MemoryManager::GetInstance()->ObjectPool)[i].pEn->GetEntityType() == ENTITY_TYPE::CHARGER ||
			(*MemoryManager::GetInstance()->ObjectPool)[i].pEn->GetEntityType() == ENTITY_TYPE::BOSS1 ||
			(*MemoryManager::GetInstance()->ObjectPool)[i].pEn->GetEntityType() == ENTITY_TYPE::BOSS2)
		{
			LEnemyPositions.emplace_back(i);
			continue;
		}
		if ((*MemoryManager::GetInstance()->ObjectPool)[i].pEn->GetEntityType() == ENTITY_TYPE::PLATFORM)
		{
			LWallPositions.emplace_back(i);
			continue;
		}
		if ((*MemoryManager::GetInstance()->ObjectPool)[i].pEn->GetEntityType() == ENTITY_TYPE::PATHFINDER)
		{
			Pathfinder_pos = i;
			continue;
		}
		if ((*MemoryManager::GetInstance()->ObjectPool)[i].pEn->GetEntityType() == ENTITY_TYPE::SPOTBOX)
		{
			LLightPositions.emplace_back(i);
			continue;
		}
		if ((*MemoryManager::GetInstance()->ObjectPool)[i].pEn->GetEntityType() == ENTITY_TYPE::SEEKERBOX)
		{
			LBoxPositions.emplace_back(i);
			continue;
		}
		if ((*MemoryManager::GetInstance()->ObjectPool)[i].pEn->GetEntityType() == ENTITY_TYPE::PUZZLETILE)
		{
			LPTPositions.emplace_back(i);
			continue;
		}
		if ((*MemoryManager::GetInstance()->ObjectPool)[i].pEn->GetEntityType() == ENTITY_TYPE::PILLAR)
		{
			LPillarPositions.emplace_back(i);
			continue;
		}
		if ((*MemoryManager::GetInstance()->ObjectPool)[i].pEn->GetEntityType() == ENTITY_TYPE::DIRECT_LIGHT
			|| (*MemoryManager::GetInstance()->ObjectPool)[i].pEn->GetEntityType() == ENTITY_TYPE::POINT_LIGHT
			|| (*MemoryManager::GetInstance()->ObjectPool)[i].pEn->GetEntityType() == ENTITY_TYPE::SPOT_LIGHT)
		{
			MemoryManager::GetInstance()->lightList.push_back(i);
			continue;
		}
	}
	//create the grid from the floor position.
	MemoryManager::GetInstance()->map = new grid *[floor_scale_x];
	for (int i = 0; i < floor_scale_y; ++i)//left to right
	{
		MemoryManager::GetInstance()->map[i] = new grid[floor_scale_y];
	}
	for (int i = 0; i < floor_scale_x; ++i)
	{
		for (int j = 0; j < floor_scale_y; ++j)
		{
			MemoryManager::GetInstance()->map[i][j] = { i,j,false };
		}
	}

	std::vector<unsigned int>::iterator itr = ObjectFactoryManager::GetInstance()->LWallPositions.begin();
	while (itr != ObjectFactoryManager::GetInstance()->LWallPositions.end())
	{
		int i = (*MemoryManager::GetInstance()->ObjectPool)[(*itr)].pBd->mPos_x;
		int j = (*MemoryManager::GetInstance()->ObjectPool)[(*itr)].pBd->mPos_y;
		float scale_i = (*MemoryManager::GetInstance()->ObjectPool)[(*itr)].pBd->mScale_x;
		float scale_j = (*MemoryManager::GetInstance()->ObjectPool)[(*itr)].pBd->mScale_y;
		i = i - scale_i / 2;//leftmost
		j = j - scale_j / 2;//bottom-most
		for (int y = j; y < j + scale_j; ++y)
		{
			for (int x = i; x < i + scale_i; ++x)
			{
				(MemoryManager::GetInstance()->map[y][x]).is_wall = true;
			}
		}
		++itr;
	}
	MemoryManager::GetInstance()->proxy_map = new node *[floor_scale_x];
	for (int i = 0; i < floor_scale_y; ++i)//left to right
	{
		MemoryManager::GetInstance()->proxy_map[i] = new node[floor_scale_y];
	}
	MemoryManager::GetInstance()->mfloor_scale_y = floor_scale_y;
	MemoryManager::GetInstance()->mfloor_scale_x = floor_scale_x;

}


void ObjectFactoryManager::LoadObject(std::string pFileName, unsigned int key)
{

	unsigned int position = key;
	Json::Value root = ResourceManager::GetInstance()->LoadJson(pFileName);

	if (!root.isNull())
	{
		Json::Value objectRoot = root["Player"];

		if (!objectRoot.isNull())
		{
			Player* pl = new Player();
			pl->Serialize(objectRoot, position);
		}

		objectRoot = root["Bullet"];

		if (!objectRoot.isNull())
		{
			Bullet* bl = new Bullet();
			bl->Serialize(objectRoot, position);
		}

		objectRoot = root["EBullet"];

		if (!objectRoot.isNull())
		{
			EBullet* bl = new EBullet();
			bl->Serialize(objectRoot, position);
		}

		objectRoot = root["Guidebox"];

		if (!objectRoot.isNull())
		{
			Guidebox* bl = new Guidebox();
			bl->Serialize(objectRoot, position);
		}
		objectRoot = root["Hint"];

		if (!objectRoot.isNull())
		{
			Hint* bl = new Hint();
			bl->Serialize(objectRoot, position);
		}

		objectRoot = root["Platform"];
		if (!objectRoot.isNull())
		{
			Platform* pl = new Platform();
			pl->Serialize(objectRoot, position);
		}

		objectRoot = root["Block"];
		if (!objectRoot.isNull())
		{
			Block* pl = new Block();
			pl->Serialize(objectRoot, position);
		}
		objectRoot = root["Door"];
		if (!objectRoot.isNull())
		{
			Door* pl = new Door();
			pl->Serialize(objectRoot, position);
		}
		objectRoot = root["LP"];
		if (!objectRoot.isNull())
		{
			LP* pl = new LP();
			pl->Serialize(objectRoot, position);
		}
		objectRoot = root["RP"];
		if (!objectRoot.isNull())
		{
			RP* pl = new RP();
			pl->Serialize(objectRoot, position);
		}
		objectRoot = root["Collider"];
		if (!objectRoot.isNull())
		{
			Collider* pl = new Collider();
			pl->Serialize(objectRoot, position);
		}
		objectRoot = root["Floor"];
		if (!objectRoot.isNull())
		{
			Floor* pl = new Floor();
			pl->Serialize(objectRoot, position);
		}
		objectRoot = root["Stopper"];
		if (!objectRoot.isNull())
		{
			Stopper* pl = new Stopper();
			pl->Serialize(objectRoot, position);
		}
		objectRoot = root["MovingPlatform"];
		if (!objectRoot.isNull())
		{
			MovingPlatform* pl = new MovingPlatform();
			pl->Serialize(objectRoot, position);
		}
		objectRoot = root["SeekerBox"];
		if (!objectRoot.isNull())
		{
			SeekerBox* pl = new SeekerBox();
			pl->Serialize(objectRoot, position);
		}
		objectRoot = root["ConditionBox"];
		if (!objectRoot.isNull())
		{
			ConditionBox* pl = new ConditionBox();
			pl->Serialize(objectRoot, position);
		}
		objectRoot = root["Pathfinder"];
		if (!objectRoot.isNull())
		{
			Pathfinder* pl = new Pathfinder();
			pl->Serialize(objectRoot, position);
		}
		objectRoot = root["Pillar"];
		if (!objectRoot.isNull())
		{
			Pillar* cat = new Pillar();
			cat->Serialize(objectRoot, position);
		}
		objectRoot = root["Boss1"];
		if (!objectRoot.isNull())
		{
			Boss1* hel = new Boss1();
			hel->Serialize(objectRoot, position);
		}
		objectRoot = root["Boss2"];
		if (!objectRoot.isNull())
		{
			Boss2* hel = new Boss2();
			hel->Serialize(objectRoot, position);
		}
		objectRoot = root["Seeker"];
		if (!objectRoot.isNull())
		{
			Seeker* hel = new Seeker();
			hel->Serialize(objectRoot, position);
		}
		objectRoot = root["Enemy"];
		if (!objectRoot.isNull())
		{
			Enemy* fp = new Enemy();
			fp->Serialize(objectRoot, position);
		}
		objectRoot = root["WEnemy"];
		if (!objectRoot.isNull())
		{
			WEnemy* fp = new WEnemy();
			fp->Serialize(objectRoot, position);
		}
		objectRoot = root["ViewBox"];
		if (!objectRoot.isNull())
		{
			ViewBox* fp = new ViewBox();
			fp->Serialize(objectRoot, position);
		}
		objectRoot = root["Hint"];
		if (!objectRoot.isNull())
		{
			Hint* fp = new Hint();
			fp->Serialize(objectRoot, position);
		}
		objectRoot = root["SpotBox"];
		if (!objectRoot.isNull())
		{
			SpotBox* fp = new SpotBox();
			fp->Serialize(objectRoot, position);
		}
		objectRoot = root["Right"];
		if (!objectRoot.isNull())
		{
			Right* fp = new Right();
			fp->Serialize(objectRoot, position);
		}
		objectRoot = root["Left"];
		if (!objectRoot.isNull())
		{
			Left* fp = new Left();
			fp->Serialize(objectRoot, position);
		}
		objectRoot = root["HealthBar"];
		if (!objectRoot.isNull())
		{
			HealthBar* fp = new HealthBar();
			fp->Serialize(objectRoot, position);
		}


		objectRoot = root["SpotLight"];
		if (!objectRoot.isNull())
		{
			SpotLight* fp = new SpotLight();
			fp->Serialize(objectRoot, position);
		}
		objectRoot = root["DirectLight"];
		if (!objectRoot.isNull())
		{
			DirectLight* fp = new DirectLight();
			fp->Serialize(objectRoot, position);
		}

		objectRoot = root["PointLight"];
		if (!objectRoot.isNull())
		{
			PointLight* fp = new PointLight();
			fp->Serialize(objectRoot, position);
		}
		objectRoot = root["PuzzleTile"];
		if (!objectRoot.isNull())
		{
			PuzzleTile* fp = new PuzzleTile();
			fp->Serialize(objectRoot, position);
		}
		objectRoot = root["TimeBar"];
		if (!objectRoot.isNull())
		{
			TimeBar* fp = new TimeBar();
			fp->Serialize(objectRoot, position);
		}
		objectRoot = root["Switch"];
		if (!objectRoot.isNull())
		{
			Switch* fp = new Switch();
			fp->Serialize(objectRoot, position);
		}
		objectRoot = root["Visible_Box"];
		if (!objectRoot.isNull())
		{
			Visible_Box* fp = new Visible_Box();
			fp->Serialize(objectRoot, position);
		}
		objectRoot = root["Shooter"];
		if (!objectRoot.isNull())
		{
			Shooter* fp = new Shooter();
			fp->Serialize(objectRoot, position);
		}
		objectRoot = root["Padding"];
		if (!objectRoot.isNull())
		{
			Padding* fp = new Padding();
			fp->Serialize(objectRoot, position);
		}
		objectRoot = root["Charger"];
		if (!objectRoot.isNull())
		{
			Charger* fp = new Charger();
			fp->Serialize(objectRoot, position);
		}
		objectRoot = root["LootDrop"];
		if (!objectRoot.isNull())
		{
			LootDrop* fp = new LootDrop();
			fp->Serialize(objectRoot, position);
		}
		objectRoot = root["Exploder"];
		if (!objectRoot.isNull())
		{
			Exploder* fp = new Exploder();
			fp->Serialize(objectRoot, position);
		}
		objectRoot = root["Gate"];
		if (!objectRoot.isNull())
		{
			Gate* fp = new Gate();
			fp->Serialize(objectRoot, position);
		}
		objectRoot = root["Menu"];
		if (!objectRoot.isNull())
		{
			Menu* fp = new Menu();
			fp->Serialize(objectRoot, position);
		}
		objectRoot = root["Credits"];
		if (!objectRoot.isNull())
		{
			Credits* fp = new Credits();
			fp->Serialize(objectRoot, position);
		}
		objectRoot = root["Intro"];
		if (!objectRoot.isNull())
		{
			Intro* fp = new Intro();
			fp->Serialize(objectRoot, position);
		}
		objectRoot = root["LevelSwitch"];
		if (!objectRoot.isNull())
		{
			LevelSwitch* fp = new LevelSwitch();
			fp->Serialize(objectRoot, position);
		}
		objectRoot = root["Tree"];
		if (!objectRoot.isNull())
		{
			Tree* fp = new Tree();
			fp->Serialize(objectRoot, position);
		}
		MemoryManager::GetInstance()->current_position++;
	}
}


void ObjectFactoryManager::ReSerialize(Json::Value objectRoot, unsigned int key)
{
	//Transform
	Json::Value transform = objectRoot["Transform"];
	if (!transform.isNull())
	{
		(*MemoryManager::GetInstance()->ObjectPool)[key].pTr->Serialize(transform, key);

		if ((*MemoryManager::GetInstance()->ObjectPool)[key].pBd)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mPos_x = (*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mPositionX;
			(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mPos_y = (*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mPositionY;
			(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mPos_z = (*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mPositionZ;

		/*	(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mScale_x = (*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mScaleX;
			(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mScale_y = (*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mScaleY;
			(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mScale_z = (*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mScaleZ;*/



			/*if ((*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mShape->type == SHAPE_AABB)
			{
				ShapeAABB* newShape = static_cast<ShapeAABB*>((*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mShape);

				newShape->mBottom = (0 - (*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mScale_z) / 2;
				newShape->mTop = (0 + (*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mScale_z) / 2;

				newShape->mIn = (0 + (*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mScale_y) / 2;
				newShape->mOut = (0 - (*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mScale_y) / 2;

				newShape->mLeft = (0 - (*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mScale_x) / 2;
				newShape->mRight = (0 + (*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mScale_x) / 2;
			}*/
		}
		/*pBd->mPos_x = pTr->mPositionX;
		pBd->mPos_y = pTr->mPositionY;
		pBd->mPos_z = pTr->mPositionZ;*/
		//}
		//reserialize body to reset the body position and shape


	/*	if (pBd)
		{
			pBd->Serialize(objectRoot, key);
		}*/
	}
	//Body
	Json::Value body = objectRoot["Body"];
	if (!body.isNull())
	{
		(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->Serialize(body, key);
		/*if (pBd)
		{
			pBd->Serialize(body, key);
		}*/
	}
	//Asset
	Json::Value asset = objectRoot["Asset"];
	if (!asset.isNull())
	{
		Asset* pAs = (*MemoryManager::GetInstance()->ObjectPool)[key].pAs;

		(*MemoryManager::GetInstance()->ObjectPool)[key].pAs->Serialize(asset, key);

		Json::Value dataRoot = asset["Diffuse"];
		if (!dataRoot.isNull())
		{
			(*MemoryManager::GetInstance()->ObjectPool)[key].pAs->obj->SetDiffuse(dataRoot[0].asFloat(), dataRoot[1].asFloat(), dataRoot[2].asFloat());

		}


	}
	//Controller
	Json::Value contr = objectRoot["Controller"];
	if (!contr.isNull())
	{
		Controller* pCr = (*MemoryManager::GetInstance()->ObjectPool)[key].pCr;
		if (pCr)
		{
			pCr->Serialize(contr, key);
		}
	}
	//Asset
	//Json::Value asset = objectRoot["Asset"];
	//if (!asset.isNull())
	//{
	//	Asset* pAs = (*MemoryManager::GetInstance()->ObjectPool)[key].pAs;
	//	if (pAs)
	//	{
	//		pAs->Serialize(asset, key);
	//	}
	//}
	//Animation
	Json::Value anim = objectRoot["Animation"];
	if (!anim.isNull())
	{
		Animation* pAn = (*MemoryManager::GetInstance()->ObjectPool)[key].pAn;
		if (pAn)
		{
			pAn->Serialize(anim, key);
		}
	}
	Json::Value ligdetail = objectRoot["LightDetail"];
	if (!ligdetail.isNull())
	{
		LightDetail* pLd = (*MemoryManager::GetInstance()->ObjectPool)[key].pLd;
		if (pLd)
		{
			pLd->Serialize(ligdetail, key);
		}
	}
	Json::Value hintS = objectRoot["HintStr"];
	if (!hintS.isNull())
	{
		Hint* hint = static_cast<Hint*>((*MemoryManager::GetInstance()->ObjectPool)[key].pEn);
		hint->hstring = hintS.asString();
	}
	Json::Value CBP = objectRoot["ConditionBoxPos"];
	if (!CBP.isNull())
	{
		Door* cb = static_cast<Door*>((*MemoryManager::GetInstance()->ObjectPool)[key].pEn);
		cb->CBoxPos.x = CBP[0].asFloat();
		cb->CBoxPos.y = CBP[1].asFloat();
		cb->CBoxPos.z = CBP[2].asFloat();
		cb->CBoxScale.x = CBP[3].asFloat();
		cb->CBoxScale.y = CBP[4].asFloat();
		cb->CBoxScale.z = CBP[5].asFloat();
		cb->CBoxType = CBP[6].asInt();
		cb->Enemy_Required = CBP[7].asInt();
	}
	Json::Value CBP1 = objectRoot["MovementType"];
	if (!CBP1.isNull())
	{
		MovingPlatform* cb = static_cast<MovingPlatform*>((*MemoryManager::GetInstance()->ObjectPool)[key].pEn);
		cb->type = CBP1[0].asInt();
		cb->speed = CBP1[1].asFloat();
	}
	Json::Value nxtlevel = objectRoot["NextLevel"];
	if (!nxtlevel.isNull())
	{
		std::string lvl = nxtlevel.asString();
		MemoryManager::GetInstance()->mpOwner->mNextLevel = lvl;
	}
	
	Json::Value levelname = objectRoot["LevelName"];
	if (!levelname.isNull())
	{
		std::string lvl = levelname.asString();
		MemoryManager::GetInstance()->mpOwner->mNextLevel = lvl;
		auto s = static_cast<LevelSwitch*>((*MemoryManager::GetInstance()->ObjectPool)[key].pEn);
		s->mLevelName = lvl;
	}
}
void ObjectFactoryManager::FireBullet(Body* ptr, int curKey, int enemy_key)
{
	if (enemy_key > 0)
	{
		(*MemoryManager::GetInstance()->ObjectPool)[curKey].pTr->mPositionX = ptr->mPos_x + 1;
		(*MemoryManager::GetInstance()->ObjectPool)[curKey].pTr->mPositionY = ptr->mPos_y;
		(*MemoryManager::GetInstance()->ObjectPool)[curKey].pTr->mPositionZ = ptr->mPos_z + 0.25;
		if ((*MemoryManager::GetInstance()->ObjectPool)[curKey].pBd)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[curKey].pBd->mPos_x = ptr->mPos_x + 1;
			(*MemoryManager::GetInstance()->ObjectPool)[curKey].pBd->mPos_y = ptr->mPos_y;
			(*MemoryManager::GetInstance()->ObjectPool)[curKey].pBd->mPos_z = ptr->mPos_z + 0.25;

			float move_x, move_y;
			move_x = (*MemoryManager::GetInstance()->ObjectPool)[enemy_key].pBd->mPos_x - (*MemoryManager::GetInstance()->ObjectPool)[curKey].pBd->mPos_x;
			move_y = (*MemoryManager::GetInstance()->ObjectPool)[enemy_key].pBd->mPos_y - (*MemoryManager::GetInstance()->ObjectPool)[curKey].pBd->mPos_y;
			float newRot = atan2(move_y, move_x);
			//newRot = acos(newRot) ;
			//std::cout << end_x << ", " << end_y << ", " << newRot << std::endl;
			(*MemoryManager::GetInstance()->ObjectPool)[curKey].pBd->mVelo_y = 5.0f * sin(newRot);
			(*MemoryManager::GetInstance()->ObjectPool)[curKey].pBd->mVelo_x = 5.0f * cos(newRot);
			(*MemoryManager::GetInstance()->ObjectPool)[curKey].pBd->mVelo_z = 0;
			(*MemoryManager::GetInstance()->ObjectPool)[curKey].pBd->mAcc_z = 0;

		}
	}
}
void ObjectFactoryManager::DropBox(const char* pFileName)
{
	srand((unsigned int)time(0));
	int curKey = MemoryManager::GetInstance()->current_position;
	LoadObject(pFileName, curKey);
	(*MemoryManager::GetInstance()->ObjectPool)[curKey].pTr->mPositionX = -17.0f;
	(*MemoryManager::GetInstance()->ObjectPool)[curKey].pTr->mPositionY = 0.0f;
	(*MemoryManager::GetInstance()->ObjectPool)[curKey].pTr->mPositionZ = 100.0f;
	(*MemoryManager::GetInstance()->ObjectPool)[curKey].pBd->mPos_x = -17.0f;
	(*MemoryManager::GetInstance()->ObjectPool)[curKey].pBd->mPos_y = 0.0f;
	(*MemoryManager::GetInstance()->ObjectPool)[curKey].pBd->mPos_z = 100.0f;
	(*MemoryManager::GetInstance()->ObjectPool)[curKey].pAs->obj->SetDiffuse(float(rand() % 100) / 100.0f, float(rand() % 100) / 100.0f, float(rand() % 100) / 100.0f);
}

void ObjectFactoryManager::CreateAmmoPack()
{
	start = MemoryManager::GetInstance()->current_position;
	//create ammo from key to key+9, so 10 fireballs.
	for (int i = 0; i < 21; i++)
	{
		int curKey = MemoryManager::GetInstance()->current_position;
		LoadObject("Bullet", curKey);
		//(*MemoryManager::GetInstance()->ObjectPool)[curKey].pBd->active = false;
	}
}


void ObjectFactoryManager::DeleteObject()
{
	if (MemoryManager::GetInstance()->current_position > 1)
		MemoryManager::GetInstance()->Free();
}


void ObjectFactoryManager::Update(float rdeltaTime)
{
}

void ObjectFactoryManager::Destroy()
{
	delete mInstance;
}
void ObjectFactoryManager::SaveLevel()
{
	Json::StreamWriterBuilder builder;
	Json::StreamWriter* writer = builder.newStreamWriter();


	unsigned int numObj = MemoryManager::GetInstance()->current_position;
	std::unordered_map<unsigned, MemoryManager::ObjectNode> mObjPool = (*MemoryManager::GetInstance()->ObjectPool);
	Json::Value root;
	std::string filename = ImGUIManager::GetInstance()->filename;
	root[filename] = Json::arrayValue;


	for (unsigned int i = 0; i < numObj; ++i)
	{
		if (mObjPool[i].pEn->runtime == false)
		{
			std::string name = mObjPool[i].pEn->mEntityName;

			Json::Value myObj(Json::objectValue);
			myObj["DataFile"] = name;
			if (mObjPool[i].pTr)
			{
				mObjPool[i].pTr->DeSerialize(myObj);
			}
			//if (mObjPool[i].pAs)
			//{
			//	mObjPool[i].pAs->DeSerialize(myObj);
			//}
			//if (mObjPool[i].pBd)
			//{
			//	mObjPool[i].pBd->DeSerialize(myObj);
			//}

			root[filename].append(myObj);
		}
	}
	std::ofstream outputFile("Resource/Levels/" + filename + ".json");
	writer->write(root, &outputFile);
	outputFile.close();
}

int ObjectFactoryManager::CreateRunTimeObject(const char* pFileName)
{
	int curKey = MemoryManager::GetInstance()->current_position;

	LDynamicList.emplace_back(curKey);

	LoadObject(pFileName, curKey);
	(*MemoryManager::GetInstance()->ObjectPool)[curKey].pTr->mPositionX = 10;
	(*MemoryManager::GetInstance()->ObjectPool)[curKey].pTr->mPositionY = 10;
	(*MemoryManager::GetInstance()->ObjectPool)[curKey].pTr->mPositionZ = 10;
	if ((*MemoryManager::GetInstance()->ObjectPool)[curKey].pBd)
	{
		(*MemoryManager::GetInstance()->ObjectPool)[curKey].pBd->mPos_x = 10;
		(*MemoryManager::GetInstance()->ObjectPool)[curKey].pBd->mPos_y = 10;
		(*MemoryManager::GetInstance()->ObjectPool)[curKey].pBd->mPos_z = 10;
	}
	/*if (ptr->facing_up == true)
	{
		(*MemoryManager::GetInstance()->ObjectPool)[curKey].pBd->velo_y = 5;
	}
	if (ptr->facing_down == true)
	{
		(*MemoryManager::GetInstance()->ObjectPool)[curKey].pBd->velo_y = -5;
	}
	if (ptr->facing_right == true)
	{
		(*MemoryManager::GetInstance()->ObjectPool)[curKey].pBd->velo_x = 5;
	}
	if (ptr->facing_left == true)
	{
		(*MemoryManager::GetInstance()->ObjectPool)[curKey].pBd->velo_x = -5;
	}*/
	return curKey;
}