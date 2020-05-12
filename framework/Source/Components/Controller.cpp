/**
/**
 * @file Controller.cpp
 * @authors Vineet Amarjeet Dogra
 * @date 10/1/19
 * @brief This file governs how the controller component is supposed to work and manages certain scenarios where the player should or should not be able to move.
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#include "../pch.h"
#include "Controller.h"
#include "Transform.h"
#include "../GameObject.h"
#include "../../Manager/DXInputManager.h"
#include "..//..//Manager/MemoryManager.h"
#include "../../Manager/ObjectFactoryManager.h"
#include "../../Manager/GraphicsManager.h"
 //#include "../../Manager/ScriptManager.h"
#include "../../Manager/AudioManager.h"
#include "../../Manager/LevelEditor.h"
#include "../../Manager/EventManager.h"
#include "../../Manager/PhysicsManager.h"
#include "../../Manager/GameStateManager.h"


#include "..//Entity/Player.h"
#include "..//Entity/Bullet.h"
#include "..//Entity/left.h"
#include "..//Entity/right.h"

#include "..\Events\MoveEvent.h"

 /**
	In this constructor we initialize the variables.
 */
float epsilon = 0.1f;
Controller::Controller() : Component(CONTROLLER)
{
	key = -1;
	debug = false;
	lvle = false;
	ammo_check = 0;
	usable = true;
	lock_check = false;
	lock_key = -1;
	auto_lock_key = 0;
	rolling = false;
	auto_check = false;
}

Controller::~Controller()
{
}

/**
   This functions serializes the controller component.
*/


void Controller::Serialize(Json::Value root, unsigned int key)
{
	this->key = key;
	(*MemoryManager::GetInstance()->ObjectPool)[key].pCr = this;
	(*MemoryManager::GetInstance()->ObjectPool)[key].pCr->mpEnowner = (*MemoryManager::GetInstance()->ObjectPool)[key].pEn;
	 ammo_check = 0;
	 usable = true;
	 lock_check = false;
	 lock_key = -1;
	 auto_lock_key = 0;
	 rolling = false;
	 auto_check = false;
}

void Controller::DeSerialize(Json::Value& root)
{
	root["Controller"] = (Json::objectValue);
}

/**
   This functions updates the position or runtime object generation by the player based on the key inputs. It also governs if the player is able to move in a particular direction or not.
   \param rdeltaTime | Delta time between frames.
*/

