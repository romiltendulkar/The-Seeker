#include "../pch.h"
#include "Bullet.h"
#include "../Components/Transform.h"
#include "../Components/Asset.h"
#include "../Components/Body.h"
#include "../../Manager/ParticleManager.h"
#include "..\Events\CollisionEvent.h"
#include "Player.h"
#include "Charger.h"
#include "Shooter.h"
#include "Boss1.h"
#include "Boss2.h"


Bullet::Bullet() : Entity(ENTITY_TYPE::BULLET)
{
	key = -1;
	pBd = nullptr;
}

Bullet::~Bullet() {

}

void Bullet::Serialize(Json::Value root, unsigned int position)
{
	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn = this;
	Json::Value dataRoot = root["Transform"];

	//Transform
	if (!dataRoot.isNull())
	{
		if (pTr) delete pTr;
		Transform* pTr = new Transform();
		pTr->Serialize(dataRoot, position);
		this->pTr = pTr;
	}

	//Asset
	dataRoot = root["Asset"];
	
	if (!dataRoot.isNull())
	{
		Asset* pAs = new Asset();
		pAs->Serialize(dataRoot, position);
		pAs->obj->SetDiffuse(1, 1, 1);
		pAs->obj->SetMaterialID(7);
		(*MemoryManager::GetInstance()->ObjectPool)[position].pAs = pAs;
	}

	//Body
	dataRoot = root["Body"];

	if (!dataRoot.isNull())
	{
		if (pBd) delete pBd;
		Body* pBd = new Body();
		pBd->Serialize(dataRoot, position);
		this->pBd = pBd;
	}

	//Animation
	dataRoot = root["Animation"];

	if (!dataRoot.isNull())
	{
		if (pAn) delete pAn;
		Animation* pAn = new Animation();
		pAn->Serialize(dataRoot, position);
		this->pAn = pAn;
	}
	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn = this;

	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn->mEntityName = "Bullet";
	key = position;
	this->complete = true;
	this->runtime = true;
	
}

void Bullet::Update(float deltaTime)
{
	if (pBd->mVelo_x != 0.0f || pBd->mVelo_y != 0.0f)
	{
		framecounter++;
	}
	if (framecounter >= 120)
	{
		framecounter = 0;
		pBd->active = false;
		pBd->mVelo_x = 0.0f;
		pBd->mVelo_y = 0.0f;
		pTr->mScaleX = 0.0f;
		pTr->mScaleY = 0.0f;
		pTr->mScaleZ = 0.0f;
	}
	if (pBd->active)
	{
		pTr->mScaleX = 0.3f;
		pTr->mScaleY = 0.3f;
		pTr->mScaleZ = 0.3f;
	}
	if (!pBd->active)
	{
		pTr->mScaleX = 0.0f;
		pTr->mScaleY = 0.0f;
		pTr->mScaleZ = 0.0f;
	}
	if (pBd)
		pBd->Update(deltaTime);
	if (pAn)
		pAn->Update(deltaTime);
}

