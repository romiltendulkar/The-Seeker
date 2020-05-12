/**
 * @file CollisionManager.h
 * @author Vineet Dogra, Romil Tendulkar
 * @date 09/26/2019
 * @brief This class implements the various collision detection functions that are needed
		As well as the data structure needed for spatial partitioning
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */

#pragma once

#ifndef EPSILON
#define EPSILON 0.000001
#define EPSILON2 0.000002
#endif
#include "../Source/pch.h"
#include "../Source/Components/Body.h" //not in the engine, please include.
 //#include "shape.h"//not in the engine, please include.
#include "../Source/Util/Vector3D.h"
#include "../Source/Util/SpatialGrid.h"
//#include <list>
//#include <vector>


struct epa
{
	float distance;
	Vector3 Normal;
	Vector3 Point;
};
struct vertex
{
	Vector3  point, support_a, support_b;
	vertex(const Vector3& p, const Vector3& a, const Vector3& b) : point(p), support_a(a), support_b(b)
	{

	}
	vertex() = default;
};
struct Ret
{
	bool collides;
	std::vector<vertex> Simplex;
	std::vector<Vector3> Shape1;
	std::vector<Vector3> Shape2;
};
struct Tri
{
	vertex v1, v2, v3;//coordinates of the triangle.
	Vector3 Normal;
	Tri(const  vertex& v1, const  vertex& v2, const  vertex& v3)
	{
		this->v1 = v1;
		this->v2 = v2;
		this->v3 = v3;
		Normal = (v2.point - v1.point).Cross(v3.point - v1.point);
		Normal.Normalize();
		Normal *= -1;
	}
	void print()
	{
		std::cout << "vertices are: " << std::endl << " 1) " << v1.point.x << ", " << v1.point.y << ", " << v1.point.z << std::endl;
		std::cout << std::endl << " 2) " << v2.point.x << ", " << v2.point.y << ", " << v2.point.z << std::endl;
		std::cout << std::endl << " 3) " << v3.point.x << ", " << v3.point.y << ", " << v3.point.z << std::endl;
	}
};
struct Edge
{
	vertex a, b;//edge ab
	Edge(const vertex& e1, const vertex& e2) :a(e1), b(e2)
	{

	}
};

class Contact
{
public:
	/**
	This constructor initializes the body pointers for the contact object
	*/
	Contact();
	~Contact();
	Body* mpBodies[2];
};

/*expected shapes:
cuboids (cubes are cuboids with equal side lengths)
Spheres*/

class CollisionManager
{
public:

	/**
		This constructor initializes the Spatial Partitioning data structure
	*/
	CollisionManager();
	~CollisionManager();

	/**
	Deletes all the contact objects. Resetting the list of contacts
	*/
	void Reset();

	/**
		This function is used to perform the SAT(Seperating Axis Theorem) test for collision on objects
		This is the first Broad Phase Detection that is performed.

		\param pShape1 | pointer to the shape object present in the body component of the first object
		\param pos1x | The position on the x axis for the first object
		\param pos1y | The position on the y axis for the first object
		\param pos1z | The position on the z axis for the first object

		\param pShape2 | pointer to the shape object present in the body component of the Second object
		\param pos2x | The position on the x axis for the second object
		\param pos2y | The position on the y axis for the second object
		\param pos2z | The position on the z axis for the second object

		\return returns true for no collision and false when collision occurs
	*/
	bool CheckCollisionCuboidCuboid(ShapeX* pShape1, float pos1x, float pos1y, float pos1z, ShapeX* pShape2, float pos2x, float pos2y, float pos2z);

	/**
	TThis function performs the secondary broad phase collision test

	\param pShape1 | pointer to the shape object present in the body component of the first object
	\param pos1x | The position on the x axis for the first object
	\param pos1y | The position on the y axis for the first object
	\param pos1z | The position on the z axis for the first object

	\param pShape2 | pointer to the shape object present in the body component of the Second object
	\param pos2x | The position on the x axis for the second object
	\param pos2y | The position on the y axis for the second object
	\param pos2z | The position on the z axis for the second object

	\return returns true for collision and false for no collision
	*/
	bool CheckBroadPhase2(ShapeX* pShape1, float pos1x, float pos1y, float pos1z, ShapeX* pShape2, float pos2x, float pos2y, float pos2z);
	/*bool CheckCollisionSphereCuboid(Shape* pShape1, float pos1x, float pos1y, Shape* pShape2, float pos2x, float pos2y, std::list<Contact*>& mContacts);
	bool CheckCollisionCuboidSphere(Shape* pShape1, float pos1x, float pos1y, Shape* pShape2, float pos2x, float pos2y, std::list<Contact*>& mContacts);
	bool CheckCollisionSphereSphere(Shape* pShape1, float pos1x, float pos1y, Shape* pShape2, float pos2x, float pos2y, std::list<Contact*>& mContacts);*/
	std::list<Contact*> mContacts;

	/**
	This function creates and returns an instance of Collision Manager.
	If an instance already exists it is returned instead.

	\return returns a static pointer of Collision Manager
	*/
	static CollisionManager* GetInstance();

	/**
		This function resets the spatial partitioning data structure
	*/
	void ResetGrid();

	SpatialGrid* mSpatial;
	void PrintVertices(std::vector<Vector3>&);
	Vector3 Support(std::vector<Vector3>&, Vector3 Direction);
	epa FindClosestEdge(std::vector<vertex>&, std::vector<Vector3>&, std::vector<Vector3>&);
	bool FindOppositeEdge(std::vector<Edge>&, Edge&);
	void Barycentric(const Vector3& p, const  Vector3& a, const Vector3& b, const Vector3& c, float* u, float* v, float* w);
	Ret CollisionManager::ViewCone(ShapeX* pShape2, float pos2x, float pos2y, float pos2z);
private:
	static CollisionManager* mInstance;

};