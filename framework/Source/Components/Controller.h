/**
 * @file Controller.cpp
 * @authors Vineet Amarjeet Dogra
 * @date 10/1/19
 * @brief This file governs how the controller component is supposed to work and manages certain scenarios where the player should or should not be able to move.
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "../pch.h"
#include "Component.h"
class Body;
class Controller : public Component
{
public:
	/**
   In this constructor we initialize the variables.
	*/
	Controller();
	~Controller();
	/**
	   In this function we call the input manager to take inputs for the player.

	  \param rdeltaTime | Delta time between frames.
	*/
	void Update(float deltaTime);

	void HandleEvent(Event* EventToHandle);
	/**
	   In this function we serialize the controller, telling the entity it has a controller component.

	  \param  root is used to retreieve value form json file, key is used to pinpoint which node in the memory manager we are working on.
	*/
	void Serialize(Json::Value root, unsigned int key);
	void DeSerialize(Json::Value& root);
	/**
	   In this function .

	  \param  Body* ptr is used to get the body of the hitbox, int atkm tells us vertical or horizontal attack.
	*/
	void Attack(Body* ptr, int atkm);
	int speed_index = 0;
	bool speed_press = false;
	unsigned int key;
	bool debug;
	bool lvle;
	int ammo_check = 0;
	bool usable = true;
	bool lock_check = false;
	int lock_key = -1;
	int auto_lock_key = 0;
	bool rolling = false;
	bool auto_check = false;
	unsigned int frame_counter_auto = -1;
	unsigned int auto_counter = 0;
	bool pressed_left = false;
private:

};
#endif // !CONTROLLER