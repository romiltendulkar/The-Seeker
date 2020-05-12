/**
 * @file PhysicsManager.h
 * @author Romil Tendulkar
 * @date 09/29/2019
 * @brief This class is used to call the integration function of the body for the physics update,
			and to check collisions and assign appropriate response
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */


#pragma once

class GameState;

class PhysicsManager
{
public:
	/**
	This function creates and returns an instance of Physics Manager.
	If an instance already exists it is returned instead.

	\return returns a static pointer of Physics Manager
	*/
	static PhysicsManager* GetInstance();

	/**
		This functions initiates the owner gamestate for the manager as a null pointer.
		The game state functionality hasnt been implemented yet but is needed for gameplay.
	*/
	PhysicsManager();

	/**
		seperate constructor to initialize the owner gamestate with the arguement passed

		\param pOwner | the pointer to the gamestate that wants to initialize the physics manager
	*/
	PhysicsManager(GameState *mpOwner);
	~PhysicsManager();

	/**
	This function updates the body components of the objects first, this is required for physics.
	Then the function performs collision detection. Later proceeding onto collision response
	for all the colliding bodies.

	\param rdeltaTime | The time between passed between frames is sent to perform the calculations
	required for physics
	*/
	void Update(float rdeltaTime);

	/**
		Used to call the destructor on the static pointer to this class.
	*/
	int rayresult = -1;
	void Destroy();
	bool lock_found = false;
	bool Stop = false;
	std::vector<unsigned int> LVisibleEnemies;
	Vector3 ppoint;
	int counter;
private:
	static PhysicsManager* mInstance;
	GameState *pmOwner;
};