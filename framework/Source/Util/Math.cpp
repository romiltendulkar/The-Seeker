#include "../pch.h"
#include "Math.h"

float * MAT4::Pntr()
{
	return &(M[0][0]);
}

void MAT4::print()
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			printf("%9.4f%c", M[i][j], j == 3 ? '\n' : ' ');
	printf("\n");
}

Matrix MyMul(const Matrix & a, const Matrix & b)
{
	return b*a;
}

Matrix MyRotate(const int i, const float theta)
{
	MAT4 R;
	int j = (i + 1) % 3;
	int k = (j + 1) % 3;
	R[j][j] = R[k][k] = cos(theta*DEGREE_TO_RADIAN);
	R[k][j] = sin(theta*pi / 180.0f);
	R[j][k] = -R[k][j];
	return Matrix(&R.M[0][0]);
}

Matrix MyScale(const float x, const float y, const float z)
{
	Matrix S;
	S._11 = x;
	S._22 = y;
	S._33 = z;
	return S;
}

Matrix MyScale(const Vector3& a)
{
	Matrix S;
	S._11 = a.x;
	S._22 = a.y;
	S._33 = a.z;
	return S;
}

Matrix MyTranslate(const float x, const float y, const float z)
{
	Matrix T;
	//T[0][3] = x;
	//T[1][3] = y;
	//T[2][3] = z;
	T._14 = x;
	T._24 = y;
	T._34 = z;
	return T;
}

Matrix MyTranslate(const Vector3& a)
{
	Matrix T;
	T._14 = a.x;
	T._24 = a.y;
	T._34 = a.z;
	return T;
}

Vector4 MyVectorTransform(const Matrix & a, const Vector4 & b)
{
	return Vector4::Transform(b, a.Transpose());
}
Vector3 MyVectorTransform(const Matrix & a, const Vector3 & b)
{
	return Vector3::Transform(b, a.Transpose());
}
////this is right hand side prespective and it has boild-in transpose
//MAT4 Perspective(const float rx, const float ry, const float front, const float back)
//{
//	MAT4 P;
//	P[0][0] = 1.0f / rx;
//	P[1][1] = 1.0f / ry;
//	P[2][2] = -(back + front) / (back - front);
//	P[2][3] = -(2.0f*front*back) / (back - front);
//	P[3][2] = -1;
//	return P;
//}
//this is right hand side LookAt and it has boild-in transpose
//MAT4 MyLookAt(const float cameraX, const float cameraY, const float cameraZ, const float centerX, const float centerY, const float centerZ, const float cameraUpX, const float cameraUpY, const float cameraUpZ)
//{
//	VEC3 E(cameraX, cameraY, cameraZ);
//	VEC3 C(centerX, centerY, centerZ);
//	VEC3 U(cameraUpX, cameraUpY, cameraUpZ);
//	VEC3 V = normalize(C - E);
//	VEC3 A = normalize(cross(V, U));
//	VEC3 B = cross(A, V);
//	MAT4 result;
//	result[0][0] = A[0]; result[0][1] = A[1]; result[0][2] = A[2]; result[0][3] = -dot(A, E);
//	result[1][0] = B[0]; result[1][1] = B[1]; result[1][2] = B[2]; result[1][3] = -dot(B, E);
//	result[2][0] = -V[0]; result[2][1] = -V[1]; result[2][2] = -V[2]; result[2][3] = dot(V, E);
//	result[3][0] = 0; result[3][1] = 0; result[3][2] = 0; result[3][3] = 1.0f;
//
//	return result;
//}

Matrix MyLookAt(Vector3 rpos, Vector3 rlook_dir, Vector3 rup)
{
	Vector3 v= rlook_dir;
	v.Normalize();
	Vector3 a = v.Cross(rup);
	a.Normalize();
	Vector3 b = a.Cross(v);
	Matrix result;
	result._11 = a.x;result._12 = b.x; result._13 = -v.x; result._14 = 0;
	result._21 = a.y;result._22 = b.y; result._23 = -v.y; result._24 = 0;
	result._31 = a.z;result._32 = b.z; result._33 = -v.z; result._34 = 0;
	result._41 = -a.Dot(rpos); result._42 = -b.Dot(rpos); result._43 = v.Dot(rpos); result._44 = 1.0f;

	return result;
}

