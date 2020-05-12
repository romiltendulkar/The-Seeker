#pragma once
//#ifndef VECTOR3D_H
//#define VECTOR3D_H

#include "math.h"
//#define EPSILON 0.0001f
#define PI      3.1415926535897932384626433832795

typedef struct Vector3x
{
	float x, y, z;
}Vector3x;




void Vector3Zero(Vector3x* pResult);

void Vector3Set(Vector3x* pResult, float x, float y, float z);

void Vector3Neg(Vector3x* pResult, Vector3x* pVec0);

void Vector3Add(Vector3x* pResult, Vector3x* pVec0, Vector3x* pVec1);

void Vector3Sub(Vector3x* pResult, Vector3x* pVec0, Vector3x* pVec1);

void Vector3Normalize(Vector3x* pResult, Vector3x* pVec0);

void Vector3Scale(Vector3x* pResult, Vector3x* pVec0, float c);

void Vector3ScaleAdd(Vector3x* pResult, Vector3x* pVec0, Vector3x* pVec1, float c);

/*
In this function, pResult will be the vector pVec0 scaled by c and pVec1 will be subtracted from it
*/
void Vector3ScaleSub(Vector3x* pResult, Vector3x* pVec0, Vector3x* pVec1, float c);

/*
This function returns the length of the vector pVec0
*/
float Vector3Length(Vector3x* pVec0);

/*
This function returns the square of pVec0's length. Avoid the square root
*/
float Vector3SquareLength(Vector3x* pVec0);

/*
In this function, pVec0 and pVec1 are considered as 2D points.
The distance between these 2 2D points is returned
*/
float Vector3Distance(Vector3x* pVec0, Vector3x* pVec1);

/*
In this function, pVec0 and pVec1 are considered as 2D points.
The squared distance between these 2 2D points is returned. Avoid the square root
*/
float Vector3SquareDistance(Vector3x* pVec0, Vector3x* pVec1);

/*
This function returns the dot product between pVec0 and pVec1
*/
float Vector3DotProduct(Vector3x* pVec0, Vector3x* pVec1);
/*
This function returns the cross product between pVec0and pVec1
*/
void Vector3CrossProduct(Vector3x* pResult , Vector3x* pVec0, Vector3x* pVec1);

/*
This function computes the coordinates of the vector represented by the angle "angle", which is in Degrees
*/
void Vector3FromAngleDeg(Vector3x* pResult, float angle);

/*
This function computes the coordinates of the vector represented by the angle "angle", which is in Radian
*/
void Vector3FromAngleRad(Vector3x* pResult, float angle);

//#endif
