/**
 * @file DXObject.h
 * @author Ching-Yen Lin
 * @date 9/01/19
 * @brief This file content DXObject member setting
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#ifndef DXOBJECT_H
#define DXOBJECT_H
#include "../pch.h"
#include "Shape.h"
#include "../Components/Transform.h"
enum TEXTURETYPE
{
	DIFFUSE_TEXTURE = 0,
	SPECULAR_TEXTURE,
	SHININESS_TEXTURE,
	NORMAL_TEXTURE
};
class Shape;
class DXTexture;
struct ObjectInfo
{
	Vector3 diffuse;
	FLOAT shininess;
	Vector3 specular;
	INT MaterialID;
	XMUINT4 TextureIO;
	Vector4 TextureSize;
};
struct ModelTransformation
{
	Matrix ModelTr;
	Matrix NormalTr;
};
struct BoneTransform
{
	Matrix boneTr[133];
};
class DXObject
{
public:

	/**
	 Initial the DXObject
	*/
	DXObject(Shape*, ID3D11Device*, ID3D11DeviceContext*, bool rdynamicvertex = false);
	
	/**
	 Initial the DXObject with define Object Transform
	*/
	DXObject(Shape*, ID3D11Device*, ID3D11DeviceContext*, Matrix objectTr, bool rdynamicvertex = false);
	
	/**
	clean VBO
	*/
	~DXObject();

	/**
	Set ObjectTr
	*/
	void SetObjectTr(Transform* pTr);
	void SetObjectTr(Matrix);
	Matrix& GetObjectTr();

	/**
	set Diffuse
	*/
	void SetDiffuse(float r, float g, float b);
	void UseDiffuseColor(bool r);
	/**
	set Specular
	*/
	void SetSpecular(float r, float g, float b);

	/**
	set Shininess
	*/
	void SetShininess(float a);

	/**
	set MaterialID
	*/
	void SetMaterialID(int id);

	/**
	return bool about Has Bone
	*/
	bool HasBone();
	
	/**
	 draw object
	*/
	void Draw(ID3D11Buffer* objectBuff, ID3D11Buffer* modelBuff, ID3D11Buffer* boneBuff, D3D_PRIMITIVE_TOPOLOGY= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	void DynamicDraw2D(ID3D11Buffer* objectBuff, ID3D11Buffer* modelBuff, ID3D11Buffer* boneBuff);
	/**
	Draw Bone Helper for recursive call
	*/
	void DrawBoneHelper(BoneNode & rootnode, Matrix&);
	
	/**
	Set Texture
	*/
	void SetTexture(TEXTURETYPE, DXTexture*);
	Shape* pShape;
	ObjectInfo& GetObjectInfo();
private:
	bool mCustomColor = false;
	ID3D11DeviceContext *pDeviceContext;
	ID3D11Buffer* pVertexBuffer;
	ID3D11Buffer* pIndexBuffer;
	Matrix mObjectTr;
	ObjectInfo mObjectInfo;
	bool mDynamicVertex = false;
	DXTexture* LTextureList[4];
	void SetTextureIO(SubMesh& rsubmesh);

	/**
	reset Texture slots
	*/
	void RestTextureIO(SubMesh& rsubmesh);
	unsigned int mVertexCount = 0;
	unsigned int mIndexCount = 0;
};

/**
Bind ObjectInfo
*/
void BindObjectInfo(ID3D11DeviceContext *, ID3D11Buffer*, ObjectInfo);

/**
Bind ModelTransformation
*/
void BindModelTransformation(ID3D11DeviceContext *, ID3D11Buffer*, Matrix);

/**
Bind Bone Transformation
*/
void BindBoneTransformation(ID3D11DeviceContext *, ID3D11Buffer*, std::vector<Matrix>&);
#endif
