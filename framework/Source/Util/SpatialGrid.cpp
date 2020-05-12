/**
 * @file SpatialGrid.cpp
 * @author Romil Tendulkar
 * @date 11/05/2019
 * @brief This class implements the spatial partitioning data structure itself and
			the functions necessary for its upkeep.
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#include "../pch.h"
#include "SpatialGrid.h"

/**
	This function initializes the spatial partitioning data structure

	\param WorldS | The Scale of the world, define this as the length of an axis in one direction
	\param Parts  | The total partitions to divide the world space in
*/
SpatialGrid::SpatialGrid(float WorldS, int Parts)
{
	worldScale = WorldS;
	partitions = Parts;
	if (partitions == 0)
	{
		partitions = 16;
	}
	if (worldScale == 0)
	{
		worldScale = 100;
	}
	cellWidth = WorldS / (partitions / 4);
	
}

/**
	This function deletes the spatial grid and all data therein
*/
SpatialGrid::~SpatialGrid()
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			Cells[i][j].data.clear();
		}
	}
}

/**
	This function assigns an object to a specific cell in the grid based on its geometry

	\param pShape | The pointer to the shape for object, this is present in the body component
	\param posX | The x position of the object
	\param posY | the y position of the object
*/
void SpatialGrid::AssignCell(ShapeX* pShape, float posX, float posY)
{
	//This function is used to assign cells for the body during its update
	ShapeAABB* pAABB = static_cast<ShapeAABB*>(pShape);
	float mBottom = posY - pAABB->pOwnerBody->mScale_y;
	float mTop = posY + pAABB->pOwnerBody->mScale_y;

	float mLeft = posX - pAABB->pOwnerBody->mScale_x;
	float mRight = posX + pAABB->pOwnerBody->mScale_x;

	//----------------------------------------------------------------
	//----------------------------------------------------------------
	//for centre position
	float Px, Py;
	/*int gridx, gridy;
	Px = posX / cellWidth;
	if (Px < 0)
	{
		gridx = abs(floor(Px));

	}
	else
	{
		gridx = 2 + ceil(Px);
	}
	Py = posY / cellWidth;
	if (Py < 0)
	{
		gridy = abs(floor(Py));
	}
	else
	{
		gridy = 2 + ceil(Py);
	}
	Cells[gridx][gridy].data.push_back(pAABB->pOwnerBody);*/

	//-------------------------------------------------------------
	//-------------------------------------------------------------
	// min check
	int minx, miny, maxx, maxy;
	Px = mLeft / cellWidth;
	if (Px < 0)
	{
		minx = (int)abs(floor(Px));

	}
	else
	{
		minx = 2 + (int)ceil(Px);
	}
	Py = mBottom / cellWidth;
	if (Py < 0)
	{
		miny = (int)abs(floor(Py));
	}
	else
	{
		miny = 2 + (int)ceil(Py);
	}

	//-------------------------------------------------------------
	//-------------------------------------------------------------
	// max check

	Px = mRight / cellWidth;
	if (Px < 0)
	{
		maxx = (int)abs(floor(Px));

	}
	else
	{
		maxx = 2 + (int)ceil(Px);
	}
	Py = mTop / cellWidth;
	if (Py < 0)
	{
		maxy = (int)abs(floor(Py));
	}
	else
	{
		maxy = 2 + (int)ceil(Py);
	}

	//-------------------------------------------------------------
	//-------------------------------------------------------------
	// assign to cells
	
	for (int i = minx; i < maxx; ++i)
	{
		for (int j = miny; j < maxy; ++j)
		{
			Cells[i - 1][j - 1].data.push_back(pAABB->pOwnerBody);
		}
	}

}