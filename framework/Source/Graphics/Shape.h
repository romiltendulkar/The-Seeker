/**
 * @file Shape.h
 * @author Ching-Yen Lin
 * @date 9/01/19
 * @brief This file content shape member
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#ifndef SHAPE_H
#define SHAPE_H
 //#define _HAS_STD_BYTE 0
#include "../pch.h"
#include "../Util/Math.h"
#include "Animation/BoneNode.h"
#include "Animation/AnimationClips.h"
#include "DXTexture.h"
using namespace std;
struct AnimationVertexType
{
	Vector3 vertex;
	Vector3 vertexNormal;
	Vector2 vertexTexture;
	Vector3 vertexTangent;
	Vector4 boneWeight;
	XMUINT4 boneId;
};
struct SimpleVertexType
{
	Vector3 vertex;
	Vector3 vertexNormal;
	Vector2 vertexTexture;
	Vector3 vertexTangent;
};

struct SubMesh
{
	SubMesh() :
		MaterialID(-1),
		VertexStart(0), VertexCount(0),
		FaceStart(0), FaceCount(0),
		DiffuseTextureID(0), SpecularTextureID(0)
		, ShininessTextureID(0), NormalTextureID(0),
		Shininess(0)
	{}
	unsigned int MaterialID;
	unsigned int VertexStart;
	unsigned int VertexCount;
	unsigned int FaceStart;
	unsigned int FaceCount;
	Vector3 DiffuseColor;
	Vector3 SpecularColor;
	float Shininess;
	DXTexture* DiffuseTextureID;
	DXTexture* SpecularTextureID;
	DXTexture* ShininessTextureID;
	DXTexture* NormalTextureID;
};

class Shape
{
public:
	Shape() :mHasAnimation(false), mHasBone(false), mDefaultMaterialID(0) {
		mMinPoint.x = numeric_limits<float>::infinity(); mMinPoint.y = numeric_limits<float>::infinity(); mMinPoint.z = numeric_limits<float>::infinity();
		mMaxPoint.x = -numeric_limits<float>::infinity(); mMaxPoint.y = -numeric_limits<float>::infinity(); mMaxPoint.z = -numeric_limits<float>::infinity();
	}
	virtual ~Shape() { DeleteData(); }

	vector<AnimationVertexType> LAnimedVertexData;
	vector <SimpleVertexType> LSimpleVertexData;
	vector<unsigned int> LFace;
	//bone things
	vector<unsigned int> LNumBoneInfo;
	vector<Matrix> LBoneTr;
	vector<Matrix> LBonePnt;//for debug draw
	size_t mPntcount = 0;
	size_t mTricount = 0;
	vector<SubMesh> LSubMeshList;
	vector<DXTexture*> LTextureList;

	float mModelSize = 0;
	Vector3 mMedelCenter;
	Matrix mModelRotate;

	Vector3 mDefaultDiffuseColor, mDefaultSpecularColor;
	float mDefaultShininess;
	int mDefaultMaterialID;

	Matrix mModelTr;
	Vector3 mMinPoint, mMaxPoint;
	float mTempModelSize;
	bool mHasAnimation;
	bool mHasBone;
	bool mActive = true;

	/**
	Delete Data
	*/
	virtual void DeleteData();
