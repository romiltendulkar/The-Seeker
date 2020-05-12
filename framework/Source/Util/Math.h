#ifndef MATH_H
#define MATH_H
#include "../pch.h"
const float pi = 3.14159f;

class VEC2
{
public:
	float V[2];
	VEC2()
	{
		V[0] = 0; V[1] = 0;
	}
	VEC2(float x, float y)
	{
		V[0] = x; V[1] = y;
	}
	VEC2(float a)
	{
		V[0] = V[1] = a;
	}
	VEC2 operator-(const VEC2 in) {
		VEC2 out; out.V[0] = V[0] - in.V[0];
		out.V[1] = V[1] - in.V[1]; return out;
	}
	VEC2 operator+(const VEC2 in) {
		VEC2 out; out.V[0] = V[0] + in.V[0];
		out.V[1] = V[1] + in.V[1]; return out;
	}
	VEC2 operator*(const float in) {
		VEC2 out; out.V[0] = V[0] * in;
		out.V[1] = V[1] * in; return out;
	}
	VEC2 operator/(const float in) {
		VEC2 out; out.V[0] = V[0] / in;
		out.V[1] = V[1] / in; return out;
	}
	float& operator[](const int i) { return V[i]; }
	const float& operator[](const int i) const { return V[i]; }
};
class IVEC3
{
public:
	int V[3];
	IVEC3()
	{
		V[0] = 0; V[1] = 0; V[2] = 0;
	}
	IVEC3(int x, int y, int z)
	{
		V[0] = x; V[1] = y; V[2] = z;
	}
	IVEC3(int a)
	{
		V[0] = V[1] = a; V[2] = a;
	}
	IVEC3 operator-(const IVEC3 in) {
		IVEC3 out; out.V[0] = V[0] - in.V[0];
		out.V[1] = V[1] - in.V[1];
		out.V[2] = V[2] - in.V[2]; return out;
	}
	IVEC3 operator+(const IVEC3 in) {
		IVEC3 out; out.V[0] = V[0] + in.V[0];
		out.V[1] = V[1] + in.V[1];
		out.V[2] = V[2] + in.V[2]; return out;
	}
	IVEC3 operator*(const float in) {
		IVEC3 out; out.V[0] = V[0] * int(in);
		out.V[1] = V[1] * int(in);
		out.V[2] = V[2] * int(in); return out;
	}
	IVEC3 operator/(const float in) {
		IVEC3 out; out.V[0] = V[0] / int(in);
		out.V[1] = V[1] / int(in);
		out.V[2] = V[2] / int(in); return out;
	}
	int& operator[](const int i) { return V[i]; }
	const int& operator[](const int i) const { return V[i]; }
};
class VEC3
{
public:
	float V[3];
	VEC3()
	{
		for (int i = 0; i < 3; i++)
		{
			V[i] = 0;
		}
	}
	VEC3(float x, float y, float z)
	{
		V[0] = x; V[1] = y; V[2] = z;
	}
	VEC3(float a)
	{
		V[0] = V[1] = V[2] = a;
	}
	VEC3 operator-(const VEC3 in) {
		VEC3 out; out.V[0] = V[0] - in.V[0];
		out.V[1] = V[1] - in.V[1];
		out.V[2] = V[2] - in.V[2]; return out;
	}
	VEC3 operator+(const VEC3 in) {
		VEC3 out; out.V[0] = V[0] + in.V[0];
		out.V[1] = V[1] + in.V[1];
		out.V[2] = V[2] + in.V[2]; return out;
	}
	VEC3 operator*(const float in) {
		VEC3 out; out.V[0] = V[0] * in;
		out.V[1] = V[1] * in;
		out.V[2] = V[2] * in; return out;
	}
	VEC3 operator/(const float in) {
		VEC3 out; out.V[0] = V[0] / in;
		out.V[1] = V[1] / in;
		out.V[2] = V[2] / in; return out;
	}
	float& operator[](const int i) { return V[i]; }
	const float& operator[](const int i) const { return V[i]; }
};