Vector3 PerpendicularFinder(Vector3 rvector)
{
	return Vector3(0.0f, -1.0f, 0.0f);
	Vector3 test= Vector3(1.0f, 0.0f,(rvector.x) /(rvector.z));
	test.Normalize();
	if (rvector.Dot(test) == 0)return test;
	test = Vector3(1.0f, - rvector.x / (rvector.y), 0.0f);
	test.Normalize();
	if (rvector.Dot(test) == 0)return test;
	test = Vector3(-rvector.y / rvector.x, 1.0f, 0.0f);
	test.Normalize();
	return test;
}

Matrix MyPerspective(const float rx, const float ry, const float front, const float back)
{
	Matrix P;
	P._11 = 1.0f / rx;
	P._22= 1.0f / ry;
	P._33 = -(back + front) / (back - front);
	P._43 = -(2.0f*front*back) / (back - front);
	P._34 = -1;
	return P;
}

MAT4 operator*(const MAT4 A, const MAT4 B)
{
	MAT4 M;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j) {
			M[i][j] = 0.0;
			for (int k = 0; k < 4; ++k)
				M[i][j] += A[i][k] * B[k][j];
		}
	return M;
}

Vector3 MyCross(Vector3 a, Vector3 b)
{
	return Vector3(a.y * b.z - b.y * a.z,
		a.z * b.x - b.z * a.x,
		a.x * b.y - b.x * a.y);
}

Vector3 MyNormalize(Vector3 a)
{
	return a / sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
}