protected:

	/**
	Set MinPoint & MaxPoint
	*/
	void SetMinMaxPoint(Vector3& vertex);

	/**
	Compute unit Size
	*/
	virtual void ComputeSize();

	/**
	Compute Tanget
	*/
	virtual void ComputeTanget();

	/**
	go through ProcessNode in ASSIMP
	*/
	void StaticProcessNode(aiNode * node, const aiScene * scene);

	/**
	go through ProcessMesh in ASSIMP
	*/
	void StaticProcessMesh(aiMesh * mesh, const aiScene * scene);

	/**
	Texture Linker
	*/
	void TextureLinker(aiMaterial *, aiTextureType);

};
class Triangle : public Shape
{
public:
	/**
	2D Triangle
	*/
	Triangle(const int n = 1);
};
class Quad : public Shape
{
public:
	/**
	2D Quad
	*/
	Quad(const int n = 1);
};
class Cylinder : public Shape
{
public:
	/**
	3D Cylinder
	*/
	Cylinder(const int n = 32);
};
class Torus : public Shape
{
public:
	/**
	creat Torus
	*/
	Torus(const int n = 32);
};
class Disk : public Shape
{
public:
	/**
	2D Disk
	*/
	Disk(const int n = 32);
};
class Sphere : public Shape
{
public:
	/**
	3D Sphere
	*/
	Sphere(const int n = 32);
};
class Box : public Shape
{
public:
	/**
	3D Box
	*/
	Box(VEC3 p0, VEC3 P1);
};
class OBJ : public Shape
{
public:
	/**
	OBJ loader
	*/
	OBJ(string filename);
};
class FBX : public Shape
{
public:
	/**
	FBX loader
	*/
	FBX(string filename, const vector<string>& animationnamelist = vector<string>());
	
	/**
	FBX update
	*/
	void Update(float time, string animationName, bool isLoop, float speed);

	/**
	FBX update
	*/
	void Blending(string animationName1, string animationName2,float, float time, float speed);
	
	/**
	clear FBX
	*/
	~FBX();
	unordered_map<string, AnimationClips> LAnimationlist;
	vector<string> LBoneName;;
	BoneNode mRootBone;
	void AddAnimationFromFile(string filename, string animationname);
	
	/**
	add Bone Node To Array
	*/
	void AddBoneNodeToArray(BoneNode& rootnode, vector<Matrix>& jointMatrices, vector<Matrix>& jointMatrices2, Matrix parentBindTransform);

	/**
	add Bone Node To Array
	*/
	void AddBlendingToArray(BoneNode& rootnode, vector<Matrix>& jointMatrices, vector<Matrix>& jointMatrices2, Matrix parentBindTransform);
	
private:
	int mJointCount;
	string mPreviousAnimation;
	string mCurrentAnimation;
	float mBlendingValue;
	unordered_map<string, int>LBoneNametoindex;
	unordered_map<string, Matrix>LBoneOffsetMatrix;

	/**
	Set BoneInfo
	*/
	void SetBoneInfo(int, int, float);

	/**
	go through ProcessNode in ASSIMP
	*/
	void ProcessNode(aiNode *node, const aiScene *scene);

	/**
	go through ProcessMesh in ASSIMP
	*/
	void ProcessMesh(aiMesh *mesh, const aiScene *scene);
	
	/**
	read Bone Hierarchy
	*/
	BoneNode ReadBoneHierarchy(const aiNode* node);
	
	/**
	load Animation
	*/
	void LoadAnimation(aiAnimation* anim, string animname = "");
};
//q
struct ParticleInfo
{
	Vector3 position;
	Vector3 velocitydir;
	bool active;
	float age;
	size_t size;
};
class Particles :public Shape
{

public:
	/**
	create Particles
	*/
	Particles(size_t maxsize= 1000);

	/**
	clean Particles
	*/
	~Particles();

	/**
	Update Particles
	*/
	void Update(float time);

	/**
	Get Particle list
	*/
	vector<ParticleInfo>& GetParticlelist();
	float mTotalTime;
	size_t mMaxParticles;
	size_t mSpawnNumber;
	float mSpawnTime;//
	Vector2 mLifeTime;//
	Vector3 mMinOffset;//
	Vector3 mMaxOffset;//
	Vector3 mMaxVelocity;//
	Vector3 mMinVelocity;//
	Vector3 mMaxSize;
	Vector3 mMinSize;
	bool mStop=false;
private:
	vector<ParticleInfo> mParticlelist;

	/**
	Emit Particles
	*/
	void EmitParticles();

	/**
	Kill Particles
	*/
	void KillParticles();


};
#endif // !1
void showMesh(aiMesh* mesh);
void showAnimation(aiAnimation* anim);
void showBoneHierarchy(const aiScene* scene, const aiNode* node, const int level);
void ReadAssimpFile(const string& path);