class VEC4
{
public:
	float V[4];
	VEC4()
	{
		for (int i = 0; i < 4; i++)
		{
			V[i] = 0;
		}
	}
	VEC4(float x, float y, float z, float w)
	{
		V[0] = x; V[1] = y; V[2] = z; V[3] = w;
	}
	VEC4(float a)
	{
		V[0] = V[1] = V[2] = V[3] = a;
	}
	VEC4 operator-(const VEC4 in) {
		VEC4 out; out.V[0] = V[0] - in.V[0];
		out.V[1] = V[1] - in.V[1];
		out.V[2] = V[2] - in.V[2];
		out.V[3] = V[3] - in.V[3]; return out;
	}
	VEC4 operator+(const VEC4 in) {
		VEC4 out; out.V[0] = V[0] + in.V[0];
		out.V[1] = V[1] + in.V[1];
		out.V[2] = V[2] + in.V[2];
		out.V[3] = V[3] + in.V[3]; return out;
	}
	VEC4 operator*(const float in) {
		VEC4 out; out.V[0] = V[0] * in;
		out.V[1] = V[1] * in;
		out.V[2] = V[2] * in;
		out.V[3] = V[3] * in; return out;
	}
	VEC4 operator/(const float in) {
		VEC4 out; out.V[0] = V[0] / in;
		out.V[1] = V[1] / in;
		out.V[2] = V[2] / in;
		out.V[3] = V[3] / in; return out;
	}
	float& operator[](const int i) { return V[i]; }
	const float& operator[](const int i) const { return V[i]; }

};

class MAT4
{
public:
	float M[4][4];
	//identity
	typedef float ROW4[4];
	MAT4()
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				M[i][j] = i == j ? 1.0f : 0.0f;
	}
	MAT4(const float *pArray)
	{
		M[0][0] = pArray[0];
		M[0][1] = pArray[1];
		M[0][2] = pArray[2];
		M[0][3] = pArray[3];

		M[1][0] = pArray[4];
		M[1][1] = pArray[5];
		M[1][2] = pArray[6];
		M[1][3] = pArray[7];

		M[2][0] = pArray[8];
		M[2][1] = pArray[9];
		M[2][2] = pArray[10];
		M[2][3] = pArray[11];

		M[3][0] = pArray[12];
		M[3][1] = pArray[13];
		M[3][2] = pArray[14];
		M[3][3] = pArray[15];
	}
	ROW4& operator[](const int i) { return M[i]; }
	const ROW4& operator[](const int i) const { return M[i]; }
	MAT4(const MAT4& q) {
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				M[i][j] = q[i][j];
			}
		}
	}
	MAT4& operator=(const MAT4& q) {
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				M[i][j] = q[i][j];
			}
		}
		return *this;
	}
	float* Pntr();
	void print();
};
class MyQuaternion
{
public:
	MyQuaternion();
	MyQuaternion(float is, float ix, float iy, float iz);
	MyQuaternion(float scalar, const Vector3& v);
	MyQuaternion(const Vector4& v);
	MyQuaternion(const float *pArray);

	MyQuaternion(const MyQuaternion& q);
	MyQuaternion& operator=(const MyQuaternion& q);

	MyQuaternion(MyQuaternion&& q);
	MyQuaternion& operator=(MyQuaternion&& q);

	operator Vector4() const;

	// Comparison operators
	bool operator == (const MyQuaternion& q) const;
	bool operator != (const MyQuaternion& q) const;

	// Assignment operators
	MyQuaternion& operator= (const Vector4& F);
	MyQuaternion& operator+= (const MyQuaternion& q);
	MyQuaternion& operator-= (const MyQuaternion& q);
	MyQuaternion& operator*= (const MyQuaternion& q);
	MyQuaternion& operator*= (float S);
	MyQuaternion& operator/= (const MyQuaternion& q);

	// Unary operators
	MyQuaternion operator+ () const;
	MyQuaternion operator- () const;

	// MyQuaternion operations
	float Length() const;
	float LengthSquared() const;

