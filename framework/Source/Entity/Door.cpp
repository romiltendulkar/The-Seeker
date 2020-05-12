#include "../pch.h"
#include "Door.h"
#include "../Components/Transform.h"
#include "../Components/Asset.h"
#include "../Components/Body.h"
#include "../../Manager/ObjectFactoryManager.h"
#include "../../Manager/UIManager.h"

#include "..\Events\CollisionEvent.h"

Door::Door() : Entity(ENTITY_TYPE::DOOR)
{
	mTimer = 0;
	mTimerLimit = 2;
	Type1Counter = 0;
	Type2Counter = 0;
	Type3Counter = 0;
	Enemy_Killed = 0;
	w = a = s = d = false;
}

Door::~Door() {}

void Door::Serialize(Json::Value root, unsigned int position)
{

	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn = this;

	Json::Value dataRoot = root["Transform"];

	//Transform
	if (!dataRoot.isNull())
	{
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

		pAs->obj->SetDiffuse(0.54f, 0.27f, 0.07f);
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

	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn->mEntityName = "Door";
	key = position;
	this->complete = true;
	this->runtime = false;
	ObjectFactoryManager::GetInstance()->LoadObject("ConditionBox", key + 1);
}
void Door::Update(float deltaTime)
{
	if (!checkonce)
	{
		checkonce = true;
		(*MemoryManager::GetInstance()->ObjectPool)[key + 1].pBd->mPos_x = CBoxPos.x;
		(*MemoryManager::GetInstance()->ObjectPool)[key + 1].pBd->mPos_y = CBoxPos.y;
		(*MemoryManager::GetInstance()->ObjectPool)[key + 1].pBd->mPos_z = CBoxPos.z;

		ShapeAABB* newShape = new ShapeAABB((*MemoryManager::GetInstance()->ObjectPool)[key + 1].pBd);

		newShape->mBottom = 0 - CBoxScale.z;
		newShape->mTop = 0 + CBoxScale.z;

		newShape->mIn = 0 + CBoxScale.y;
		newShape->mOut = 0 - CBoxScale.y;

		newShape->mLeft = 0 - CBoxScale.x;
		newShape->mRight = 0 + CBoxScale.x;
		(*MemoryManager::GetInstance()->ObjectPool)[key + 1].pBd->mShape = newShape;

		(*MemoryManager::GetInstance()->ObjectPool)[key + 1].pBd->mScale_x = CBoxScale.x;
		(*MemoryManager::GetInstance()->ObjectPool)[key + 1].pBd->mScale_y = CBoxScale.y;
		(*MemoryManager::GetInstance()->ObjectPool)[key + 1].pBd->mScale_z = CBoxScale.z;
	}
	Body *b1 = (*MemoryManager::GetInstance()->ObjectPool)[key + 1].pBd;
	Body* b2 = (*MemoryManager::GetInstance()->ObjectPool)[2].pBd;
	if (CBoxType == 1 &&
		!CollisionManager::GetInstance()->CheckCollisionCuboidCuboid(b1->mShape, b1->mPos_x, b1->mPos_y, b1->mPos_z, b2->mShape, b2->mPos_x, b2->mPos_y, b2->mPos_z)
		)
	{
		if (mConditionVariable == false)
			UIManager::GetInstance()->SetDialogue("Oh you are finally awake! Try moving around by pressing controller Dpad up down left right");
		//std::cout << Type1Counter << std::endl;
		if ((DXInputManager::GetInstance()->isKeyTrigger(DIK_W)||
			DXInputManager::GetInstance()->GamepadTrigger(GAMEPAD_UP)) && !w)
		{
			++Type1Counter;
			w = true;
		}
		else if ((DXInputManager::GetInstance()->isKeyTrigger(DIK_A)||
			DXInputManager::GetInstance()->GamepadTrigger(GAMEPAD_LEFT)) && !a)
		{
			++Type1Counter;
			a = true;
		}
		else if ((DXInputManager::GetInstance()->isKeyTrigger(DIK_S) ||
			DXInputManager::GetInstance()->GamepadTrigger(GAMEPAD_DOWN)) && !s)
		{
			++Type1Counter;
			s = true;
		}
		else if ((DXInputManager::GetInstance()->isKeyTrigger(DIK_D)
			||DXInputManager::GetInstance()->GamepadTrigger(GAMEPAD_RIGHT))
			&& !d)
		{
			++Type1Counter;
			d = true;
		}
		if (Type1Counter == 4)
		{
			mConditionVariable = true;
		}
	}
	if (CBoxType == 2 &&
		!CollisionManager::GetInstance()->CheckCollisionCuboidCuboid(b1->mShape, b1->mPos_x, b1->mPos_y, b1->mPos_z, b2->mShape, b2->mPos_x, b2->mPos_y, b2->mPos_z)
		)
	{
		if (mConditionVariable == false)
		{
			UIManager::GetInstance()->SetDialogue("Press controller X for light punch, practice 3 times and get the rust off sleepy head!");
		}
		//std::vector<unsigned int>::iterator itr = ObjectFactoryManager::GetInstance()->LEnemyPositions.begin();
		//while (itr != ObjectFactoryManager::GetInstance()->LEnemyPositions.end())
		//{
		//	Body* bb1 = (*MemoryManager::GetInstance()->ObjectPool)[key + 1].pBd;
		//	Body* bb2 = (*MemoryManager::GetInstance()->ObjectPool)[*itr].pBd;
		//	if (!CollisionManager::GetInstance()->CheckCollisionCuboidCuboid(bb1->mShape, bb1->mPos_x, bb1->mPos_y, bb1->mPos_z,
		//		bb2->mShape, bb2->mPos_x, bb2->mPos_y, bb2->mPos_z)
		//		)
		//	{
		//		if(bb2.)
		//	}
		//	++itr;
		//}
		//std::cout << Type1Counter << std::endl;
		if (DXInputManager::GetInstance()->isKeyTrigger(DIK_SPACE)||
			DXInputManager::GetInstance()->GamepadTrigger(GAMEPAD_X))
		{
			++Type2Counter;
		}
		if (Type2Counter >= 3)// && Enemy_Killed == Enemy_Required)
		{
			mConditionVariable = true;
		}
	}
	if (CBoxType == 3 &&
		!CollisionManager::GetInstance()->CheckCollisionCuboidCuboid(b1->mShape, b1->mPos_x, b1->mPos_y, b1->mPos_z, b2->mShape, b2->mPos_x, b2->mPos_y, b2->mPos_z)
		)
	{
		if (mConditionVariable == false)
			UIManager::GetInstance()->SetDialogue("Press contoller Y for heavy punch, the path wont open up for the weak!");
		if ((DXInputManager::GetInstance()->isKeyTrigger(DIK_E))||
			DXInputManager::GetInstance()->GamepadTrigger(GAMEPAD_Y))
		{
			++Type3Counter;
		}
		if (Type3Counter >= 3)// && Enemy_Killed == Enemy_Required)
		{
			mConditionVariable = true;
		}
	}
	if (CBoxType == 4 &&
		!CollisionManager::GetInstance()->CheckCollisionCuboidCuboid(b1->mShape, b1->mPos_x, b1->mPos_y, b1->mPos_z, b2->mShape, b2->mPos_x, b2->mPos_y, b2->mPos_z)
		)
	{
		if (mConditionVariable == false)
			UIManager::GetInstance()->SetDialogue("When I see an enemy I will mark them red, press LB to lock me onto them and RB to shoot!");
	}
	if (mConditionVariable)
	{
		pTr->mPositionZ -= 0.03;
		pBd->mPos_z -= 0.03;
	}
}

void Door::HandleEvent(Event* pEventToHandle)
{
	if (pEventToHandle)
	{
		if (pEventToHandle->mEventType == EVENT_TYPE::COLLIDE_EVENT)
		{
			float top, bottom, left, right, in, out;
			CollisionEvent* pEvent = static_cast<CollisionEvent*>(pEventToHandle);
			ENTITY_TYPE b1 = pEvent->mpBody1->mpEnowner->GetEntityType();
			ENTITY_TYPE b2 = pEvent->mpBody2->mpEnowner->GetEntityType();
			if (b2 == ENTITY_TYPE::DOOR)
			{
				if (b1 == ENTITY_TYPE::EXPLODER || b1 == ENTITY_TYPE::SHOOTER || b1 == ENTITY_TYPE::BOSS1 || b1 == ENTITY_TYPE::CHARGER ||
					b1 == ENTITY_TYPE::SEEKER || b1 == ENTITY_TYPE::PLAYER || b1 == ENTITY_TYPE::BULLET || b1 == ENTITY_TYPE::COLLIDER ||
					b1 == ENTITY_TYPE::BLOCK || b1 == ENTITY_TYPE::ENEMY || b1 == ENTITY_TYPE::VIEWBOX || b1 == ENTITY_TYPE::WENEMY ||
					b1 == ENTITY_TYPE::SWITCH || b1 == ENTITY_TYPE::PILLAR)
				{
					top = abs((pEvent->mpBody2->mPos_z + pEvent->mpBody2->mHeight) - (pEvent->mpBody1->mPos_z - pEvent->mpBody1->mHeight));
					bottom = abs((pEvent->mpBody2->mPos_z - pEvent->mpBody2->mHeight) - (pEvent->mpBody1->mPos_z + pEvent->mpBody1->mHeight));
					left = abs((pEvent->mpBody2->mPos_x + pEvent->mpBody2->mWidth) - (pEvent->mpBody1->mPos_x - pEvent->mpBody1->mWidth));
					right = abs((pEvent->mpBody2->mPos_x - pEvent->mpBody2->mWidth) - (pEvent->mpBody1->mPos_x + pEvent->mpBody1->mWidth));
					in = abs((pEvent->mpBody2->mPos_y + pEvent->mpBody2->mDepth) - (pEvent->mpBody1->mPos_y - pEvent->mpBody1->mDepth));
					out = abs((pEvent->mpBody2->mPos_y - pEvent->mpBody2->mDepth) - (pEvent->mpBody1->mPos_y + pEvent->mpBody1->mDepth));
					//std::cout << "top: " << top << ", bottom: " << bottom << ", left: " << left << ", right: " << right << ", in: "<<in<<" out: "<< out<<  std::endl;
					if (top < bottom && top < right && top < left && top < in && top < out)//condition for top
					{
						pEvent->mpBody1->body_State = ON_SURFACE;
						pEvent->mpBody1->mPos_z = pEvent->mpBody2->mPos_z + pEvent->mpBody2->mHeight + pEvent->mpBody1->mHeight;
						if (b1 == ENTITY_TYPE::PLAYER || b1 == ENTITY_TYPE::SEEKER || b1 == ENTITY_TYPE::ENEMY || b1 == ENTITY_TYPE::WENEMY || b1 == ENTITY_TYPE::BLOCK)
						{
							pEvent->mpBody1->mVelo_x *= pEvent->mpBody2->mFrictionOnSurface;
							pEvent->mpBody1->mVelo_y *= pEvent->mpBody2->mFrictionOnSurface;
						}
						if (b1 == ENTITY_TYPE::BULLET)
						{
							pEvent->mpBody1->mVelo_z = (pEvent->mpBody2->mVelo_z * 0.50f);
						}
						if (b1 == ENTITY_TYPE::BLOCK)
						{
							pEvent->mpBody1->mVelo_z = 0;
						}
					}
					else if (bottom < top && bottom < right && bottom < left && bottom < in && bottom < out)//condition for bottom
					{
						pEvent->mpBody1->mPos_z = pEvent->mpBody2->mPos_z - pEvent->mpBody2->mHeight - pEvent->mpBody1->mHeight;
					}
					else if (right < top && right < bottom && right < left && right < in && right < out)//condition for right
					{
						pEvent->mpBody1->mCollidingSideRight = true;
						//c->mpBodies[0]->mRightMovement = false;
						pEvent->mpBody1->mPos_x = pEvent->mpBody2->mPos_x - pEvent->mpBody2->mWidth - pEvent->mpBody1->mWidth - 0.0001f;
						pEvent->mpBody1->mVelo_x *= -1;
						if (b1 == ENTITY_TYPE::PLAYER)
						{
							pEvent->mpBody1->mVelo_x = 0;
						}
					}
					else if (in < right && in < left && in < top && in < bottom && in < out)
					{
						pEvent->mpBody1->mCollidingSideIn = true;
						//c->mpBodies[0]->mDownMovement = false;
						pEvent->mpBody1->mPos_y = pEvent->mpBody2->mPos_y + pEvent->mpBody2->mDepth + pEvent->mpBody1->mDepth + 0.0001f;
						pEvent->mpBody1->mVelo_y *= -1;
						if (b1 == ENTITY_TYPE::PLAYER)
						{
							pEvent->mpBody1->mVelo_y = 0;
							//	c->mpBodies[0]->mAcc_y = -10;
						}
					}
					else if (out < right && out < left && out < top && out < bottom && out < in)
					{
						pEvent->mpBody1->mCollidingSideOut = true;
						//c->mpBodies[0]->mUpMovement = false;
						pEvent->mpBody1->mPos_y = pEvent->mpBody2->mPos_y - pEvent->mpBody2->mDepth - pEvent->mpBody1->mDepth - 0.0001f;
						pEvent->mpBody1->mVelo_y *= -1;
						if (b1 == ENTITY_TYPE::PLAYER)
						{
							pEvent->mpBody1->mVelo_y = 0;
							//	c->mpBodies[0]->mAcc_y = -60;
						}

					}
					else//condition for left
					{
						pEvent->mpBody1->mCollidingSideLeft = true;
						//c->mpBodies[0]->mLeftMovement = false;
						pEvent->mpBody1->mPos_x = pEvent->mpBody2->mPos_x + pEvent->mpBody2->mWidth + pEvent->mpBody1->mWidth + 0.0001f;
						pEvent->mpBody1->mVelo_x *= -1;
						if (b1 == ENTITY_TYPE::PLAYER)
						{
							pEvent->mpBody1->mVelo_x = 0;
						}
					}
				}
			}
			else if (b1 == ENTITY_TYPE::DOOR)
			{
				if (b2 == ENTITY_TYPE::EXPLODER || b2 == ENTITY_TYPE::SHOOTER || b2 == ENTITY_TYPE::BOSS1 || b2 == ENTITY_TYPE::CHARGER ||
					b2 == ENTITY_TYPE::SEEKER || b2 == ENTITY_TYPE::PLAYER || b2 == ENTITY_TYPE::BULLET || b2 == ENTITY_TYPE::COLLIDER ||
					b2 == ENTITY_TYPE::BLOCK || b2 == ENTITY_TYPE::ENEMY || b2 == ENTITY_TYPE::VIEWBOX || b2 == ENTITY_TYPE::WENEMY ||
					b2 == ENTITY_TYPE::SWITCH || b2 == ENTITY_TYPE::PILLAR)
				{
					top = abs((pEvent->mpBody1->mPos_z + pEvent->mpBody1->mHeight) - (pEvent->mpBody2->mPos_z - pEvent->mpBody2->mHeight));
					bottom = abs((pEvent->mpBody1->mPos_z - pEvent->mpBody1->mHeight) - (pEvent->mpBody2->mPos_z + pEvent->mpBody2->mHeight));
					left = abs((pEvent->mpBody1->mPos_x + pEvent->mpBody1->mWidth) - (pEvent->mpBody2->mPos_x - pEvent->mpBody2->mWidth));
					right = abs((pEvent->mpBody1->mPos_x - pEvent->mpBody1->mWidth) - (pEvent->mpBody2->mPos_x + pEvent->mpBody2->mWidth));
					in = abs((pEvent->mpBody1->mPos_y + pEvent->mpBody1->mDepth) - (pEvent->mpBody2->mPos_y - pEvent->mpBody2->mDepth));
					out = abs((pEvent->mpBody1->mPos_y - pEvent->mpBody1->mDepth) - (pEvent->mpBody2->mPos_y + pEvent->mpBody2->mDepth));
					//std::cout << "top: " << top << ", bottom: " << bottom << ", left: " << left << ", right: " << right << ", in: "<<in<<" out: "<< out<<  std::endl;
					if (top < bottom && top < right && top < left && top < in && top < out)//condition for top
					{
						pEvent->mpBody2->body_State = ON_SURFACE;
						pEvent->mpBody2->mPos_z = pEvent->mpBody1->mPos_z + pEvent->mpBody1->mHeight + pEvent->mpBody2->mHeight;
						if (b2 == ENTITY_TYPE::PLAYER || b2 == ENTITY_TYPE::SEEKER || b2 == ENTITY_TYPE::ENEMY || b2 == ENTITY_TYPE::WENEMY || b2 == ENTITY_TYPE::BLOCK)
						{
							pEvent->mpBody2->mVelo_x *= pEvent->mpBody1->mFrictionOnSurface;
							pEvent->mpBody2->mVelo_y *= pEvent->mpBody1->mFrictionOnSurface;
						}
						if (b2 == ENTITY_TYPE::BULLET)
						{
							pEvent->mpBody2->mVelo_z = abs(pEvent->mpBody2->mVelo_z * 0.50f);
						}
						if (b2 == ENTITY_TYPE::BLOCK)
						{
							pEvent->mpBody2->mVelo_z = 0;
						}
					}
					else if (bottom < top && bottom < right && bottom < left && bottom < in && bottom < out)//condition for bottom
					{
						pEvent->mpBody2->mPos_z = pEvent->mpBody1->mPos_z - pEvent->mpBody1->mHeight - pEvent->mpBody2->mHeight;
					}
					else if (right < top && right < bottom && right < left && right < in && right < out)//condition for right
					{
						pEvent->mpBody2->mCollidingSideRight = true;
						//c->mpBodies[1]->mRightMovement = false;
						pEvent->mpBody2->mPos_x = pEvent->mpBody1->mPos_x - pEvent->mpBody1->mWidth - pEvent->mpBody2->mWidth - 0.0001f;
						pEvent->mpBody2->mVelo_x *= -1;
						if (b2 == ENTITY_TYPE::PLAYER)
						{
							pEvent->mpBody2->mVelo_x = 0;
						}
					}
					else if (in < right && in < left && in < top && in < bottom && in < out)
					{
						pEvent->mpBody2->mCollidingSideIn = true;
						//c->mpBodies[1]->mDownMovement = false;
						pEvent->mpBody2->mPos_y = pEvent->mpBody1->mPos_y + pEvent->mpBody1->mDepth + pEvent->mpBody2->mDepth + 0.0001f;
						pEvent->mpBody2->mVelo_y *= -1;
						if (b2 == ENTITY_TYPE::PLAYER)
						{
							//	c->mpBodies[1]->mAcc_y = -10;
							pEvent->mpBody2->mVelo_y = 0;
						}
					}
					else if (out < right && out < left && out < top && out < bottom && out < in)
					{
						pEvent->mpBody2->mCollidingSideOut = true;
						//c->mpBodies[1]->mUpMovement = false;
						pEvent->mpBody2->mPos_y = pEvent->mpBody1->mPos_y - pEvent->mpBody1->mDepth - pEvent->mpBody2->mDepth - 0.0001f;
						pEvent->mpBody2->mVelo_y *= -1;
						if (b2 == ENTITY_TYPE::PLAYER)
						{
							pEvent->mpBody2->mVelo_y = 0;

						}
					}
					else//condition for left
					{
						pEvent->mpBody2->mCollidingSideLeft = true;
						//c->mpBodies[1]->mLeftMovement = false;
						pEvent->mpBody2->mPos_x = pEvent->mpBody1->mPos_x + pEvent->mpBody1->mWidth + pEvent->mpBody2->mWidth + 0.0001f;
						pEvent->mpBody2->mVelo_x *= -1;
						if (b2 == ENTITY_TYPE::PLAYER)
						{
							pEvent->mpBody2->mVelo_x = 0;
						}
					}
				}
			}
		}
	}
}