void Controller::Update(float deltaTime)
{
	//sol::state state;

	//state.open_libraries(sol::lib::base, sol::lib::package);
	//std::string package_path = state["package"]["path"];
	//state["package"]["path"] = (package_path + ";Source/Script/middleclass.lua").c_str();
	//state.script_file("Source/Script/Controller.lua");

	//ScriptManager obj(&state, "Class");




	//float mpositiony = state["config"]["mpositiony"];
	//float mpositionx = state["config"]["mpositionx"];
	//float mpositionz = state["config"]["mpositionz"];

	//float StopNum = state["config"]["StopNum"];
	//float StartNum = state["config"]["StartNum"];
	//float WalkNum = state["config"]["WalkNum"];

	//float StopDen = state["config"]["StopDen"];
	//float StartDen = state["config"]["StartDen"];
	//float WalkDen = state["config"]["WalkDen"];

	//float speed = state["config"]["speed"];

	//int walks = state["config"]["noofsteps"];

	////on surface movement
	//float l_velo_x_s = state["left_surface"]["mVelo_x"];
	//float l_velo_y_s = state["left_surface"]["mVelo_y"];
	//float l_velo_z_s = state["left_surface"]["mVelo_z"];

	//float r_velo_x_s = state["right_surface"]["mVelo_x"];
	//float r_velo_y_s = state["right_surface"]["mVelo_y"];
	//float r_velo_z_s = state["right_surface"]["mVelo_z"];

	//float u_velo_x_s = state["up_surface"]["mVelo_x"];
	//float u_velo_y_s = state["up_surface"]["mVelo_y"];
	//float u_velo_z_s = state["up_surface"]["mVelo_z"];

	//float d_velo_x_s = state["down_surface"]["mVelo_x"];
	//float d_velo_y_s = state["down_surface"]["mVelo_y"];
	//float d_velo_z_s = state["down_surface"]["mVelo_z"];

	////on falling movement
	//float l_velo_x_f = state["left_falling"]["mVelo_x"];
	//float l_velo_y_f = state["left_falling"]["mVelo_y"];
	//float l_velo_z_f = state["left_surface"]["mVelo_z"];

	//float r_velo_x_f = state["right_falling"]["mVelo_x"];
	//float r_velo_y_f = state["right_falling"]["mVelo_y"];
	//float r_velo_z_f = state["right_falling"]["mVelo_z"];

	//float u_velo_x_f = state["up_falling"]["mVelo_x"];
	//float u_velo_y_f = state["up_falling"]["mVelo_y"];
	//float u_velo_z_f = state["up_falling"]["mVelo_z"];

	//float d_velo_x_f = state["down_falling"]["mVelo_x"];
	//float d_velo_y_f = state["down_falling"]["mVelo_y"];
	//float d_velo_z_f = state["down_falling"]["mVelo_z"];

	////space movement
	//float l_velo_x_space = state["left_space"]["mVelo_x"];
	//float l_velo_y_space = state["left_space"]["mVelo_y"];
	//float l_velo_z_space = state["left_space"]["mVelo_z"];

	//float r_velo_x_space = state["right_space"]["mVelo_x"];
	//float r_velo_y_space = state["right_space"]["mVelo_y"];
	//float r_velo_z_space = state["right_space"]["mVelo_z"];

	//float u_velo_x_space = state["up_space"]["mVelo_x"];
	//float u_velo_y_space = state["up_space"]["mVelo_y"];
	//float u_velo_z_space = state["up_space"]["mVelo_z"];

	//float d_velo_x_space = state["down_space"]["mVelo_x"];
	//float d_velo_y_space = state["down_space"]["mVelo_y"];
	//float d_velo_z_space = state["down_space"]["mVelo_z"];

	////Up Event
	//float speed_up = state["Up_Event"]["speed"];
	//float time_up = state["Up_Event"]["time"];
	//std::string direction_up = state["Up_Event"]["direction"];

	////Up Event2
	//float speed_up1 = state["Up1_Event"]["speed"];
	//float time_up1 = state["Up1_Event"]["time"];
	//std::string direction_up1 = state["Up1_Event"]["direction"];


	////Left Event
	//float speed_left = state["Left_Event"]["speed"];
	//float time_left = state["Left_Event"]["time"];
	//std::string direction_left = state["Left_Event"]["direction"];

	////right Event
	//float speed_right = state["Right_Event"]["speed"];
	//float time_right = state["Right_Event"]["time"];
	//std::string direction_right = state["Right_Event"]["direction"];


	////down Event
	//float speed_down = state["Down_Event"]["speed"];
	//float time_down = state["Down_Event"]["time"];
	//std::string direction_down = state["Down_Event"]["direction"];


	//std::string shape = state["config"]["shape"];
	//const char* shape_char = shape.c_str();

	float u_velo_y_space = 10.0f;
	float u_velo_y_s = 2.0f;
	float u_velo_y_f = 2.0f;
	float d_velo_y_space = -10.0f;
	float d_velo_y_s = -2.0f;
	float d_velo_y_f = -2.0f;
	float l_velo_x_space = -10.0f;
	float l_velo_x_s = -2.0f;
	float l_velo_x_f = -2.0f;
	float r_velo_x_space = 10.0f;
	float r_velo_x_f = 2.0f;
	float r_velo_x_s = 2.0f;

	//std::string walk_animation = state["config"]["walk_animation"];
	//std::string stop_animation = state["config"]["stop_animation"];
	//std::string start_animation = state["config"]["start_animation"];
	if (ammo_check == 0)
	{
		ObjectFactoryManager::GetInstance()->CreateAmmoPack();
		ammo_check++;
	}

	//float StopTime = obj.CallFunction<float>("StopTime", StopNum, StopDen);

	//float StartTime = obj.CallFunction<float>("StartTime", StartNum, StartDen);
	//float WalkTime = obj.CallFunction<float>("WalkTime", WalkNum, WalkDen);
	if (usable)
	{
		if (lock_key == -1)
		{
			auto_check = false;
		}
		Transform* pTr = (*MemoryManager::GetInstance()->ObjectPool)[key].pTr;
		Body* pBd = (*MemoryManager::GetInstance()->ObjectPool)[key].pBd;
		Animation* pAn = (*MemoryManager::GetInstance()->ObjectPool)[key].pAn;
		Player* pl = static_cast<Player*>((*MemoryManager::GetInstance()->ObjectPool)[key].pEn);
		Right* hx = static_cast<Right*>((*MemoryManager::GetInstance()->ObjectPool)[0].pEn);
		Left* hv = static_cast<Left*>((*MemoryManager::GetInstance()->ObjectPool)[1].pEn);
		pressed_left = false;
		if (LevelEditor::GetInstance()->levelEditoris_on == true)
		{
			if ((DXInputManager::GetInstance()->isMouseTrigger(1))
				||DXInputManager::GetInstance()->GamepadTrigger(GAMEPAD_RIGHTSHOULDER))
			{
				lock_check = true;
			}
			if ((DXInputManager::GetInstance()->isMouseTrigger(2) ||
				DXInputManager::GetInstance()->GamepadTrigger(GAMEPAD_A)) && lock_key != -1)//auto_aim  modes
			{
				if (!auto_check)
				{
					auto_check = true;
					auto_lock_key = lock_key;
				}
				else
				{
					auto_check = false;
					auto_lock_key = -1;
					auto_counter = 0;
				}
			}
			else if ((DXInputManager::GetInstance()->isMouseTrigger(2)||
				DXInputManager::GetInstance()->GamepadTrigger(GAMEPAD_A)) && auto_lock_key != -1)//auto_aim  modes
			{
				auto_check = false;
				auto_lock_key = -1;
				auto_counter = 0;
			}
			if (auto_lock_key != -1 && (*MemoryManager::GetInstance()->ObjectPool)[auto_lock_key].pBd->active)
			{
				++frame_counter_auto;
				auto_counter++;
				if (auto_counter <= 360)
				{
					float  distance = ((pBd->mPos_x - (*MemoryManager::GetInstance()->ObjectPool)[auto_lock_key].pBd->mPos_x) * (pBd->mPos_x - (*MemoryManager::GetInstance()->ObjectPool)[auto_lock_key].pBd->mPos_x) +
						(pBd->mPos_y - (*MemoryManager::GetInstance()->ObjectPool)[auto_lock_key].pBd->mPos_y) * (pBd->mPos_y - (*MemoryManager::GetInstance()->ObjectPool)[auto_lock_key].pBd->mPos_y));
					if (distance < 350)
					{
						int begin = ObjectFactoryManager::GetInstance()->start;
						for (int i = begin; i < ObjectFactoryManager::GetInstance()->start + 20; i++)
						{
							if ((*MemoryManager::GetInstance()->ObjectPool)[i].pBd->active && (*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mVelo_x == 0 && (*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mVelo_y == 0)//if active fire.
							{
								Bullet* b = static_cast<Bullet*>((*MemoryManager::GetInstance()->ObjectPool)[i].pEn);
								b->framecounter = 0;
								if (frame_counter_auto % 10 == 0)
								{
									ObjectFactoryManager::GetInstance()->FireBullet(pBd, i, auto_lock_key);
								}
								AudioManager::GetInstance()->LoadAudio("Small Fireball-SoundBible.com.wav");
								break;
							}
							else if ((*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mVelo_x == 0 && (*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mVelo_y == 0)
							{
								Bullet* b = static_cast<Bullet*>((*MemoryManager::GetInstance()->ObjectPool)[i].pEn);
								b->framecounter = 0;
								(*MemoryManager::GetInstance()->ObjectPool)[i].pBd->active = true;
								if (frame_counter_auto % 10 == 0)
								{
									ObjectFactoryManager::GetInstance()->FireBullet(pBd, i, auto_lock_key);
								}
								break;
							}
						}
					}
					else
					{
						auto_counter = 0;
						frame_counter_auto = 0;
						auto_lock_key = -1;
					}
				}
				else if (auto_counter >= 720)
				{
					auto_counter = 0;
				}
			}
			if ((DXInputManager::GetInstance()->isKeyTrigger(DIK_LSHIFT) || DXInputManager::GetInstance()->GamepadTrigger(GAMEPAD_B)) && pl->mDashCounterCD == 0)
			{
				//std::cout << "PRESS" << std::endl;
				//lAnimationChain.push({ "roll", 1.0, 76 / 30.0f });
				AudioManager::GetInstance()->LoadAudio("Bite-SoundBible.com.wav");
				pl->mDashCounter = 10;
				pl->mDashCounterCD = -1;
				rolling = true;
				pAn->SetCurrentStatus("roll", 1.0f, 1 / 30.0f);
			}
			//if ((DXInputManager::GetInstance()->isKeyPressed(DIK_W) || DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_UP)) && (DXInputManager::GetInstance()->isKeyTrigger(DIK_SPACE) || DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_A)) && pBd->body_State == ON_SURFACE)
			//{
			//	pBd->mLeftMovement = false;
			//	pBd->mRightMovement = false;
			//	pBd->mDownMovement = false;
			//	pBd->mVelo_y = u_velo_y_space;
			//	pBd->mFacingUp = true;
			//	pBd->mFacingDown = false;
			//	pBd->mFacingRight = false;
			//	pBd->mFacingLeft = false;
			//}
			if ((DXInputManager::GetInstance()->isKeyPressed(DIK_W) || DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_UP)) && pBd->mUpMovement == true && rolling == false && hx->mFrameCounter <= 0 && hv->mFrameCounter <= 0)
			{
				//pTr->mPositionY += mpositiony;
				pBd->mVelo_y = u_velo_y_s;
				pBd->mFacingUp = true;
				pBd->mFacingDown = false;
				pBd->mFacingRight = false;
				pBd->mFacingLeft = false;
				//lAnimationChain.push({ "run", 1.0, 19 / 30.0f });
				(*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mRotationZ = 180;
				if (pAn)
					pAn->ChangeCurrentStatus("run", 1, 21 / 30.0f);
			}
			if ((DXInputManager::GetInstance()->isKeyPressed(DIK_W) || DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_UP)) && pBd->mUpMovement == true && pBd->body_State == FALLING)
			{
				pBd->mLeftMovement = false;
				pBd->mRightMovement = false;
				pBd->mDownMovement = false;
				pBd->mVelo_y = u_velo_y_f;
				pBd->mFacingUp = true;
				pBd->mFacingDown = false;
				pBd->mFacingRight = false;
				pBd->mFacingLeft = false;
			}
			//if ((DXInputManager::GetInstance()->isKeyPressed(DIK_S) || DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_DOWN)) && (DXInputManager::GetInstance()->isKeyTrigger(DIK_SPACE) || DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_A)) && pBd->body_State == ON_SURFACE)
			//{
			//	pBd->mLeftMovement = false;
			//	pBd->mRightMovement = false;
			//	pBd->mUpMovement = false;
			//	pBd->mVelo_y = d_velo_y_space;
			//	pBd->mFacingUp = false;
			//	pBd->mFacingDown = true;
			//	pBd->mFacingRight = false;
			//	pBd->mFacingLeft = false;
			//}
			//if ((DXInputManager::GetInstance()->isKeyPressed(DIK_S) || DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_A)) && pBd->mDownMovement == true && pBd->body_State == FALLING)
			//{
			//	//pTr->mPositionY -= mpositiony;
			//	pBd->mLeftMovement = false;
			//	pBd->mRightMovement = false;
			//	pBd->mUpMovement = false;
			//	pBd->mVelo_y = d_velo_y_s;
			//	pBd->mFacingUp = false;
			//	pBd->mFacingDown = true;
			//	pBd->mFacingRight = false;
			//	pBd->mFacingLeft = false;
			//}
			if ((DXInputManager::GetInstance()->isKeyPressed(DIK_S) || DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_DOWN)) && pBd->mDownMovement == true && rolling == false && hx->mFrameCounter <= 0 && hv->mFrameCounter <= 0)
			{
				//pTr->mPositionY -= mpositiony;
				pBd->mVelo_y = d_velo_y_f;
				pBd->mFacingUp = false;
				pBd->mFacingDown = true;
				pBd->mFacingRight = false;
				pBd->mFacingLeft = false;
				(*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mRotationZ = 0;
				if (pAn)
					pAn->ChangeCurrentStatus("run", 1, 21 / 30.0f);
			}
			/*if ((DXInputManager::GetInstance()->isKeyPressed(DIK_A) || DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_LEFT)) && (DXInputManager::GetInstance()->isKeyTrigger(DIK_SPACE) || DXInputManager::GetInstance()->GamepadTrigger(GAMEPAD_A)) && pBd->body_State == ON_SURFACE)
			{
				pBd->mDownMovement = false;
				pBd->mRightMovement = false;
				pBd->mUpMovement = false;
				pBd->mVelo_x = l_velo_x_space;
				pBd->mFacingUp = false;
				pBd->mFacingDown = false;
				pBd->mFacingRight = false;
				pBd->mFacingLeft = true;
			}*/
			if ((DXInputManager::GetInstance()->isKeyPressed(DIK_A) || DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_LEFT)) && pBd->mLeftMovement == true && pBd->body_State == FALLING)
			{
				pBd->mDownMovement = false;
				pBd->mRightMovement = false;
				pBd->mUpMovement = false;
				pBd->mVelo_x = l_velo_x_s;
				pBd->mFacingUp = false;
				pBd->mFacingDown = false;
				pBd->mFacingRight = false;
				pBd->mFacingLeft = true;
			}
			if ((DXInputManager::GetInstance()->isKeyPressed(DIK_A) || DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_LEFT)) && pBd->mLeftMovement == true && rolling == false && hx->mFrameCounter <= 0 && hv->mFrameCounter <= 0)
			{
				//pTr->mPositionX -= 0.1;
				pBd->mVelo_x = l_velo_x_f;
				pBd->mFacingUp = false;
				pBd->mFacingDown = false;
				pBd->mFacingRight = false;
				pBd->mFacingLeft = true;
				(*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mRotationZ = -90;
				if (pAn)
					pAn->ChangeCurrentStatus("run", 1, 21 / 30.0f);
			}
			if ((DXInputManager::GetInstance()->isKeyPressed(DIK_D) || DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_RIGHT)) && DXInputManager::GetInstance()->isKeyTrigger(DIK_SPACE) && pBd->body_State == ON_SURFACE)
			{
				pBd->mDownMovement = false;
				pBd->mLeftMovement = false;
				pBd->mUpMovement = false;
				pBd->mVelo_x = r_velo_x_space;
				pBd->mFacingUp = false;
				pBd->mFacingDown = false;
				pBd->mFacingRight = true;
				pBd->mFacingLeft = false;
			}
			else if ((DXInputManager::GetInstance()->isKeyPressed(DIK_D) || DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_RIGHT)) && pBd->mRightMovement == true && pBd->body_State && rolling == false && hx->mFrameCounter <= 0 && hv->mFrameCounter <= 0)
			{
				//pTr->mPositionX += 0.1;
				pBd->mVelo_x = r_velo_x_f;
				pBd->mFacingUp = false;
				pBd->mFacingDown = false;
				pBd->mFacingRight = true;
				pBd->mFacingLeft = false;
				(*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mRotationZ = 90;
				if (pAn)
					pAn->ChangeCurrentStatus("run", 1, 21 / 30.0f);
			}
			if ((DXInputManager::GetInstance()->isKeyPressed(DIK_D) || DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_RIGHT)) && pBd->mRightMovement == true && pBd->body_State == FALLING)
			{
				//pTr->mPositionX += 0.1;
				pBd->mDownMovement = false;
				pBd->mLeftMovement = false;
				pBd->mUpMovement = false;
				pBd->mVelo_x = r_velo_x_s;
				pBd->mFacingUp = false;
				pBd->mFacingDown = false;
				pBd->mFacingRight = true;
				pBd->mFacingLeft = false;
			}
			if (DXInputManager::GetInstance()->isKeyTrigger(DIK_F) || DXInputManager::GetInstance()->GamepadTrigger(GAMEPAD_X))
			{
				ObjectFactoryManager::GetInstance()->DropBox("Block");
			}

			if (DXInputManager::GetInstance()->isKeyTrigger(DIK_DELETE))
			{
				//ObjectFactoryManager::GetInstance()->DeleteRunTimeObject();
			}
			/*if (DXInputManager::GetInstance()->isKeyPressed(DIK_1))
			{
				if (pAn)
					pAn->ChangeCurrentStatus(stop_animation, speed, StopTime);

			}*/
			if ((DXInputManager::GetInstance()->isKeyPressed(DIK_W) && DXInputManager::GetInstance()->isKeyPressed(DIK_A) || DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_UP) && DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_LEFT)) && rolling == false && hx->mFrameCounter <= 0 && hv->mFrameCounter <= 0)
			{
				pBd->mFacingUp = true;
				pBd->mFacingLeft = true;
				(*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mRotationZ = -135;
				if (pAn)
					pAn->ChangeCurrentStatus("run", 1, 21 / 30.0f);
			}
			if ((DXInputManager::GetInstance()->isKeyPressed(DIK_W) && DXInputManager::GetInstance()->isKeyPressed(DIK_D) || DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_UP) && DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_RIGHT)) && rolling == false && hx->mFrameCounter <= 0 && hv->mFrameCounter <= 0)
			{
				pBd->mFacingUp = true;
				pBd->mFacingRight = true;
				(*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mRotationZ = 135;
				if (pAn)
					pAn->ChangeCurrentStatus("run", 1, 21 / 30.0f);
			}
			if ((DXInputManager::GetInstance()->isKeyPressed(DIK_S) && DXInputManager::GetInstance()->isKeyPressed(DIK_A) || DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_DOWN) && DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_LEFT)) && rolling == false && hx->mFrameCounter <= 0 && hv->mFrameCounter <= 0)
			{
				pBd->mFacingDown = true;
				pBd->mFacingLeft = true;
				(*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mRotationZ = -45;
				if (pAn)
					pAn->ChangeCurrentStatus("run", 1, 21 / 30.0f);
			}
			if ((DXInputManager::GetInstance()->isKeyPressed(DIK_S) && DXInputManager::GetInstance()->isKeyPressed(DIK_D) || DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_DOWN) && DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_RIGHT)) && rolling == false && hx->mFrameCounter <= 0 && hv->mFrameCounter <= 0)
			{
				pBd->mFacingDown = true;
				pBd->mFacingRight = true;
				(*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mRotationZ = 45;
				if (pAn)
					pAn->ChangeCurrentStatus("run", 1, 21 / 30.0f);
			}
			if ((DXInputManager::GetInstance()->isMouseTrigger(0) || DXInputManager::GetInstance()->GamepadTrigger(GAMEPAD_LEFTSHOULDER)) && lock_key != -1)
			{
				int begin = ObjectFactoryManager::GetInstance()->start;
				for (int i = begin; i < ObjectFactoryManager::GetInstance()->start + 20; i++)
				{
					if ((*MemoryManager::GetInstance()->ObjectPool)[i].pBd->active && (*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mVelo_x == 0 && (*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mVelo_y == 0)//if active fire.
					{
						Bullet* b = static_cast<Bullet*>((*MemoryManager::GetInstance()->ObjectPool)[i].pEn);
						b->framecounter = 0;
						ObjectFactoryManager::GetInstance()->FireBullet(pBd, i, lock_key);
						AudioManager::GetInstance()->LoadAudio("Small Fireball-SoundBible.com.wav");
						break;
					}
					else if ((*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mVelo_x == 0 && (*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mVelo_y == 0)
					{
						Bullet* b = static_cast<Bullet*>((*MemoryManager::GetInstance()->ObjectPool)[i].pEn);
						b->framecounter = 0;
						(*MemoryManager::GetInstance()->ObjectPool)[i].pBd->active = true;
						ObjectFactoryManager::GetInstance()->FireBullet(pBd, i, lock_key);
						break;
					}
				}
			}
			if ((DXInputManager::GetInstance()->isKeyTrigger(DIK_SPACE) || DXInputManager::GetInstance()->GamepadTrigger(GAMEPAD_X)) && rolling == false && hx->resetcounter <= 0 && (hv->resetcounter - 30) <= 0)
			{
				pressed_left = true;
				Attack(pBd, 1);
				pAn->SetCurrentStatus("hatk", 3, 26 / 30.0f);
				AudioManager::GetInstance()->LoadAudio("Realistic_Punch-Mark_DiAngelo.wav");

			}
			if ((DXInputManager::GetInstance()->isKeyTrigger(DIK_E) || DXInputManager::GetInstance()->GamepadTrigger(GAMEPAD_Y)) && rolling == false && (hx->resetcounter - 30) <= 0 && hv->resetcounter <= 0)
			{
				Attack(pBd, 2);
				AudioManager::GetInstance()->LoadAudio("Realistic_Punch-Mark_DiAngelo.wav");
				pAn->SetCurrentStatus("vatk", 2, 38 / 30.0f);
			}
			if (DXInputManager::GetInstance()->isKeyTrigger(DIK_END))
			{
				MemoryManager::GetInstance()->mpOwner->ChangeLevel(MemoryManager::GetInstance()->mpOwner->mNextLevel, true);

			}
			if (DXInputManager::GetInstance()->isKeyTrigger(DIK_1))
			{
				pl->ToggleGodMode();

			}
			if (DXInputManager::GetInstance()->isKeyTrigger(DIK_2))
			{
				pl->ToggleOneHitKill();
			}
			if (DXInputManager::GetInstance()->isKeyTrigger(DIK_3))
			{
				MemoryManager::GetInstance()->mpOwner->ChangeLevel("Level4", true);

			}
			if (DXInputManager::GetInstance()->isKeyTrigger(DIK_4))
			{
				MemoryManager::GetInstance()->mpOwner->ChangeLevel("Level3", true);

			}

			if (abs(pBd->mVelo_x) <= epsilon && abs(pBd->mVelo_y) <= epsilon && pl->mDashCounter <= 0 && hx->resetcounter <= 0 && hv->resetcounter <= 0)
			{

				pAn->ChangeCurrentStatus("idle", 1, 59.0f / 30.0f);
			}
			if (pl->mDashCounter <= 0)
			{
				rolling = false;
			}
		}
	}
}

void Controller::Attack(Body* ptr, int atkm)
{
	if (atkm == 2)
	{
		Right* hx = static_cast<Right*>((*MemoryManager::GetInstance()->ObjectPool)[0].pEn);
		hx->mFrameCounter = 30;
		hx->resetcounter = 30;
		hx->mHit = false;
		(*MemoryManager::GetInstance()->ObjectPool)[0].pBd->active = true;
		if (ptr->mFacingDown == true)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[0].pTr->mPositionX = ptr->mPos_x - 0.25;
			(*MemoryManager::GetInstance()->ObjectPool)[0].pTr->mPositionY = ptr->mPos_y - 1;
			(*MemoryManager::GetInstance()->ObjectPool)[0].pTr->mPositionZ = ptr->mPos_z;
		}
		else if (ptr->mFacingLeft == true)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[0].pTr->mPositionX = ptr->mPos_x - 1;
			(*MemoryManager::GetInstance()->ObjectPool)[0].pTr->mPositionY = ptr->mPos_y + 0.25;
			(*MemoryManager::GetInstance()->ObjectPool)[0].pTr->mPositionZ = ptr->mPos_z;
		}
		else if (ptr->mFacingRight == true)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[0].pTr->mPositionX = ptr->mPos_x + 1;
			(*MemoryManager::GetInstance()->ObjectPool)[0].pTr->mPositionY = ptr->mPos_y - 0.25;
			(*MemoryManager::GetInstance()->ObjectPool)[0].pTr->mPositionZ = ptr->mPos_z;
		}
		else if (ptr->mFacingUp == true)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[0].pTr->mPositionX = ptr->mPos_x + 0.25;
			(*MemoryManager::GetInstance()->ObjectPool)[0].pTr->mPositionY = ptr->mPos_y + 1;
			(*MemoryManager::GetInstance()->ObjectPool)[0].pTr->mPositionZ = ptr->mPos_z;
		}
		(*MemoryManager::GetInstance()->ObjectPool)[0].pBd->mPos_x = (*MemoryManager::GetInstance()->ObjectPool)[0].pTr->mPositionX;
		(*MemoryManager::GetInstance()->ObjectPool)[0].pBd->mPos_y = (*MemoryManager::GetInstance()->ObjectPool)[0].pTr->mPositionY;
		(*MemoryManager::GetInstance()->ObjectPool)[0].pBd->mPos_z = (*MemoryManager::GetInstance()->ObjectPool)[0].pTr->mPositionZ;
	}
	else if (atkm == 1)
	{
		Left* hx = static_cast<Left*>((*MemoryManager::GetInstance()->ObjectPool)[1].pEn);
		hx->resetcounter = 15;
		hx->mFrameCounter = 10;
		hx->mHit = false;
		//hx->speedcounter = 120 + 180 / speed_index;
		(*MemoryManager::GetInstance()->ObjectPool)[1].pBd->active = true;
		if (ptr->mFacingDown == true)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[1].pTr->mPositionX = ptr->mPos_x + 0.5;
			(*MemoryManager::GetInstance()->ObjectPool)[1].pTr->mPositionY = ptr->mPos_y - 1;
			(*MemoryManager::GetInstance()->ObjectPool)[1].pTr->mPositionZ = ptr->mPos_z;
		}
		else if (ptr->mFacingLeft == true)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[1].pTr->mPositionX = ptr->mPos_x - 1;
			(*MemoryManager::GetInstance()->ObjectPool)[1].pTr->mPositionY = ptr->mPos_y - 0.5;
			(*MemoryManager::GetInstance()->ObjectPool)[1].pTr->mPositionZ = ptr->mPos_z;
		}
		else if (ptr->mFacingRight == true)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[1].pTr->mPositionX = ptr->mPos_x + 1;
			(*MemoryManager::GetInstance()->ObjectPool)[1].pTr->mPositionY = ptr->mPos_y + 0.5;
			(*MemoryManager::GetInstance()->ObjectPool)[1].pTr->mPositionZ = ptr->mPos_z;
		}
		else if (ptr->mFacingUp == true)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[1].pTr->mPositionX = ptr->mPos_x - 0.5;
			(*MemoryManager::GetInstance()->ObjectPool)[1].pTr->mPositionY = ptr->mPos_y + 1;
			(*MemoryManager::GetInstance()->ObjectPool)[1].pTr->mPositionZ = ptr->mPos_z;
		}
		(*MemoryManager::GetInstance()->ObjectPool)[1].pBd->mPos_x = (*MemoryManager::GetInstance()->ObjectPool)[1].pTr->mPositionX;
		(*MemoryManager::GetInstance()->ObjectPool)[1].pBd->mPos_y = (*MemoryManager::GetInstance()->ObjectPool)[1].pTr->mPositionY;
		(*MemoryManager::GetInstance()->ObjectPool)[1].pBd->mPos_z = (*MemoryManager::GetInstance()->ObjectPool)[1].pTr->mPositionZ;

	}
}

void Controller::HandleEvent(Event* EventToHandle)
{
	if (EventToHandle->mEventType == EVENT_TYPE::MOVE_EVENT)
	{
		MoveEvent* EV = static_cast<MoveEvent*>(EventToHandle);
		EV->DoMovement(this);
	}
}