	void Normalize();
	void Normalize(MyQuaternion& result) const;

	void Rotate(Vector3);
	void ToMatrix(Matrix&);
	Matrix ToMatrix();

	void Conjugated();
	MyQuaternion Conjugate();
	void Inversed();
	MyQuaternion Inverse() const;

	float Dot(const MyQuaternion& Q) const;

	// Static functions
	//static MyQuaternion CreateFromAxisAngle(const Vector3& axis, float angle);
	//static MyQuaternion CreateFromYawPitchRoll(float yaw, float pitch, float roll);
	//static MyQuaternion CreateFromRotationMatrix(const Matrix& M);

	//static void Lerp(float t, const MyQuaternion& q1, const MyQuaternion& q2, MyQuaternion& result);
	//static MyQuaternion Lerp(float t, const MyQuaternion& q1, const MyQuaternion& q2);

	static void Slerp(float t, const MyQuaternion& Q1, const MyQuaternion& Q2, MyQuaternion& result);
	static MyQuaternion Slerp(float t, const MyQuaternion& Q1, const MyQuaternion& Q2);

	//static void Concatenate(const MyQuaternion& q1, const MyQuaternion& q2, MyQuaternion& result);
	//static MyQuaternion Concatenate(const MyQuaternion& q1, const MyQuaternion& q2);

	// Constants
	static const MyQuaternion Identity;

	float s; float x; float y; float z;
	float Angle;
	Vector3 Axis;
};

// Binary operators
MyQuaternion operator+ (const MyQuaternion& Q1, const MyQuaternion& Q2);
MyQuaternion operator- (const MyQuaternion& Q1, const MyQuaternion& Q2);
MyQuaternion operator* (const MyQuaternion& Q1, const MyQuaternion& Q2);
MyQuaternion operator* (const MyQuaternion& Q, float S);
MyQuaternion operator/ (const MyQuaternion& Q1, const MyQuaternion& Q2);
MyQuaternion operator* (float S, const MyQuaternion& Q);

Matrix MyMul(const Matrix& a, const Matrix& b);
Matrix MyRotate(const int i, const float theta);
Matrix MyScale(const float x, const float y, const float z);
Matrix MyScale(const Vector3& a);
Matrix MyTranslate(const float x, const float y, const float z);
Matrix MyTranslate(const Vector3& a);
Vector4 MyVectorTransform(const Matrix& a, const Vector4& b);
Vector3 MyVectorTransform(const Matrix & a, const Vector3 & b);
//MAT4 Perspective(const float rx, const float ry,
//	const float front, const float back);
//MAT4 MyLookAt(const float cameraX, const float cameraY, const float cameraZ,
//	const float centerX, const float centerY, const float centerZ,
//	const float cameraUpX, const float cameraUpY, const float cameraUpZ);

Matrix MyLookAt(Vector3 rpos, Vector3 rlook_dir, Vector3 rup);
Vector3 PerpendicularFinder(Vector3 rvector);
Matrix MyPerspective(const float rx, const float ry,
	const float front, const float back);

MAT4 operator* (const MAT4 A, const MAT4 B);
Vector3 MyCross(Vector3 a, Vector3 b);
Vector3 MyNormalize(Vector3 x);
Vector4 MyNormalize(Vector4 x);
float MyDot(Vector3 a, Vector3 b);
float MyDot(MyQuaternion a, MyQuaternion b);
float MyDot(Vector4 a, Vector4 b);
bool invert(const MAT4* mat, MAT4* inv);


/**
	Translation LERP
*/
Vector3 TranslationLERP(Vector3 v0, Vector3 v1, float t);

/**
	Scale LERP
*/
Vector3 ScaleLERP(Vector3 s0, Vector3 s1, float t);


Matrix MMatrixOrthoRH(float fWidth, float fHeight, float fNear, float fFar);


/**
helper function for Random Float
*/
float RandomFloatRange(float hi, float low);
float UniformRandom(float hi, float low);

#endif 1
