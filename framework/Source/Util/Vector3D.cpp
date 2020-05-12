#include "../pch.h"
#include "Vector3D.h"


// ---------------------------------------------------------------------------
void Vector3Zero(Vector3x* pResult)
{
	pResult->x = 0;
	pResult->y = 0;
	pResult->z = 0;
}

// ---------------------------------------------------------------------------

void Vector3Set(Vector3x* pResult, float x, float y, float z)
{
	pResult->x = x;
	pResult->y = y;
	pResult->z = z;
}

// ---------------------------------------------------------------------------

void Vector3Neg(Vector3x* pResult, Vector3x* pVec0)
{
	pResult->x = -pVec0->x;
	pResult->y = -pVec0->y;
	pResult->z = -pVec0->z;
}

// ---------------------------------------------------------------------------

void Vector3Add(Vector3x* pResult, Vector3x* pVec0, Vector3x* pVec1)
{
	pResult->x = pVec0->x + pVec1->x;
	pResult->y = pVec0->y + pVec1->y;
	pResult->z = pVec0->z + pVec1->z;
}

// ---------------------------------------------------------------------------

void Vector3Sub(Vector3x* pResult, Vector3x* pVec0, Vector3x* pVec1)
{
	pResult->x = pVec0->x - pVec1->x;
	pResult->y = pVec0->y - pVec1->y;
	pResult->z = pVec0->z - pVec1->z;
}

// ---------------------------------------------------------------------------

void Vector3Normalize(Vector3x* pResult, Vector3x* pVec0)
{
	float magnitude = Vector3Length(pVec0);
	pResult->x = pVec0->x / magnitude;
	pResult->y = pVec0->y / magnitude;
	pResult->z = pVec0->z / magnitude;
}

// ---------------------------------------------------------------------------

void Vector3Scale(Vector3x* pResult, Vector3x* pVec0, float c)
{
	pResult->x = pVec0->x * c;
	pResult->y = pVec0->y * c;
	pResult->z = pVec0->z * c;
}

// ---------------------------------------------------------------------------

void Vector3ScaleAdd(Vector3x* pResult, Vector3x* pVec0, Vector3x* pVec1, float c)
{
	pResult->x = pVec0->x * c + pVec1->x;
	pResult->y = pVec0->y * c + pVec1->y;
	pResult->z = pVec0->z * c + pVec1->z;
}

// ---------------------------------------------------------------------------

void Vector3ScaleSub(Vector3x* pResult, Vector3x* pVec0, Vector3x* pVec1, float c)
{
	pResult->x = pVec0->x * c - pVec1->x;
	pResult->y = pVec0->y * c - pVec1->y;
	pResult->z = pVec0->z * c - pVec1->z;
}

// ---------------------------------------------------------------------------

float Vector3Length(Vector3x* pVec0)
{
	float z = sqrtf(Vector3SquareLength(pVec0));
	return z;
}

// ---------------------------------------------------------------------------

float Vector3SquareLength(Vector3x* pVec0)
{
	return ((pVec0->x * pVec0->x) + (pVec0->y * pVec0->y) + (pVec0->z * pVec0->z));
}

// ---------------------------------------------------------------------------

float Vector3Distance(Vector3x* pVec0, Vector3x* pVec1)
{
	return sqrtf(Vector3SquareDistance(pVec0, pVec1));
}

// ---------------------------------------------------------------------------

float Vector3SquareDistance(Vector3x* pVec0, Vector3x* pVec1)
{
	float xf = (pVec0->x - pVec1->x);
	xf = xf * xf;
	float yf = (pVec0->y - pVec1->y);
	yf = yf * yf;
	float zf = (pVec0->z - pVec1->z);
	zf = zf * zf;
	return (xf + yf + zf);
}

// ---------------------------------------------------------------------------

float Vector3DotProduct(Vector3x* pVec0, Vector3x* pVec1)
{
	return ((pVec0->x * pVec1->x) + (pVec0->y * pVec1->y) + (pVec0->z * pVec1->z));
}

// ---------------------------------------------------------------------------

void Vector3CrossProduct(Vector3x* pResult, Vector3x* pVec0, Vector3x* pVec1)
{
	pResult->x = (pVec0->y * pVec1->z) - (pVec0->z * pVec1->y);
	pResult->y = -1*((pVec0->x * pVec1->z) - (pVec0->z * pVec1->x));
	pResult->z = (pVec0->x * pVec1->y) - (pVec0->y * pVec1->x);
}

// ---------------------------------------------------------------------------

void Vector3FromAngleDeg(Vector3x* pResult, float angle)
{
	Vector3FromAngleRad(pResult, (float)((angle * PI) / 180));
}

// ---------------------------------------------------------------------------

void Vector3FromAngleRad(Vector3x* pResult, float angle)
{
	pResult->x = pResult->x * cosf(angle);
	pResult->y = pResult->y * sinf(angle);
	pResult->z = pResult->z * sinf(angle);
}