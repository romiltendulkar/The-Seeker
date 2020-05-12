/**
 * @file SpatialGrid.h
 * @author Romil Tendulkar
 * @date 11/05/2019
 * @brief This class implements the spatial partitioning data structure itself and
			the functions necessary for its upkeep.
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */

#pragma once

#include "../pch.h"
#include "ShapeX.h"
#include "..\Components\Body.h"

struct SpatialCell
{
	std::vector<Body*>data;
};

class SpatialGrid
{
public:
	SpatialGrid(float worldScale, int partitions);
	~SpatialGrid();
	void AssignCell(ShapeX *pShape, float posx, float posy);
	SpatialCell Cells[4][4];
	float worldScale;
	int partitions;
	float cellWidth;
};
