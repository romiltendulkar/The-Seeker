/**
 * @file CollisionManager.cpp
 * @author Vineet Dogra, Romil Tendulkar
 * @date 09/26/2019
 * @brief This class implements the various collision detection functions that are needed
		As well as the data structure needed for spatial partitioning
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#include "../Source/pch.h"
#include "CollisionManager.h"
#include "../Manager/MemoryManager.h"
#include "..\Source\Util\ShapeX.h"
#include <math.h>


Contact::Contact()
{
	mpBodies[0] = mpBodies[1] = nullptr;
}

Contact::~Contact()
{

}

CollisionManager* CollisionManager::GetInstance()
{
	if (mInstance == nullptr)
	{
		mInstance = new CollisionManager();
	}
	return mInstance;
}

CollisionManager::CollisionManager()
{
	mSpatial = new SpatialGrid(100, 16);
}

CollisionManager::~CollisionManager()
{
}

void CollisionManager::Reset()
{
	//std::cout << "number of freed collisions: " << mContacts.size() << std::endl;
	for (Contact* c : mContacts)
	{
		delete c;
	}

	mContacts.clear();
}

bool CollisionManager::CheckCollisionCuboidCuboid(ShapeX* pShape1, float pos1x, float pos1y, float pos1z, ShapeX* pShape2, float pos2x, float pos2y,
	float pos2z)
{

	ShapeAABB* pAABB1 = static_cast<ShapeAABB*>(pShape1);
	ShapeAABB* pAABB2 = static_cast<ShapeAABB*>(pShape2);


	// Get the half dimension values for each cube
	float halfWidth1 = pAABB1->pOwnerBody->mScale_x;
	float halfWidth2 = pAABB2->pOwnerBody->mScale_x;

	float halfHeight1 = pAABB1->pOwnerBody->mScale_y;
	float halfHeight2 = pAABB2->pOwnerBody->mScale_y;

	float halfDepth1 = pAABB1->pOwnerBody->mScale_z;
	float halfDepth2 = pAABB2->pOwnerBody->mScale_z;

	// Update positions with possible offsets
	float xOffset1 = ((pAABB1->mLeft + pAABB1->mRight) / 2.0f);
	float xOffset2 = ((pAABB2->mLeft + pAABB2->mRight) / 2.0f);
	float yOffset1 = ((pAABB1->mBottom + pAABB1->mTop) / 2.0f);
	float yOffset2 = ((pAABB2->mBottom + pAABB2->mTop) / 2.0f);

	Vector3x Ax, Ay, Az, Bx, By, Bz;
	Vector3x T;
	Vector3x PA, PB;
	Ax.x = cos(pAABB1->pOwnerBody->mAng_x);
	Ax.y = Ax.z = 0;
	Ay.y = cos(pAABB1->pOwnerBody->mAng_y);
	Ay.x = Ay.z = 0;
	Az.z = cos(pAABB1->pOwnerBody->mAng_z);
	Az.x = Az.y = 0;
	Bx.x = cos(pAABB2->pOwnerBody->mAng_x);
	Bx.y = Bx.z = 0;
	By.y = cos(pAABB2->pOwnerBody->mAng_y);
	By.x = By.z = 0;
	Bz.z = cos(pAABB2->pOwnerBody->mAng_z);
	Bz.x = Bz.y = 0;
	PA.x = pos1x;
	PA.y = pos1y;
	PA.z = pos1z;
	//////////////////////////////////////////////////////
	PB.x = pos2x;
	PB.y = pos2y;
	PB.z = pos2z;
	Vector3Zero(&T);
	Vector3Sub(&T, &PA, &PB);//GOT T

	//Case 1
	if (std::abs(Vector3DotProduct(&T, &Ax)) > (halfWidth1 + std::abs(halfWidth2 * Vector3DotProduct(&Bx, &Ax)) + std::abs(halfHeight2 * Vector3DotProduct(&By, &Ax)) +
		std::abs(halfDepth2 * Vector3DotProduct(&Bz, &Ax))))
	{
		return true;
	}

	//Case 2
	if (std::abs(Vector3DotProduct(&T, &Ay)) > (halfHeight1 + std::abs(halfWidth2 * Vector3DotProduct(&Bx, &Ay)) + std::abs(halfHeight2 * Vector3DotProduct(&By, &Ay)) +
		std::abs(halfDepth2 * Vector3DotProduct(&Bz, &Ay))))
	{
		return true;
	}

	//Case 3
	if (std::abs(Vector3DotProduct(&T, &Az)) > (halfDepth1 + std::abs(halfWidth2 * Vector3DotProduct(&Bx, &Az)) + std::abs(halfHeight2 * Vector3DotProduct(&By, &Az)) +
		std::abs(halfDepth2 * Vector3DotProduct(&Bz, &Az))))
	{
		return true;
	}

	//////////////////////////////////////////////////////////////////////
	//Case 4
	if (std::abs(Vector3DotProduct(&T, &Bx)) > (halfWidth2 + std::abs(halfWidth1 * Vector3DotProduct(&Bx, &Ax)) + std::abs(halfHeight1 * Vector3DotProduct(&Ay, &Bx)) +
		std::abs(halfDepth1 * Vector3DotProduct(&Az, &Bx))))
	{
		return true;
	}

	//Case 5
	if (std::abs(Vector3DotProduct(&T, &Ay)) > (halfHeight2 + std::abs(halfWidth1 * Vector3DotProduct(&Ax, &By)) + std::abs(halfHeight1 * Vector3DotProduct(&Ay, &By)) +
		std::abs(halfDepth1 * Vector3DotProduct(&Az, &By))))
	{
		return true;
	}

	//Case 6
	if (std::abs(Vector3DotProduct(&T, &Az)) > (halfDepth2 + std::abs(halfWidth1 * Vector3DotProduct(&Ax, &Bz)) + std::abs(halfHeight1 * Vector3DotProduct(&Ay, &Bz)) +
		std::abs(halfDepth1 * Vector3DotProduct(&Az, &Bz))))
	{
		return true;
	}

	//////////////////////////////////////////////////////////////////////
	//Case 7
	Vector3x AxBx;
	Vector3CrossProduct(&AxBx, &Ax, &Bx);
	if (std::abs(Vector3DotProduct(&T, &AxBx)) > (std::abs(halfHeight1 * Vector3DotProduct(&Bx, &Az)) + std::abs(halfDepth1 * Vector3DotProduct(&Bx, &Ay)) +
		std::abs(halfHeight2 * Vector3DotProduct(&Ax, &Bz)) + std::abs(halfDepth2 * Vector3DotProduct(&Ax, &By))))
	{
		return true;
	}

	//Case 8
	Vector3x AxBy;
	Vector3CrossProduct(&AxBy, &Ax, &By);
	if (std::abs(Vector3DotProduct(&T, &AxBy)) > (std::abs(halfHeight1 * Vector3DotProduct(&Az, &By)) + std::abs(halfDepth1 * Vector3DotProduct(&Ay, &By)) +
		std::abs(halfWidth2 * Vector3DotProduct(&Bz, &Ax)) + std::abs(halfDepth2 * Vector3DotProduct(&Bx, &Ax))))
	{
		return true;
	}

	//Case 9
	Vector3x AxBz;
	Vector3CrossProduct(&AxBz, &Ax, &Bz);
	if (std::abs(Vector3DotProduct(&T, &AxBz)) > (std::abs(halfHeight1 * Vector3DotProduct(&Az, &Bz)) + std::abs(halfDepth1 * Vector3DotProduct(&Ay, &Bz)) +
		std::abs(halfWidth2 * Vector3DotProduct(&By, &Ax)) + std::abs(halfHeight2 * Vector3DotProduct(&Bx, &Ax))))
	{
		return true;
	}

	//////////////////////////////////////////////////////////////////
	//Case10
	Vector3x AyBx;
	Vector3CrossProduct(&AyBx, &Ay, &Bx);
	if (std::abs(Vector3DotProduct(&T, &AyBx)) > (std::abs(halfWidth1 * Vector3DotProduct(&Az, &Bx)) + std::abs(halfDepth1 * Vector3DotProduct(&Ax, &Bx)) +
		std::abs(halfHeight2 * Vector3DotProduct(&Bz, &Ay)) + std::abs(halfDepth2 * Vector3DotProduct(&By, &Ay))))
	{
		return true;
	}

	//Case 11
	Vector3x AyBy;
	Vector3CrossProduct(&AyBy, &Ay, &By);
	if (std::abs(Vector3DotProduct(&T, &AyBy)) > (std::abs(halfWidth1 * Vector3DotProduct(&Az, &By)) + std::abs(halfDepth1 * Vector3DotProduct(&Ax, &By)) +
		std::abs(halfWidth2 * Vector3DotProduct(&Bz, &Ay)) + std::abs(halfDepth2 * Vector3DotProduct(&Bx, &Ay))))
	{
		return true;
	}

	//Case 12
	Vector3x AyBz;
	Vector3CrossProduct(&AyBz, &Ay, &Bz);
	if (std::abs(Vector3DotProduct(&T, &AyBz)) > (std::abs(halfWidth1 * Vector3DotProduct(&Az, &Bz)) + std::abs(halfDepth1 * Vector3DotProduct(&Ax, &Bz)) +
		std::abs(halfWidth2 * Vector3DotProduct(&By, &Ay)) + std::abs(halfHeight2 * Vector3DotProduct(&Bx, &Ay))))
	{
		return true;
	}

	//////////////////////////////////////////////////////////////////
	//Case 13
	Vector3x AzBx;
	Vector3CrossProduct(&AzBx, &Az, &Bx);
	if (std::abs(Vector3DotProduct(&T, &AzBx)) > (std::abs(halfWidth1 * Vector3DotProduct(&Ay, &Bx)) + std::abs(halfHeight1 * Vector3DotProduct(&Ax, &Bx)) +
		std::abs(halfHeight2 * Vector3DotProduct(&Bz, &Az)) + std::abs(halfDepth2 * Vector3DotProduct(&By, &Az))))
	{
		return true;
	}

	//Case 14
	Vector3x AzBy;
	Vector3CrossProduct(&AzBy, &Az, &By);
	if (std::abs(Vector3DotProduct(&T, &AzBy)) > (std::abs(halfWidth1 * Vector3DotProduct(&Ay, &By)) + std::abs(halfHeight1 * Vector3DotProduct(&Ax, &By)) +
		std::abs(halfWidth2 * Vector3DotProduct(&Bz, &Az)) + std::abs(halfDepth2 * Vector3DotProduct(&Bx, &Az))))
	{
		return true;
	}

	//Case 15
	Vector3x AzBz;
	Vector3CrossProduct(&AzBz, &Az, &Bz);
	if (std::abs(Vector3DotProduct(&T, &AzBz)) > (std::abs(halfWidth1 * Vector3DotProduct(&Ay, &Bz)) + std::abs(halfHeight1 * Vector3DotProduct(&Ax, &Bz)) +
		std::abs(halfWidth2 * Vector3DotProduct(&Az, &By)) + std::abs(halfHeight2 * Vector3DotProduct(&Az, &Bx))))
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool CollisionManager::CheckBroadPhase2(ShapeX* pShape1, float pos1x, float pos1y, float pos1z, ShapeX* pShape2, float pos2x, float pos2y, float pos2z)
{
	ShapeAABB* pAABB1 = static_cast<ShapeAABB*>(pShape1);
	float bottom1 = pos1y - pAABB1->pOwnerBody->mScale_y;
	float top1 = pos1y + pAABB1->pOwnerBody->mScale_y;
	float in1 = pos1z + pAABB1->pOwnerBody->mScale_z;
	float out1 = pos1z - pAABB1->pOwnerBody->mScale_z;
	float left1 = pos1x - pAABB1->pOwnerBody->mScale_x;
	float right1 = pos1x + pAABB1->pOwnerBody->mScale_x;

	ShapeAABB* pAABB2 = static_cast<ShapeAABB*>(pShape2);
	float bottom2 = pos1y - pAABB2->pOwnerBody->mScale_y;
	float top2 = pos1y + pAABB2->pOwnerBody->mScale_y;
	float in2 = pos1z + pAABB2->pOwnerBody->mScale_z;
	float out2 = pos1z - pAABB2->pOwnerBody->mScale_z;
	float left2 = pos1x - pAABB2->pOwnerBody->mScale_x;
	float right2 = pos1x + pAABB2->pOwnerBody->mScale_x;

	if (bottom1 > top2 || top1 < bottom2)
	{
		return false;
	}
	if (left1 > right2 || right1 < left2)
	{
		return false;
	}
	if (out1 > in2 || in1 < out2)
	{
		return false;
	}
	else
	{
		return true;
	}
}


Ret CollisionManager::ViewCone(ShapeX* pShape2, float pos2x, float pos2y, float pos2z)
{


	/*
	STEP 1: Retrieve all the vertices for the two shapes.
	*/
	float player_x = (*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mPos_x;
	float player_y = (*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mPos_y;
	float player_z = (*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mPos_z;
	ShapeAABB* pAABB2 = static_cast<ShapeAABB*>(pShape2);
	// Get the half dimension values for enemy
	float halfWidth2 = pAABB2->pOwnerBody->mScale_x;
	float halfHeight2 = pAABB2->pOwnerBody->mScale_z;
	float halfDepth2 = pAABB2->pOwnerBody->mScale_y;

	std::vector<Vector3> Shape_1;//view cone
	std::vector<Vector3> Shape_2;//enemy
	std::vector<vertex> Simplex_Vertices;
	Vector3 spa1, spb1, spa2, spb2, spa3, spb3, spa4, spb4;
	Ret r;
	//BEFORE WE CAN START THE CALCULATION WE REQUIRE THE VERTICES OF THE DIFFERENT SHAPES.

	//LETS START WITH THE FIRST CUBOID.
	Vector3 Center_1 = { player_x, player_y, player_z };
	Vector3 Center_2 = { pos2x, pos2y, pos2z };
	if ((*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mFacingUp && (*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mFacingRight)
	{
		Shape_1.emplace_back((Center_1.x - 5), (Center_1.y + 15), (Center_1.z + 1.25));
		Shape_1.emplace_back((Center_1.x - 5), (Center_1.y + 15), (Center_1.z - 1.25));
		Shape_1.emplace_back((Center_1.x + 15), (Center_1.y - 5), (Center_1.z - 1.25));
		Shape_1.emplace_back((Center_1.x + 15), (Center_1.y - 5), (Center_1.z + 1.25));
		Shape_1.emplace_back((Center_1.x), (Center_1.y), (Center_1.z - 1.25));
		Shape_1.emplace_back((Center_1.x), (Center_1.y), (Center_1.z + 1.25));
	}
	else if ((*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mFacingUp && (*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mFacingLeft)
	{
		Shape_1.emplace_back((Center_1.x + 5), (Center_1.y + 15), (Center_1.z + 1.25));
		Shape_1.emplace_back((Center_1.x + 5), (Center_1.y + 15), (Center_1.z - 1.25));
		Shape_1.emplace_back((Center_1.x - 15), (Center_1.y - 5), (Center_1.z - 1.25));
		Shape_1.emplace_back((Center_1.x - 15), (Center_1.y - 5), (Center_1.z + 1.25));
		Shape_1.emplace_back((Center_1.x), (Center_1.y), (Center_1.z - 1.25));
		Shape_1.emplace_back((Center_1.x), (Center_1.y), (Center_1.z + 1.25));
	}
	else if ((*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mFacingUp)
	{
		Shape_1.emplace_back((Center_1.x), (Center_1.y), (Center_1.z + 1.25));
		Shape_1.emplace_back((Center_1.x), (Center_1.y), (Center_1.z - 1.25));
		Shape_1.emplace_back((Center_1.x + 15), (Center_1.y + 15), (Center_1.z - 1.25));
		Shape_1.emplace_back((Center_1.x + 15), (Center_1.y + 15), (Center_1.z + 1.25));
		Shape_1.emplace_back((Center_1.x - 15), (Center_1.y + 15), (Center_1.z - 1.25));
		Shape_1.emplace_back((Center_1.x - 15), (Center_1.y + 15), (Center_1.z + 1.25));
	}
	else if ((*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mFacingDown && (*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mFacingRight)
	{
		Shape_1.emplace_back((Center_1.x - 5), (Center_1.y - 15), (Center_1.z + 1.25));
		Shape_1.emplace_back((Center_1.x - 5), (Center_1.y - 15), (Center_1.z - 1.25));
		Shape_1.emplace_back((Center_1.x + 15), (Center_1.y + 5), (Center_1.z - 1.25));
		Shape_1.emplace_back((Center_1.x + 15), (Center_1.y + 5), (Center_1.z + 1.25));
		Shape_1.emplace_back((Center_1.x), (Center_1.y), (Center_1.z - 1.25));
		Shape_1.emplace_back((Center_1.x), (Center_1.y), (Center_1.z + 1.25));
	}
	else if ((*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mFacingDown && (*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mFacingLeft)
	{
		Shape_1.emplace_back((Center_1.x + 5), (Center_1.y - 15), (Center_1.z + 1.25));
		Shape_1.emplace_back((Center_1.x + 5), (Center_1.y - 15), (Center_1.z - 1.25));
		Shape_1.emplace_back((Center_1.x - 15), (Center_1.y + 5), (Center_1.z - 1.25));
		Shape_1.emplace_back((Center_1.x - 15), (Center_1.y + 5), (Center_1.z + 1.25));
		Shape_1.emplace_back((Center_1.x), (Center_1.y), (Center_1.z - 1.25));
		Shape_1.emplace_back((Center_1.x), (Center_1.y), (Center_1.z + 1.25));
	}
	else if ((*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mFacingDown)
	{
		Shape_1.emplace_back((Center_1.x), (Center_1.y), (Center_1.z + 1.25));
		Shape_1.emplace_back((Center_1.x), (Center_1.y), (Center_1.z - 1.25));
		Shape_1.emplace_back((Center_1.x + 15), (Center_1.y - 15), (Center_1.z - 1.25));
		Shape_1.emplace_back((Center_1.x + 15), (Center_1.y - 15), (Center_1.z + 1.25));
		Shape_1.emplace_back((Center_1.x - 15), (Center_1.y - 15), (Center_1.z - 1.25));
		Shape_1.emplace_back((Center_1.x - 15), (Center_1.y - 15), (Center_1.z + 1.25));
	}
	else if ((*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mFacingRight)
	{
		Shape_1.emplace_back((Center_1.x), (Center_1.y), (Center_1.z + 1.25));
		Shape_1.emplace_back((Center_1.x), (Center_1.y), (Center_1.z - 1.25));
		Shape_1.emplace_back((Center_1.x + 15), (Center_1.y + 15), (Center_1.z - 1.25));
		Shape_1.emplace_back((Center_1.x + 15), (Center_1.y + 15), (Center_1.z + 1.25));
		Shape_1.emplace_back((Center_1.x + 15), (Center_1.y - 15), (Center_1.z - 1.25));
		Shape_1.emplace_back((Center_1.x + 15), (Center_1.y - 15), (Center_1.z + 1.25));
	}
	else
	{
		Shape_1.emplace_back((Center_1.x), (Center_1.y), (Center_1.z + 1.25));
		Shape_1.emplace_back((Center_1.x), (Center_1.y), (Center_1.z - 1.25));
		Shape_1.emplace_back((Center_1.x - 15), (Center_1.y + 15), (Center_1.z - 1.25));
		Shape_1.emplace_back((Center_1.x - 15), (Center_1.y + 15), (Center_1.z + 1.25));
		Shape_1.emplace_back((Center_1.x - 15), (Center_1.y - 15), (Center_1.z - 1.25));
		Shape_1.emplace_back((Center_1.x - 15), (Center_1.y - 15), (Center_1.z + 1.25));
	}
	//PrintVertices(Shape_1);
	//SECOND CUBOID (MAKE IT INTO A FUNCTION LATER)

	Shape_2.emplace_back((Center_2.x - halfWidth2), (Center_2.y + halfDepth2), (Center_2.z + halfHeight2));//left,inside,up
	Shape_2.emplace_back((Center_2.x + halfWidth2), (Center_2.y + halfDepth2), (Center_2.z + halfHeight2));//right,inside,up
	Shape_2.emplace_back((Center_2.x - halfWidth2), (Center_2.y + halfDepth2), (Center_2.z - halfHeight2));//left,inside,down
	Shape_2.emplace_back((Center_2.x + halfWidth2), (Center_2.y + halfDepth2), (Center_2.z - halfHeight2));//right,inside,down
	Shape_2.emplace_back((Center_2.x - halfWidth2), (Center_2.y - halfDepth2), (Center_2.z + halfHeight2));//left,outside,up
	Shape_2.emplace_back((Center_2.x + halfWidth2), (Center_2.y - halfDepth2), (Center_2.z + halfHeight2));//right,outside,up
	Shape_2.emplace_back((Center_2.x - halfWidth2), (Center_2.y - halfDepth2), (Center_2.z - halfHeight2));//left,outside,down
	Shape_2.emplace_back((Center_2.x + halfWidth2), (Center_2.y - halfDepth2), (Center_2.z - halfHeight2));//down,outside,down
	//PrintVertices(Shape_2);

	//I am taking 3 directions to aid the support function with.
	Vector3 Direction[3];
	Direction[1] = { 1, 0, 0 };
	Direction[0] = { 0.4, 0.67, 0 };
	Direction[2] = { 0, 0, 1 };
	int check = 0;

	//get the first point in the minkowski space.
	Vector3 point1 = Support(Shape_1, Direction[0]);
	Vector3 point2 = Support(Shape_2, Direction[0] * -1);
	Vector3 Simplex1 = point1 - point2;
	spa1 = point1;
	spb1 = point2;
	//std::cout << "point 1 is : " << Simplex1.x << ", " << Simplex1.y << ", " << Simplex1.z << std::endl;
	Direction[0] *= -1;

	//second point with distance*=-1
	point1 = Support(Shape_1, Direction[0]);
	point2 = Support(Shape_2, Direction[0] * -1);
	Vector3 Simplex2 = point1 - point2;
	spa2 = point1;
	spb2 = point2;
	//std::cout << "point 2 is : " << Simplex2.x << ", " << Simplex2.y << ", " << Simplex2.z << std::endl;

	//third point will be in the direction of the normal of the line b/w point 1 and 2 and origin
	Vector3 line1_2 = Simplex2 - Simplex1;
	Vector3 line1_O = Simplex1 * -1;//do check if this is point1 or point2
	Vector3 Normal_To_Origin = line1_2.Cross(line1_O);
	Normal_To_Origin = Normal_To_Origin.Cross(line1_2);
	point1 = Support(Shape_1, Normal_To_Origin);
	point2 = Support(Shape_2, Normal_To_Origin * -1);
	Vector3 Simplex3 = point1 - point2;
	spa3 = point1;
	spb3 = point2;
	//std::cout << "point 3 is : " << Simplex3.x << ", " << Simplex3.y << ", " << Simplex3.z << std::endl;

	//for the fourth point we will need to equation of the line perpendicular to the triangle
	Vector3 Perpendicular = (Simplex3 - Simplex1).Cross(Simplex2 - Simplex1);
	if (Perpendicular.Dot(Simplex1 * -1) < 0)
	{
		Perpendicular *= -1;
	}
	point1 = Support(Shape_1, Perpendicular);
	point2 = Support(Shape_2, Perpendicular * -1);
	Vector3 Simplex4 = point1 - point2;
	spa4 = point1;
	spb4 = point2;
	//std::cout << "point 4 is : " << Simplex4.x << ", " << Simplex4.y << ", " << Simplex4.z << std::endl;
	while (1)
	{
		check += 1;
		if (check > 16)
		{
			Shape_1.clear();
			Shape_2.clear();

			r.collides = false;
			r.Simplex = Simplex_Vertices;
			r.Shape1 = Shape_1;
			r.Shape2 = Shape_2;
			return r;
		}
		//Now we check if the origin is enclosed by the tetrahedron
		Vector3 ver1 = Simplex4 - Simplex1;
		Vector3 ver2 = Simplex4 - Simplex2;
		Vector3 ver3 = Simplex4 - Simplex3;
		Vector3 ver4 = Simplex4 * -1;
		Vector3 nor1 = ver1.Cross(ver2);
		Vector3 nor2 = ver2.Cross(ver3);
		Vector3 nor3 = ver3.Cross(ver1);
		if (nor1.Dot(ver4) > 0)
		{
			Vector3 Dir = nor1;
			Simplex3 = Simplex4;
			spa3 = spa4;
			spb3 = spb4;
			point1 = Support(Shape_1, Dir);
			point2 = Support(Shape_2, Dir * -1);
			Simplex4 = point1 - point2;
			spa4 = point1;
			spb4 = point2;
		}
		else if (nor2.Dot(ver4) > 0)
		{
			Vector3 Dir = nor2;
			Simplex1 = Simplex4;
			spa1 = spa4;
			spb1 = spb4;
			point1 = Support(Shape_1, Dir);
			point2 = Support(Shape_2, Dir * -1);
			Simplex4 = point1 - point2;
			spa4 = point1;
			spb4 = point2;
		}
		else if (nor3.Dot(ver4) > 0)
		{
			Vector3 Dir = nor3;
			Simplex2 = Simplex4;
			spa2 = spa4;
			spb2 = spb4;
			point1 = Support(Shape_1, Dir);
			point2 = Support(Shape_2, Dir * -1);
			Simplex4 = point1 - point2;
			spa4 = point1;
			spb4 = point2;
		}
		else
		{
			//STORE ALL THE VERTICES FOR EPA IMPLEMENTATION
			Simplex_Vertices.emplace_back(Simplex1, spa1, spb1);
			Simplex_Vertices.emplace_back(Simplex2, spa2, spb2);
			Simplex_Vertices.emplace_back(Simplex3, spa3, spb3);
			Simplex_Vertices.emplace_back(Simplex4, spa4, spb4);
			r.collides = true;
			r.Simplex = Simplex_Vertices;
			r.Shape1 = Shape_1;
			r.Shape2 = Shape_2;
			return r;
		}
	}
	r.collides = false;
	r.Simplex = Simplex_Vertices;
	r.Shape1 = Shape_1;
	r.Shape2 = Shape_2;
	return r;
}

Vector3 CollisionManager::Support(std::vector<Vector3>& Vec, Vector3 Direction)
{
	std::vector<Vector3>::iterator itr = Vec.begin();
	float dot_product = -9999999;
	Vector3 Point;
	float newdot_product = 0;
	while (itr != Vec.end())
	{
		newdot_product = (*itr).Dot(Direction);
		if (newdot_product > dot_product)
		{
			Point = *itr;
			dot_product = newdot_product;
		}
		++itr;
	}
	return Point;
}

void CollisionManager::ResetGrid()
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			mSpatial->Cells[i][j].data.clear();
		}
	}
}