void Bullet::HandleEvent(Event* pEventToHandle)
{
	if (pEventToHandle)
	{
		if (pEventToHandle->mEventType == EVENT_TYPE::COLLIDE_EVENT)
		{
			CollisionEvent* pEvent = static_cast<CollisionEvent*>(pEventToHandle);
			ENTITY_TYPE b1 = pEvent->mpBody1->mpEnowner->GetEntityType();
			ENTITY_TYPE b2 = pEvent->mpBody2->mpEnowner->GetEntityType();

			Player* play;
			if ((*MemoryManager::GetInstance()->ObjectPool)[2].pEn->mEntityType == ENTITY_TYPE::PLAYER)
			{
				play = static_cast<Player*>((*MemoryManager::GetInstance()->ObjectPool)[2].pEn);
			}
			else
			{
				return;
			}

			if (b1 == ENTITY_TYPE::BULLET)
			{
				if (b2 == ENTITY_TYPE::CHARGER || b2 == ENTITY_TYPE::BOSS1 || b2 == ENTITY_TYPE::BOSS2 ||b2 == ENTITY_TYPE::SHOOTER)
				{
					float top = 0.0f, bottom = 0.0f, left = 0.0f, right = 0.0f, in = 0.0f, out = 0.0f;
					left = abs((pEvent->mpBody2->mPos_x + pEvent->mpBody2->mWidth) - (pEvent->mpBody1->mPos_x - pEvent->mpBody1->mWidth));
					right = abs((pEvent->mpBody2->mPos_x - pEvent->mpBody2->mWidth) - (pEvent->mpBody1->mPos_x + pEvent->mpBody1->mWidth));
					in = abs((pEvent->mpBody2->mPos_y + pEvent->mpBody2->mDepth) - (pEvent->mpBody1->mPos_y - pEvent->mpBody1->mDepth));
					out = abs((pEvent->mpBody2->mPos_y - pEvent->mpBody2->mDepth) - (pEvent->mpBody1->mPos_y + pEvent->mpBody1->mDepth));
					
					pEvent->mpBody1->active = false;

					if (right < left && right < in && right < out)//condition for right
					{
						pEvent->mpBody2->mPos_x += 0.5;
						if (b2 == ENTITY_TYPE::CHARGER)
						{
							Charger* enemy = static_cast<Charger*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody2->key].pEn);
							enemy->health -= 2;
							if (play->mOnePunchMan)
							{
								enemy->health = 0;
							}
							if (enemy->health <= 0)
							{
								pEvent->mpBody2->active = false;
								pTr->mScaleX = 0;
								pTr->mScaleY = 0;
								pTr->mScaleZ = 0;
							}
							{
								Particle temp;
								temp.age = 0.7;
								temp.position = Vector4(enemy->pBd->mPos_x, enemy->pBd->mPos_y, enemy->pBd->mPos_z, 0.5);
								temp.TextureIndex = 0;
								temp.velocity = Vector4(3, 3, 3, 0.5);
								ParticleManager::GetInstance()->SphereEmit(40, temp);
							}
						}
						else if (b2 == ENTITY_TYPE::BOSS1)
						{
							pEvent->mpBody2->mPos_x -= 0.5;
							Boss1* enemy = static_cast<Boss1*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody2->key].pEn);

							enemy->health -= 2;
							if (play->mOnePunchMan)
							{
								enemy->health = 0;
							}
							if (enemy->health <= 0)
							{
								pEvent->mpBody2->active = false;
								pTr->mScaleX = 0;
								pTr->mScaleY = 0;
								pTr->mScaleZ = 0;
							}
							{
								Particle temp;
								temp.age = 0.7;
								temp.position = Vector4(enemy->pBd->mPos_x, enemy->pBd->mPos_y, enemy->pBd->mPos_z, 0.5);
								temp.TextureIndex = 0;
								temp.velocity = Vector4(3, 3, 3, 0.5);
								ParticleManager::GetInstance()->SphereEmit(40, temp);
							}
						}
						else if (b2 == ENTITY_TYPE::BOSS2)
						{
							pEvent->mpBody2->mPos_x -= 0.5;
							Boss2* enemy = static_cast<Boss2*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody2->key].pEn);

							enemy->health -= 2;
							if (play->mOnePunchMan)
							{
								enemy->health = 0;
							}
							if (enemy->health <= 0)
							{
								pEvent->mpBody2->active = false;
								pTr->mScaleX = 0;
								pTr->mScaleY = 0;
								pTr->mScaleZ = 0;
							}
							{
								Particle temp;
								temp.age = 0.7;
								temp.position = Vector4(enemy->pBd->mPos_x, enemy->pBd->mPos_y, enemy->pBd->mPos_z, 0.5);
								temp.TextureIndex = 0;
								temp.velocity = Vector4(3, 3, 3, 0.5);
								ParticleManager::GetInstance()->SphereEmit(40, temp);
							}
						}
						else if (b2 == ENTITY_TYPE::SHOOTER)
						{
							Shooter* enemy = static_cast<Shooter*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody2->key].pEn);

							enemy->health -= 50;
							if (play->mOnePunchMan)
							{
								enemy->health = 0;
							}
							if (enemy->health <= 0)
							{
								pEvent->mpBody2->active = false;
								pTr->mScaleX = 0;
								pTr->mScaleY = 0;
								pTr->mScaleZ = 0;
							}
							{
								Particle temp;
								temp.age = 0.7;
								temp.position = Vector4(enemy->pBd->mPos_x, enemy->pBd->mPos_y, enemy->pBd->mPos_z, 0.5);
								temp.TextureIndex = 0;
								temp.velocity = Vector4(3, 3, 3, 0.5);
								ParticleManager::GetInstance()->SphereEmit(40, temp);
							}
						}
						
					}
					else if (in < right && in < left && in < out)
					{
						pEvent->mpBody2->mPos_y -= 0.5;

						if(b2 == ENTITY_TYPE::CHARGER)
						{
							Charger* enemy = static_cast<Charger*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody2->key].pEn);

							enemy->health -= 2;
							if (play->mOnePunchMan)
							{
								enemy->health = 0;
							}
							if (enemy->health <= 0)
							{
								pEvent->mpBody2->active = false;
								pTr->mScaleX = 0;
								pTr->mScaleY = 0;
								pTr->mScaleZ = 0;
							}
							{
								Particle temp;
								temp.age = 0.7;
								temp.position = Vector4(enemy->pBd->mPos_x, enemy->pBd->mPos_y, enemy->pBd->mPos_z, 0.5);
								temp.TextureIndex = 0;
								temp.velocity = Vector4(3, 3, 3, 0.5);
								ParticleManager::GetInstance()->SphereEmit(40, temp);
							}
						}
						else if (b2 == ENTITY_TYPE::BOSS1)
						{
							pEvent->mpBody2->mPos_y += 0.5;
							Boss1* enemy = static_cast<Boss1*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody2->key].pEn);

							enemy->health -= 2;
							if (play->mOnePunchMan)
							{
								enemy->health = 0;
							}
							if (enemy->health <= 0)
							{
								pEvent->mpBody2->active = false;
								pTr->mScaleX = 0;
								pTr->mScaleY = 0;
								pTr->mScaleZ = 0;
							}
							{
								Particle temp;
								temp.age = 0.7;
								temp.position = Vector4(enemy->pBd->mPos_x, enemy->pBd->mPos_y, enemy->pBd->mPos_z, 0.5);
								temp.TextureIndex = 0;
								temp.velocity = Vector4(3, 3, 3, 0.5);
								ParticleManager::GetInstance()->SphereEmit(40, temp);
							}
						}
						else if (b2 == ENTITY_TYPE::BOSS2)
						{
							pEvent->mpBody2->mPos_y += 0.5;
							Boss2* enemy = static_cast<Boss2*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody2->key].pEn);

							enemy->health -= 2;
							if (play->mOnePunchMan)
							{
								enemy->health = 0;
							}
							if (enemy->health <= 0)
							{
								pEvent->mpBody2->active = false;
								pTr->mScaleX = 0;
								pTr->mScaleY = 0;
								pTr->mScaleZ = 0;
							}
							{
								Particle temp;
								temp.age = 0.7;
								temp.position = Vector4(enemy->pBd->mPos_x, enemy->pBd->mPos_y, enemy->pBd->mPos_z, 0.5);
								temp.TextureIndex = 0;
								temp.velocity = Vector4(3, 3, 3, 0.5);
								ParticleManager::GetInstance()->SphereEmit(40, temp);
							}
						}
						else if (b2 == ENTITY_TYPE::SHOOTER)
						{
							Shooter* enemy = static_cast<Shooter*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody2->key].pEn);

							enemy->health -= 50;
							if (play->mOnePunchMan)
							{
								enemy->health = 0;
							}
							if (enemy->health <= 0)
							{
								pEvent->mpBody2->active = false;
								pTr->mScaleX = 0;
								pTr->mScaleY = 0;
								pTr->mScaleZ = 0;
							}
							{
								Particle temp;
								temp.age = 0.7;
								temp.position = Vector4(enemy->pBd->mPos_x, enemy->pBd->mPos_y, enemy->pBd->mPos_z, 0.5);
								temp.TextureIndex = 0;
								temp.velocity = Vector4(3, 3, 3, 0.5);
								ParticleManager::GetInstance()->SphereEmit(40, temp);
							}
						}
					
						
					}
					else if (out < right && out < left && out < in)
					{
						pEvent->mpBody2->mPos_y += 0.5;

						if (b2 == ENTITY_TYPE::CHARGER)
						{
							Charger* enemy = static_cast<Charger*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody2->key].pEn);

							enemy->health -= 2;
							if (play->mOnePunchMan)
							{
								enemy->health = 0;
							}
							if (enemy->health <= 0)
							{
								pEvent->mpBody2->active = false;
								pTr->mScaleX = 0;
								pTr->mScaleY = 0;
								pTr->mScaleZ = 0;
							}
							{
								Particle temp;
								temp.age = 0.7;
								temp.position = Vector4(enemy->pBd->mPos_x, enemy->pBd->mPos_y, enemy->pBd->mPos_z, 0.5);
								temp.TextureIndex = 0;
								temp.velocity = Vector4(3, 3, 3, 0.5);
								ParticleManager::GetInstance()->SphereEmit(40, temp);
							}
						}
						else if (b2 == ENTITY_TYPE::BOSS1)
						{
							pEvent->mpBody2->mPos_y -= 0.5;
							Boss1* enemy = static_cast<Boss1*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody2->key].pEn);

							enemy->health -= 2;
							if (play->mOnePunchMan)
							{
								enemy->health = 0;
							}
							if (enemy->health <= 0)
							{
								pEvent->mpBody2->active = false;
								pTr->mScaleX = 0;
								pTr->mScaleY = 0;
								pTr->mScaleZ = 0;
							}
							{
								Particle temp;
								temp.age = 0.7;
								temp.position = Vector4(enemy->pBd->mPos_x, enemy->pBd->mPos_y, enemy->pBd->mPos_z, 0.5);
								temp.TextureIndex = 0;
								temp.velocity = Vector4(3, 3, 3, 0.5);
								ParticleManager::GetInstance()->SphereEmit(40, temp);
							}
						}
						else if (b2 == ENTITY_TYPE::BOSS2)
						{
							pEvent->mpBody2->mPos_y -= 0.5;
							Boss2* enemy = static_cast<Boss2*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody2->key].pEn);

							enemy->health -= 2;
							if (play->mOnePunchMan)
							{
								enemy->health = 0;
							}
							if (enemy->health <= 0)
							{
								pEvent->mpBody2->active = false;
								pTr->mScaleX = 0;
								pTr->mScaleY = 0;
								pTr->mScaleZ = 0;
							}
							{
								Particle temp;
								temp.age = 0.7;
								temp.position = Vector4(enemy->pBd->mPos_x, enemy->pBd->mPos_y, enemy->pBd->mPos_z, 0.5);
								temp.TextureIndex = 0;
								temp.velocity = Vector4(3, 3, 3, 0.5);
								ParticleManager::GetInstance()->SphereEmit(40, temp);
							}
						}
						else if (b2 == ENTITY_TYPE::SHOOTER)
						{
							Shooter* enemy = static_cast<Shooter*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody2->key].pEn);

							enemy->health -= 50;
							if (play->mOnePunchMan)
							{
								enemy->health = 0;
							}
							if (enemy->health <= 0)
							{
								pEvent->mpBody2->active = false;
								pTr->mScaleX = 0;
								pTr->mScaleY = 0;
								pTr->mScaleZ = 0;
							}
							{
								Particle temp;
								temp.age = 0.7;
								temp.position = Vector4(enemy->pBd->mPos_x, enemy->pBd->mPos_y, enemy->pBd->mPos_z, 0.5);
								temp.TextureIndex = 0;
								temp.velocity = Vector4(3, 3, 3, 0.5);
								ParticleManager::GetInstance()->SphereEmit(40, temp);
							}
						}
						
					}
					else//condition for left
					{
						pEvent->mpBody2->mPos_x -= 0.5;
						if (b2 == ENTITY_TYPE::CHARGER)
						{
							Charger* enemy = static_cast<Charger*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody2->key].pEn);

							enemy->health -= 2;
							if (play->mOnePunchMan)
							{
								enemy->health = 0;
							}
							if (enemy->health <= 0)
							{
								pEvent->mpBody2->active = false;
								pTr->mScaleX = 0;
								pTr->mScaleY = 0;
								pTr->mScaleZ = 0;
							}
							{
								Particle temp;
								temp.age = 0.7;
								temp.position = Vector4(enemy->pBd->mPos_x, enemy->pBd->mPos_y, enemy->pBd->mPos_z, 0.5);
								temp.TextureIndex = 0;
								temp.velocity = Vector4(3, 3, 3, 0.5);
								ParticleManager::GetInstance()->SphereEmit(40, temp);
							}
						}
						else if (b2 == ENTITY_TYPE::BOSS1)
						{
							pEvent->mpBody2->mPos_x += 0.5;
							Boss1* enemy = static_cast<Boss1*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody2->key].pEn);

							enemy->health -= 2;
							if (play->mOnePunchMan)
							{
								enemy->health = 0;
							}
							if (enemy->health <= 0)
							{
								pEvent->mpBody2->active = false;
								pTr->mScaleX = 0;
								pTr->mScaleY = 0;
								pTr->mScaleZ = 0;
							}
							{
								Particle temp;
								temp.age = 0.7;
								temp.position = Vector4(enemy->pBd->mPos_x, enemy->pBd->mPos_y, enemy->pBd->mPos_z, 0.5);
								temp.TextureIndex = 0;
								temp.velocity = Vector4(3, 3, 3, 0.5);
								ParticleManager::GetInstance()->SphereEmit(40, temp);
							}
						}
						else if (b2 == ENTITY_TYPE::BOSS2)
						{
							pEvent->mpBody2->mPos_x += 0.5;
							Boss2* enemy = static_cast<Boss2*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody2->key].pEn);

							enemy->health -= 2;
							if (play->mOnePunchMan)
							{
								enemy->health = 0;
							}
							if (enemy->health <= 0)
							{
								pEvent->mpBody2->active = false;
								pTr->mScaleX = 0;
								pTr->mScaleY = 0;
								pTr->mScaleZ = 0;
							}
							{
								Particle temp;
								temp.age = 0.7;
								temp.position = Vector4(enemy->pBd->mPos_x, enemy->pBd->mPos_y, enemy->pBd->mPos_z, 0.5);
								temp.TextureIndex = 0;
								temp.velocity = Vector4(3, 3, 3, 0.5);
								ParticleManager::GetInstance()->SphereEmit(40, temp);
							}
						}
						else if (b2 == ENTITY_TYPE::SHOOTER)
						{
							Shooter* enemy = static_cast<Shooter*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody2->key].pEn);

							enemy->health -= 50;
							if (play->mOnePunchMan)
							{
								enemy->health = 0;
							}
							if (enemy->health <= 0)
							{
								pEvent->mpBody2->active = false;
								pTr->mScaleX = 0;
								pTr->mScaleY = 0;
								pTr->mScaleZ = 0;
							}
							{
								Particle temp;
								temp.age = 0.7;
								temp.position = Vector4(enemy->pBd->mPos_x, enemy->pBd->mPos_y, enemy->pBd->mPos_z, 0.5);
								temp.TextureIndex = 0;
								temp.velocity = Vector4(3, 3, 3, 0.5);
								ParticleManager::GetInstance()->SphereEmit(40, temp);
							}
						}
					}


				}
			}
			else if (b2 == ENTITY_TYPE::BULLET)
			{
				if (b1 == ENTITY_TYPE::CHARGER || b1 == ENTITY_TYPE::SHOOTER || b1 == ENTITY_TYPE::BOSS1 || b1 == ENTITY_TYPE::BOSS2)
				{
					float top = 0.0f, bottom = 0.0f, left = 0.0f, right = 0.0f, in = 0.0f, out = 0.0f;
					left = abs((pEvent->mpBody1->mPos_x + pEvent->mpBody1->mWidth) - (pEvent->mpBody2->mPos_x - pEvent->mpBody2->mWidth));
					right = abs((pEvent->mpBody1->mPos_x - pEvent->mpBody1->mWidth) - (pEvent->mpBody2->mPos_x + pEvent->mpBody2->mWidth));
					in = abs((pEvent->mpBody1->mPos_y + pEvent->mpBody1->mDepth) - (pEvent->mpBody2->mPos_y - pEvent->mpBody2->mDepth));
					out = abs((pEvent->mpBody1->mPos_y - pEvent->mpBody1->mDepth) - (pEvent->mpBody2->mPos_y + pEvent->mpBody2->mDepth));

					pEvent->mpBody2->active = false;
					if (right < left && right < in && right < out)//condition for right
					{
						pEvent->mpBody1->mPos_x += 0.5;

						if (b1 == ENTITY_TYPE::CHARGER)
						{
							Charger* enemy = static_cast<Charger*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody1->key].pEn);

							enemy->health -= 2;
							if (play->mOnePunchMan)
							{
								enemy->health = 0;
							}
							if (enemy->health <= 0)
							{
								pEvent->mpBody1->active = false;
								pTr->mScaleX = 0;
								pTr->mScaleY = 0;
								pTr->mScaleZ = 0;
							}
							{
								Particle temp;
								temp.age = 0.7;
								temp.position = Vector4(enemy->pBd->mPos_x, enemy->pBd->mPos_y, enemy->pBd->mPos_z, 0.5);
								temp.TextureIndex = 0;
								temp.velocity = Vector4(3, 3, 3, 0.5);
								ParticleManager::GetInstance()->SphereEmit(40, temp);
							}
						}
						else if (b1 == ENTITY_TYPE::BOSS1)
						{
							pEvent->mpBody1->mPos_x -= 0.5;
							Boss1* enemy = static_cast<Boss1*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody1->key].pEn);

							enemy->health -= 2;
							if (play->mOnePunchMan)
							{
								enemy->health = 0;
							}
							if (enemy->health <= 0)
							{
								pEvent->mpBody1->active = false;
								pTr->mScaleX = 0;
								pTr->mScaleY = 0;
								pTr->mScaleZ = 0;
							}
							{
								Particle temp;
								temp.age = 0.7;
								temp.position = Vector4(enemy->pBd->mPos_x, enemy->pBd->mPos_y, enemy->pBd->mPos_z, 0.5);
								temp.TextureIndex = 0;
								temp.velocity = Vector4(3, 3, 3, 0.5);
								ParticleManager::GetInstance()->SphereEmit(40, temp);
							}
						}
						else if (b1 == ENTITY_TYPE::BOSS2)
						{
							pEvent->mpBody1->mPos_x -= 0.5;
							Boss2* enemy = static_cast<Boss2*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody1->key].pEn);

							enemy->health -= 2;
							if (play->mOnePunchMan)
							{
								enemy->health = 0;
							}
							if (enemy->health <= 0)
							{
								pEvent->mpBody1->active = false;
								pTr->mScaleX = 0;
								pTr->mScaleY = 0;
								pTr->mScaleZ = 0;
							}
							{
								Particle temp;
								temp.age = 0.7;
								temp.position = Vector4(enemy->pBd->mPos_x, enemy->pBd->mPos_y, enemy->pBd->mPos_z, 0.5);
								temp.TextureIndex = 0;
								temp.velocity = Vector4(3, 3, 3, 0.5);
								ParticleManager::GetInstance()->SphereEmit(40, temp);
							}
						}
						else if (b1 == ENTITY_TYPE::SHOOTER)
						{
							Shooter* enemy = static_cast<Shooter*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody1->key].pEn);

							enemy->health -= 50;
							if (play->mOnePunchMan)
							{
								enemy->health = 0;
							}
							if (enemy->health <= 0)
							{
								pEvent->mpBody1->active = false;
								pTr->mScaleX = 0;
								pTr->mScaleY = 0;
								pTr->mScaleZ = 0;
							}
							{
								Particle temp;
								temp.age = 0.7;
								temp.position = Vector4(enemy->pBd->mPos_x, enemy->pBd->mPos_y, enemy->pBd->mPos_z, 0.5);
								temp.TextureIndex = 0;
								temp.velocity = Vector4(3, 3, 3, 0.5);
								ParticleManager::GetInstance()->SphereEmit(40, temp);
							}
						}

						
					}
					else if (in < right && in < left && in < out)
					{
						pEvent->mpBody1->mPos_y -= 0.5;
						if (b1 == ENTITY_TYPE::CHARGER)
						{
							Charger* enemy = static_cast<Charger*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody1->key].pEn);

							enemy->health -= 2;
							if (play->mOnePunchMan)
							{
								enemy->health = 0;
							}
							if (enemy->health <= 0)
							{
								pEvent->mpBody1->active = false;
								pTr->mScaleX = 0;
								pTr->mScaleY = 0;
								pTr->mScaleZ = 0;
							}
							{
								Particle temp;
								temp.age = 0.7;
								temp.position = Vector4(enemy->pBd->mPos_x, enemy->pBd->mPos_y, enemy->pBd->mPos_z, 0.5);
								temp.TextureIndex = 0;
								temp.velocity = Vector4(3, 3, 3, 0.5);
								ParticleManager::GetInstance()->SphereEmit(40, temp);
							}
						}
						else if (b1 == ENTITY_TYPE::BOSS1)
						{
							pEvent->mpBody1->mPos_y += 0.5;
							Boss1* enemy = static_cast<Boss1*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody1->key].pEn);

							enemy->health -= 2;
							if (play->mOnePunchMan)
							{
								enemy->health = 0;
							}
							if (enemy->health <= 0)
							{
								pEvent->mpBody1->active = false;
								pTr->mScaleX = 0;
								pTr->mScaleY = 0;
								pTr->mScaleZ = 0;
							}
							{
								Particle temp;
								temp.age = 0.7;
								temp.position = Vector4(enemy->pBd->mPos_x, enemy->pBd->mPos_y, enemy->pBd->mPos_z, 0.5);
								temp.TextureIndex = 0;
								temp.velocity = Vector4(3, 3, 3, 0.5);
								ParticleManager::GetInstance()->SphereEmit(40, temp);
							}
						}
						else if (b1 == ENTITY_TYPE::BOSS2)
						{
							pEvent->mpBody1->mPos_y += 0.5;
							Boss2* enemy = static_cast<Boss2*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody1->key].pEn);

							enemy->health -= 2;
							if (play->mOnePunchMan)
							{
								enemy->health = 0;
							}
							if (enemy->health <= 0)
							{
								pEvent->mpBody1->active = false;
								pTr->mScaleX = 0;
								pTr->mScaleY = 0;
								pTr->mScaleZ = 0;
							}
							{
								Particle temp;
								temp.age = 0.7;
								temp.position = Vector4(enemy->pBd->mPos_x, enemy->pBd->mPos_y, enemy->pBd->mPos_z, 0.5);
								temp.TextureIndex = 0;
								temp.velocity = Vector4(3, 3, 3, 0.5);
								ParticleManager::GetInstance()->SphereEmit(40, temp);
							}
						}
						else if (b1 == ENTITY_TYPE::SHOOTER)
						{
							Shooter* enemy = static_cast<Shooter*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody1->key].pEn);

							enemy->health -= 50;
							if (play->mOnePunchMan)
							{
								enemy->health = 0;
							}
							if (enemy->health <= 0)
							{
								pEvent->mpBody1->active = false;
								pTr->mScaleX = 0;
								pTr->mScaleY = 0;
								pTr->mScaleZ = 0;
							}
							{
								Particle temp;
								temp.age = 0.7;
								temp.position = Vector4(enemy->pBd->mPos_x, enemy->pBd->mPos_y, enemy->pBd->mPos_z, 0.5);
								temp.TextureIndex = 0;
								temp.velocity = Vector4(3, 3, 3, 0.5);
								ParticleManager::GetInstance()->SphereEmit(40, temp);
							}
						}
					}
					else if (out < right && out < left && out < in)
					{
						pEvent->mpBody1->mPos_y += 0.5;
						if (b1 == ENTITY_TYPE::CHARGER)
						{
							Charger* enemy = static_cast<Charger*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody1->key].pEn);

							enemy->health -= 2;
							if (play->mOnePunchMan)
							{
								enemy->health = 0;
							}
							if (enemy->health <= 0)
							{
								pEvent->mpBody1->active = false;
								pTr->mScaleX = 0;
								pTr->mScaleY = 0;
								pTr->mScaleZ = 0;
							}
							{
								Particle temp;
								temp.age = 0.7;
								temp.position = Vector4(enemy->pBd->mPos_x, enemy->pBd->mPos_y, enemy->pBd->mPos_z, 0.5);
								temp.TextureIndex = 0;
								temp.velocity = Vector4(3, 3, 3, 0.5);
								ParticleManager::GetInstance()->SphereEmit(40, temp);
							}
						}
						else if (b1 == ENTITY_TYPE::BOSS1)
						{
							pEvent->mpBody1->mPos_y -= 0.5;
							Boss1* enemy = static_cast<Boss1*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody1->key].pEn);

							enemy->health -= 2;
							if (play->mOnePunchMan)
							{
								enemy->health = 0;
							}
							if (enemy->health <= 0)
							{
								pEvent->mpBody1->active = false;
								pTr->mScaleX = 0;
								pTr->mScaleY = 0;
								pTr->mScaleZ = 0;
							}
							{
								Particle temp;
								temp.age = 0.7;
								temp.position = Vector4(enemy->pBd->mPos_x, enemy->pBd->mPos_y, enemy->pBd->mPos_z, 0.5);
								temp.TextureIndex = 0;
								temp.velocity = Vector4(3, 3, 3, 0.5);
								ParticleManager::GetInstance()->SphereEmit(40, temp);
							}
						}
						else if (b1 == ENTITY_TYPE::BOSS2)
						{
							pEvent->mpBody1->mPos_y -= 0.5;
							Boss2* enemy = static_cast<Boss2*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody1->key].pEn);

							enemy->health -= 2;
							if (play->mOnePunchMan)
							{
								enemy->health = 0;
							}
							if (enemy->health <= 0)
							{
								pEvent->mpBody1->active = false;
								pTr->mScaleX = 0;
								pTr->mScaleY = 0;
								pTr->mScaleZ = 0;
							}
							{
								Particle temp;
								temp.age = 0.7;
								temp.position = Vector4(enemy->pBd->mPos_x, enemy->pBd->mPos_y, enemy->pBd->mPos_z, 0.5);
								temp.TextureIndex = 0;
								temp.velocity = Vector4(3, 3, 3, 0.5);
								ParticleManager::GetInstance()->SphereEmit(40, temp);
							}
						}
						else if (b1 == ENTITY_TYPE::SHOOTER)
						{
							Shooter* enemy = static_cast<Shooter*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody1->key].pEn);

							enemy->health -= 50;
							if (play->mOnePunchMan)
							{
								enemy->health = 0;
							}
							if (enemy->health <= 0)
							{
								pEvent->mpBody1->active = false;
								pTr->mScaleX = 0;
								pTr->mScaleY = 0;
								pTr->mScaleZ = 0;
							}
							{
								Particle temp;
								temp.age = 0.7;
								temp.position = Vector4(enemy->pBd->mPos_x, enemy->pBd->mPos_y, enemy->pBd->mPos_z, 0.5);
								temp.TextureIndex = 0;
								temp.velocity = Vector4(3, 3, 3, 0.5);
								ParticleManager::GetInstance()->SphereEmit(40, temp);
							}
						}
					}
					else//condition for left
					{
						pEvent->mpBody1->mPos_x -= 0.5;
						if (b1 == ENTITY_TYPE::CHARGER)
						{
							Charger* enemy = static_cast<Charger*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody1->key].pEn);

							enemy->health -= 2;
							if (play->mOnePunchMan)
							{
								enemy->health = 0;
							}
							if (enemy->health <= 0)
							{
								pEvent->mpBody1->active = false;
								pTr->mScaleX = 0;
								pTr->mScaleY = 0;
								pTr->mScaleZ = 0;
							}
							{
								Particle temp;
								temp.age = 0.7;
								temp.position = Vector4(enemy->pBd->mPos_x, enemy->pBd->mPos_y, enemy->pBd->mPos_z, 0.5);
								temp.TextureIndex = 0;
								temp.velocity = Vector4(3, 3, 3, 0.5);
								ParticleManager::GetInstance()->SphereEmit(40, temp);
							}
						}
						else if (b1 == ENTITY_TYPE::BOSS1)
						{
							pEvent->mpBody1->mPos_x += 0.5;
							Boss1* enemy = static_cast<Boss1*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody1->key].pEn);

							enemy->health -= 2;
							if (play->mOnePunchMan)
							{
								enemy->health = 0;
							}
							if (enemy->health <= 0)
							{
								pEvent->mpBody1->active = false;
								pTr->mScaleX = 0;
								pTr->mScaleY = 0;
								pTr->mScaleZ = 0;
							}
							{
								Particle temp;
								temp.age = 0.7;
								temp.position = Vector4(enemy->pBd->mPos_x, enemy->pBd->mPos_y, enemy->pBd->mPos_z, 0.5);
								temp.TextureIndex = 0;
								temp.velocity = Vector4(3, 3, 3, 0.5);
								ParticleManager::GetInstance()->SphereEmit(40, temp);
							}
						}
						else if (b1 == ENTITY_TYPE::BOSS2)
						{
							pEvent->mpBody1->mPos_x += 0.5;
							Boss2* enemy = static_cast<Boss2*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody1->key].pEn);

							enemy->health -= 2;
							if (play->mOnePunchMan)
							{
								enemy->health = 0;
							}
							if (enemy->health <= 0)
							{
								pEvent->mpBody1->active = false;
								pTr->mScaleX = 0;
								pTr->mScaleY = 0;
								pTr->mScaleZ = 0;
							}
							{
								Particle temp;
								temp.age = 0.7;
								temp.position = Vector4(enemy->pBd->mPos_x, enemy->pBd->mPos_y, enemy->pBd->mPos_z, 0.5);
								temp.TextureIndex = 0;
								temp.velocity = Vector4(3, 3, 3, 0.5);
								ParticleManager::GetInstance()->SphereEmit(40, temp);
							}
						}
						else if (b1 == ENTITY_TYPE::SHOOTER)
						{
							Shooter* enemy = static_cast<Shooter*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody1->key].pEn);

							enemy->health -= 50;
							if (play->mOnePunchMan)
							{
								enemy->health = 0;
							}
							if (enemy->health <= 0)
							{
								pEvent->mpBody1->active = false;
								pTr->mScaleX = 0;
								pTr->mScaleY = 0;
								pTr->mScaleZ = 0;
							}
							{
								Particle temp;
								temp.age = 0.7;
								temp.position = Vector4(enemy->pBd->mPos_x, enemy->pBd->mPos_y, enemy->pBd->mPos_z, 0.5);
								temp.TextureIndex = 0;
								temp.velocity = Vector4(3, 3, 3, 0.5);
								ParticleManager::GetInstance()->SphereEmit(40, temp);
							}
						}
					}

				}
			}
		}
	}
}