Vector4 MyNormalize(Vector4 a)
{
	return a / sqrtf(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
}

float MyDot(Vector3 a, Vector3 b)
{
	return  a.x * b.x + a.y * b.y + a.z * b.z;
}

float MyDot(MyQuaternion a, MyQuaternion b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.s * b.s;
}

float MyDot(Vector4 a, Vector4 b)
{
	return   a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

////////////////////////////////////////////////////////////////////////
// Calculates (efficently) the inverse of a matrix by performing
// gaussian matrix reduction with partial pivoting followed by
// back/substitution with the loops manually unrolled.
//
// Taken from Mesa implementation of OpenGL:  http://mesa3d.sourceforge.net/
////////////////////////////////////////////////////////////////////////
#define MAT(m,r,c) ((*m)[r][c])
#define SWAP_ROWS(a, b) { float *_tmp = a; (a)=(b); (b)=_tmp; }

bool invert(const MAT4* mat, MAT4* inv)
{
	float wtmp[4][8];
	float m0, m1, m2, m3, s;
	float *r0, *r1, *r2, *r3;

	r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];

	r0[0] = MAT(mat, 0, 0);
	r0[1] = MAT(mat, 0, 1);
	r0[2] = MAT(mat, 0, 2);
	r0[3] = MAT(mat, 0, 3);
	r0[4] = 1.0;
	r0[5] = r0[6] = r0[7] = 0.0;

	r1[0] = MAT(mat, 1, 0);
	r1[1] = MAT(mat, 1, 1);
	r1[2] = MAT(mat, 1, 2);
	r1[3] = MAT(mat, 1, 3);
	r1[5] = 1.0, r1[4] = r1[6] = r1[7] = 0.0;

	r2[0] = MAT(mat, 2, 0);
	r2[1] = MAT(mat, 2, 1);
	r2[2] = MAT(mat, 2, 2);
	r2[3] = MAT(mat, 2, 3);
	r2[6] = 1.0, r2[4] = r2[5] = r2[7] = 0.0;

	r3[0] = MAT(mat, 3, 0);
	r3[1] = MAT(mat, 3, 1);
	r3[2] = MAT(mat, 3, 2);
	r3[3] = MAT(mat, 3, 3);
	r3[7] = 1.0, r3[4] = r3[5] = r3[6] = 0.0;

	/* choose pivot - or die */
	if (fabs(r3[0]) > fabs(r2[0])) SWAP_ROWS(r3, r2);
	if (fabs(r2[0]) > fabs(r1[0])) SWAP_ROWS(r2, r1);
	if (fabs(r1[0]) > fabs(r0[0])) SWAP_ROWS(r1, r0);
	if (0.0 == r0[0])  return false;

	/* eliminate first variable     */
	m1 = r1[0] / r0[0]; m2 = r2[0] / r0[0]; m3 = r3[0] / r0[0];
	s = r0[1]; r1[1] -= m1 * s; r2[1] -= m2 * s; r3[1] -= m3 * s;
	s = r0[2]; r1[2] -= m1 * s; r2[2] -= m2 * s; r3[2] -= m3 * s;
	s = r0[3]; r1[3] -= m1 * s; r2[3] -= m2 * s; r3[3] -= m3 * s;
	s = r0[4];
	if (s != 0.0) { r1[4] -= m1 * s; r2[4] -= m2 * s; r3[4] -= m3 * s; }
	s = r0[5];
	if (s != 0.0) { r1[5] -= m1 * s; r2[5] -= m2 * s; r3[5] -= m3 * s; }
	s = r0[6];
	if (s != 0.0) { r1[6] -= m1 * s; r2[6] -= m2 * s; r3[6] -= m3 * s; }
	s = r0[7];
	if (s != 0.0) { r1[7] -= m1 * s; r2[7] -= m2 * s; r3[7] -= m3 * s; }

	/* choose pivot - or die */
	if (fabs(r3[1]) > fabs(r2[1])) SWAP_ROWS(r3, r2);
	if (fabs(r2[1]) > fabs(r1[1])) SWAP_ROWS(r2, r1);
	if (0.0 == r1[1])  return false;

	/* eliminate second variable */
	m2 = r2[1] / r1[1]; m3 = r3[1] / r1[1];
	r2[2] -= m2 * r1[2]; r3[2] -= m3 * r1[2];
	r2[3] -= m2 * r1[3]; r3[3] -= m3 * r1[3];
	s = r1[4]; if (0.0 != s) { r2[4] -= m2 * s; r3[4] -= m3 * s; }
	s = r1[5]; if (0.0 != s) { r2[5] -= m2 * s; r3[5] -= m3 * s; }
	s = r1[6]; if (0.0 != s) { r2[6] -= m2 * s; r3[6] -= m3 * s; }
	s = r1[7]; if (0.0 != s) { r2[7] -= m2 * s; r3[7] -= m3 * s; }

	/* choose pivot - or die */
	if (fabs(r3[2]) > fabs(r2[2])) SWAP_ROWS(r3, r2);
	if (0.0 == r2[2])  return false;

	/* eliminate third variable */
	m3 = r3[2] / r2[2];
	r3[3] -= m3 * r2[3];
	r3[4] -= m3 * r2[4];
	r3[5] -= m3 * r2[5];
	r3[6] -= m3 * r2[6];
	r3[7] -= m3 * r2[7];

	/* last check */
	if (0.0 == r3[3]) return false;

	s = 1.0F / r3[3];             /* now back substitute row 3 */
	r3[4] *= s; r3[5] *= s; r3[6] *= s; r3[7] *= s;

	m2 = r2[3];                 /* now back substitute row 2 */
	s = 1.0F / r2[2];
	r2[4] = s * (r2[4] - r3[4] * m2);
	r2[5] = s * (r2[5] - r3[5] * m2);
	r2[6] = s * (r2[6] - r3[6] * m2);
	r2[7] = s * (r2[7] - r3[7] * m2);
	m1 = r1[3];
	r1[4] -= r3[4] * m1;
	r1[5] -= r3[5] * m1;
	r1[6] -= r3[6] * m1;
	r1[7] -= r3[7] * m1;
	m0 = r0[3];
	r0[4] -= r3[4] * m0;
	r0[5] -= r3[5] * m0;
	r0[6] -= r3[6] * m0;
	r0[7] -= r3[7] * m0;

	m1 = r1[2];                 /* now back substitute row 1 */
	s = 1.0F / r1[1];
	r1[4] = s * (r1[4] - r2[4] * m1);
	r1[5] = s * (r1[5] - r2[5] * m1);
	r1[6] = s * (r1[6] - r2[6] * m1);
	r1[7] = s * (r1[7] - r2[7] * m1);
	m0 = r0[2];
	r0[4] -= r2[4] * m0;
	r0[5] -= r2[5] * m0;
	r0[6] -= r2[6] * m0;
	r0[7] -= r2[7] * m0;

	m0 = r0[1];                 /* now back substitute row 0 */
	s = 1.0F / r0[0];
	r0[4] = s * (r0[4] - r1[4] * m0);
	r0[5] = s * (r0[5] - r1[5] * m0);
	r0[6] = s * (r0[6] - r1[6] * m0);
	r0[7] = s * (r0[7] - r1[7] * m0);

	MAT(inv, 0, 0) = r0[4];
	MAT(inv, 0, 1) = r0[5],
		MAT(inv, 0, 2) = r0[6];
	MAT(inv, 0, 3) = r0[7],
		MAT(inv, 1, 0) = r1[4];
	MAT(inv, 1, 1) = r1[5],
		MAT(inv, 1, 2) = r1[6];
	MAT(inv, 1, 3) = r1[7],
		MAT(inv, 2, 0) = r2[4];
	MAT(inv, 2, 1) = r2[5],
		MAT(inv, 2, 2) = r2[6];
	MAT(inv, 2, 3) = r2[7],
		MAT(inv, 3, 0) = r3[4];
	MAT(inv, 3, 1) = r3[5],
		MAT(inv, 3, 2) = r3[6];
	MAT(inv, 3, 3) = r3[7];

	return true;
}

MyQuaternion::MyQuaternion() {
	s = 1; x = 0; y = 0; z = 0;
}

MyQuaternion::MyQuaternion(float is, float ix, float iy, float iz)
	:s(is), x(ix), y(iy), z(iz) {}

MyQuaternion::MyQuaternion(float scalar, const Vector3 & v)
	: s(scalar), x(v.x), y(v.y), z(v.z) {}

MyQuaternion::MyQuaternion(const Vector4 & v)
	: s(v.x), x(v.y), y(v.z), z(v.w) {}

MyQuaternion::MyQuaternion(const float * pArray) {
	s = *pArray;
	x = *(pArray + 1); y = *(pArray + 2); z = *(pArray + 3);
}
MyQuaternion::MyQuaternion(const MyQuaternion & q) { s = q.s; x = q.x; y = q.y; z = q.z; }

MyQuaternion& MyQuaternion::operator=(const MyQuaternion& q) { s = q.s; x = q.x; y = q.y; z = q.z; return *this; }

MyQuaternion::MyQuaternion(MyQuaternion && q) { *this = q; }

MyQuaternion& MyQuaternion::operator=(MyQuaternion&& q) { *this = q; return *this; }

MyQuaternion::operator Vector4() const { return Vector4(s, x, y, z); }

// MyQuaternion operations
bool MyQuaternion::operator == (const MyQuaternion& q) const
{
	return(q.s == s && q.x == x && q.y == y && q.z == z);
}
bool MyQuaternion::operator != (const MyQuaternion& q) const
{
	return !(q.s == s && q.x == x && q.y == y && q.z == z);
}
MyQuaternion& MyQuaternion::operator= (const Vector4& F) {
	s = F.x; x = F.y; y = F.z; z = F.w; return *this;
}
MyQuaternion& MyQuaternion::operator+= (const MyQuaternion& q)
{
	s += q.s; x += q.x; y += q.y; z += q.z;
	return *this;
}
MyQuaternion& MyQuaternion::operator-= (const MyQuaternion& q)
{
	s -= q.s; x -= q.x; y -= q.y; z -= q.z;
	return *this;
}
MyQuaternion& MyQuaternion::operator*= (const MyQuaternion& q)
{
	Vector3 v1(x, y, z);
	Vector3 v2(q.x, q.y, q.z);
	s = s * q.s - MyDot(v1, v2);
	Vector3 v3 = v2 * s + v1 * q.s + MyCross(v1, v2);
	x = v3.x; y = v3.y; z = v3.z;
	return *this;
}
MyQuaternion& MyQuaternion::operator*= (float S)
{
	s *= S; x *= S; y *= S; z *= S;
	return *this;
}
MyQuaternion& MyQuaternion::operator/= (const MyQuaternion& q)
{
	*this = *this*q.Inverse();
	return *this;
}
MyQuaternion MyQuaternion::operator+ () const { return *this; }
MyQuaternion MyQuaternion::operator- () const { return MyQuaternion(-s, -x, -y, -z); }
float MyQuaternion::Length() const
{
	return sqrtf(s*s + MyDot(Vector3(x, y, z), Vector3(x, y, z)));
}

float MyQuaternion::LengthSquared() const
{
	return s * s + MyDot(Vector3(x, y, z), Vector3(x, y, z));
}

void MyQuaternion::Normalize()
{
	float ss = 1.0f / sqrtf(s * s + x * x + y * y + z * z);
	x /= ss;
	y /= ss;
	z /= ss;
	s /= ss;
}

void MyQuaternion::Normalize(MyQuaternion & result) const
{
	result = MyNormalize(Vector4(*this));
}

void MyQuaternion::Rotate(Vector3 r)
{
	Vector3 Zaxis(0, 0, 1);
	Axis = MyCross(Zaxis, r);
	Angle = acosf(MyDot(r, Zaxis) / sqrtf(MyDot(r, r)));
	s = 0;
	Vector3 temp = r * cosf(Angle / 2) + Axis * (1.0f - cosf(Angle))*(MyDot(Axis, r)) + MyCross(Axis, r);
	x = temp.x;
	y = temp.y;
	z = temp.z;
}

void MyQuaternion::ToMatrix(Matrix & m)
{
	m._11 = 1.0f - 2.0f*(y*y + z * z); m._12 = 2.0f*(x*y - s * z); m._13 = 2.0f*(x*z + s * y);
	m._21 = 2.0f*(x*y + s * z); m._22 = 1.0f - 2.0f*(x*x + z * z); m._23 = 2.0f*(y*z - s * x);
	m._31 = 2.0f*(x*z - s * y); m._32 = 2.0f*(y*z + s * x); m._33 = 1.0f - 2.0f*(y*y + x * x);
}

Matrix MyQuaternion::ToMatrix()
{
	Matrix m;
	m._11 = 1.0f - 2.0f*(y*y + z * z); m._12 = 2.0f*(x*y - s * z); m._13 = 2.0f*(x*z + s * y);
	m._21 = 2.0f*(x*y + s * z); m._22 = 1.0f - 2.0f*(x*x + z * z); m._23 = 2.0f*(y*z - s * x);
	m._31 = 2.0f*(x*z - s * y); m._32 = 2.0f*(y*z + s * x); m._33 = 1.0f - 2.0f*(y*y + x * x);
	return m;
}

void MyQuaternion::Conjugated()
{
	x = -x; y = -y; z = -z;
}

MyQuaternion MyQuaternion::Conjugate()
{
	MyQuaternion r;
	r.s = s; r.x = -x; r.y = -y; r.z = -z;
	return r;
}

void MyQuaternion::Inversed()
{
	float len = s * s + (-x * -x) + (-y * -y) + (-z * -z);
	s = s / len; x = -x / len; y = -y / len; z = -z / len;
}

MyQuaternion MyQuaternion::Inverse() const
{
	MyQuaternion r;
	float len = s * s + (-x * -x) + (-y * -y) + (-z * -z);
	r.s = s / len; r.x = -x / len; r.y = -y / len; r.z = -z / len;
	return r;
}

float MyQuaternion::Dot(const MyQuaternion & Q) const
{
	return x * Q.x + y * Q.y + z * Q.z + s * Q.s;
}

void MyQuaternion::Slerp(float t, const MyQuaternion & Q1, const MyQuaternion & Q2, MyQuaternion & result)
{
	MyQuaternion q0 = Q1;
	MyQuaternion q1 = Q2;
	float d = q0.Dot(q1);
	if (fabsf(d) > 0.9995f)
	{
		result = MyNormalize(q0 + t * (q1 - q0));
	}
	if (d < 0)
	{
		q0 = -1 * q0; d = -d;
	}
	float a = acoshf(d);
	result = q0 * sinf((1 - t)*a) + q1 * sin(t*a) * (1.0f / sinf(a));
}

MyQuaternion MyQuaternion::Slerp(float t, const MyQuaternion & Q1, const MyQuaternion & Q2)
{
	MyQuaternion q0 = Q1;
	MyQuaternion q1 = Q2;
	float d = q0.Dot(q1);
	if (fabsf(d) > 0.9995f)
	{
		MyQuaternion temp = q0 + t * (q1 - q0);
		temp.Normalize();
		return temp;
	}
	if (d < 0.0f)
	{
		q0 = -1 * q0; d = -d;
	}
	float a = acosf(d);
	return (q0 * sinf((1.0f - t)*a) + q1 * sin(t*a)) * (1.0f / sinf(a));
}
// Binary operators
MyQuaternion operator+ (const MyQuaternion& Q1, const MyQuaternion& Q2)
{
	return MyQuaternion(Q1.s + Q2.s, Q1.x + Q2.x, Q1.y + Q2.y, Q1.z + Q2.z);
}
MyQuaternion operator- (const MyQuaternion& Q1, const MyQuaternion& Q2)
{
	return MyQuaternion(Q1.s - Q2.s, Q1.x - Q2.x, Q1.y - Q2.y, Q1.z - Q2.z);
}
MyQuaternion operator* (const MyQuaternion& Q1, const MyQuaternion& Q2)
{
	Vector3 v1(Q1.x, Q1.y, Q1.z);
	Vector3 v2(Q2.x, Q2.y, Q2.z);
	return MyQuaternion(Q1.s*Q2.s - MyDot(v1, v2),
		v2*Q1.s + v1 * Q2.s + MyCross(v1, v2));
}
MyQuaternion operator* (const MyQuaternion& Q, float S)
{
	return MyQuaternion(Q.s*S, Q.x*S, Q.y*S, Q.z*S);
}
MyQuaternion operator/ (const MyQuaternion& Q1, const MyQuaternion& Q2)
{
	return Q1 * Q2.Inverse();
}
MyQuaternion operator* (float S, const MyQuaternion& Q)
{
	return MyQuaternion(Q.s*S, Q.x*S, Q.y*S, Q.z*S);
}



Vector3 TranslationLERP(Vector3 v0, Vector3 v1, float t)
{
	float x = (1.0f - t)*v0.x + v1.x * t;
	float y = (1.0f - t)*v0.y + v1.y * t;
	float z = (1.0f - t)*v0.z + v1.z * t;
	return Vector3(x, y, z);
}

Vector3 ScaleLERP(Vector3 s0, Vector3 s1, float t)
{
	float x = s0.x*powf(s1.x / s0.x, t);
	float y = s0.y*powf(s1.y / s0.y, t);
	float z = s0.z*powf(s1.z / s0.z, t);
	return Vector3(x, y, z);
}

Matrix MMatrixOrthoRH(float fWidth, float fHeight, float fNear, float fFar)
{
	return Matrix(2.0f / fWidth, 0, 0, 0,
		0, 2.0f / fHeight, 0, 0,
		0, 0, 1 / (fNear - fFar), 0,
		0, 0, -fNear / (fFar - fNear), 1);

}

float RandomFloatRange(float hi, float low)
{
	return low + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (hi - low)));;
}
float UniformRandom(float hi, float low)
{
	std::mt19937_64 RNGen;
	std::uniform_real_distribution<> myrandom(hi, low);
	return myrandom(RNGen);
}
