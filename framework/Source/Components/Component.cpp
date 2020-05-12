/**
 * @file Component.h
 * @authors  Pratyush Gawai
 * @date 9/21/19
 * @brief Abstract component class
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#include "../pch.h"
#include "Component.h"


Component::Component(COMPONENT_TYPE ctype)
{
	mComponentType = ctype;
	CompPosition = -1;
	mpEnowner = nullptr;
}

Component::~Component() {}

