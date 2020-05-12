/**
 * @file Shape.cpp
 * @author Ching-Yen Lin
 * @date 9/01/19
 * @brief This file content shape creation functions
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#include "../pch.h"
#include "Shape.h"
#include "../../Manager/GraphicsManager.h"

 /**
 push size face
 */
void pushquad2(std::vector<unsigned int> &face, int i, int j, int k, int l)
{
	face.push_back(i);
	face.push_back(j);
	face.push_back(k);

	face.push_back(i);
	face.push_back(k);
	face.push_back(l);
}
/**
Delete Data
*/
void Shape::DeleteData()
{
	LAnimedVertexData.clear();
	LSimpleVertexData.clear();
	LNumBoneInfo.clear();
	LBoneTr.clear();
	LBonePnt.clear();
	LSubMeshList.clear();
	LFace.clear();
	for (size_t i = 0; i < LTextureList.size(); i++)
	{
		LTextureList[i]->~DXTexture();
	}
	LTextureList.clear();
}
/**
Compute unit Size
*/
void Shape::ComputeSize()
{
	mMedelCenter = (mMaxPoint + mMinPoint) / 2.0f;
	if (mTempModelSize > 100)
	{
		if (mHasAnimation)
		{
			//mTempModelSize = 10;
			mMedelCenter = Vector3();
			//mMedelCenter = -mMedelCenter;
		}
	}
	mModelSize = 1.0f / mTempModelSize;
	mModelRotate = Matrix();// ::CreateRotationX(90.0f*DEGREE_TO_RADIAN).Transpose();
	mModelTr = Matrix::CreateScale(mModelSize)*Matrix::CreateTranslation(-mMedelCenter).Transpose();
}
void Shape::ComputeTanget()
{
	for (int index = 0; index < LFace.size(); index += 3)
	{
		int i = LFace[index];
		int j = LFace[index + 1];
		int k = LFace[index + 2];
		Vector2 A;
		Vector2 B;
		if (mHasBone == false)
		{
			A = LSimpleVertexData[i].vertexTexture - LSimpleVertexData[k].vertexTexture;
			B = LSimpleVertexData[j].vertexTexture - LSimpleVertexData[k].vertexTexture;
		}
		else
		{
			A = LAnimedVertexData[i].vertexTexture - LAnimedVertexData[k].vertexTexture;
			B = LAnimedVertexData[j].vertexTexture - LAnimedVertexData[k].vertexTexture;
		}
		float d = A.x * B.y - A.y * B.x;
		float a = B.y / d;
		float b = -A.y / d;
		Vector3 tangent;
		if (mHasBone == false)
		{
			tangent = LSimpleVertexData[i].vertex*a + LSimpleVertexData[j].vertex*b + LSimpleVertexData[k].vertex*(1.0f - a - b);
		}
		else
		{
			auto temp = (LAnimedVertexData[i].vertex*a + LAnimedVertexData[j].vertex*b + LAnimedVertexData[k].vertex*(1.0f - a - b));
			tangent.x = temp.x; tangent.y = temp.y; tangent.z = temp.z;
		}
		tangent.Normalize();
		if (mHasBone == false)
		{
			LSimpleVertexData[i].vertexTangent = tangent;
			LSimpleVertexData[j].vertexTangent = tangent;
			LSimpleVertexData[k].vertexTangent = tangent;
		}
		else
		{
			LAnimedVertexData[i].vertexTangent = tangent;
			LAnimedVertexData[j].vertexTangent = tangent;
			LAnimedVertexData[k].vertexTangent = tangent;
		}
	}

}
/**
Set MinPoint & MaxPoint
*/
void Shape::SetMinMaxPoint(Vector3& vertex)
{
	mMinPoint.x = min(mMinPoint.x, vertex.x);
	mMinPoint.y = min(mMinPoint.y, vertex.y);
	mMinPoint.z = min(mMinPoint.z, vertex.z);
	mMaxPoint.x = max(mMaxPoint.x, vertex.x);
	mMaxPoint.y = max(mMaxPoint.y, vertex.y);
	mMaxPoint.z = max(mMaxPoint.z, vertex.z);
	mTempModelSize = max(mTempModelSize, (mMaxPoint.x - mMinPoint.x));
	mTempModelSize = max(mTempModelSize, (mMaxPoint.y - mMinPoint.y));
	mTempModelSize = max(mTempModelSize, (mMaxPoint.z - mMinPoint.z));
}
/**
2D Triangle
*/
Triangle::Triangle(const int n)
{
	mDefaultDiffuseColor = Vector3(0.3f, 0.2f, 0.1f);
	mDefaultSpecularColor = Vector3(1.0f, 1.0f, 1.0f);
	mDefaultShininess = 120.0;
	for (int i = 0; i < 3; i++)
	{
		auto x = ((i == 0) ? -1 : 1)*(i + 1) % 2;
		auto y = pow(-1, (i + 1) % 2) * 1;
		auto v = (i + 1) % 2;
		LSimpleVertexData.push_back(SimpleVertexType());
		SimpleVertexType &temp = LSimpleVertexData.back();
		temp.vertex.x = float(x); temp.vertex.y = float(y); temp.vertex.z = float(0.0);
		SetMinMaxPoint(temp.vertex);
		temp.vertexNormal.x = 0.0f; temp.vertexNormal.y = 0.0f; temp.vertexNormal.z = 1.0f;
		temp.vertexTexture.x = 0.5f*i*0.5f; temp.vertexTexture.y = float(v);
		temp.vertexTangent.x = 1.0f; temp.vertexTangent.y = 0.0f; temp.vertexTangent.z = 0.0f;
	}
	LFace.push_back(0);
	LFace.push_back(1);
	LFace.push_back(2);
	mTricount = LFace.size();
	mPntcount = LSimpleVertexData.size();
	ComputeSize();
	SubMesh submesh;
	submesh.MaterialID = 1;
	submesh.VertexStart = 0;
	submesh.VertexCount = (unsigned int)mPntcount;
	submesh.FaceStart = 0;
	submesh.FaceCount = (unsigned int)LFace.size();
	submesh.DiffuseColor = mDefaultDiffuseColor;
	submesh.SpecularColor = mDefaultSpecularColor;
	submesh.Shininess = mDefaultShininess;
	LSubMeshList.push_back(submesh);
}
/**
2D Quad
*/
Quad::Quad(const int n)
{
	mDefaultDiffuseColor = Vector3(0.0f, 1.0f, 0.0f);
	mDefaultSpecularColor = Vector3(1.0f, 1.0f, 1.0f);
	mDefaultShininess = 120.0;
	float r = 1.0;
	for (int i = 0; i <= n; i++) {
		float s = i / float(n);
		for (int j = 0; j <= n; j++) {
			float t = j / float(n);
			LSimpleVertexData.push_back(SimpleVertexType());
			SimpleVertexType &temp = LSimpleVertexData.back();
			temp.vertex.x = 2.0f*r*s - r; temp.vertex.y = t * 2.0f * r - r; temp.vertex.z = 0.0f;
			SetMinMaxPoint(temp.vertex);
			temp.vertexNormal.x = 0.0f; temp.vertexNormal.y = 0.0; temp.vertexNormal.z = 1.0;
			temp.vertexTexture.x = s; temp.vertexTexture.y = t;
			temp.vertexTangent.x = 1.0f; temp.vertexTangent.y = 0.0f; temp.vertexTangent.z = 0.0f;
			if (i > 0 && j > 0) {
				pushquad2(LFace,
					(i - 1)*(n + 1) + (j - 1),
					(i - 1)*(n + 1) + (j),
					(i)*(n + 1) + (j),
					(i)*(n + 1) + (j - 1));
			}
		}
	}
	mTricount = LFace.size();
	mPntcount = LSimpleVertexData.size();
	ComputeSize();
	SubMesh submesh;
	submesh.MaterialID = 1;
	submesh.VertexStart = 0;
	submesh.VertexCount = (unsigned int)mPntcount;
	submesh.FaceStart = 0;
	submesh.FaceCount = (unsigned int)LFace.size();
	submesh.DiffuseColor = mDefaultDiffuseColor;
	submesh.SpecularColor = mDefaultSpecularColor;
	submesh.Shininess = mDefaultShininess;
	LSubMeshList.push_back(submesh);
}
/**
3D Cylinder
*/
Cylinder::Cylinder(const int n)
{
	mDefaultDiffuseColor = Vector3(0.5, 0.5, 1.0);
	mDefaultSpecularColor = Vector3(1.0, 1.0, 1.0);
	mDefaultShininess = 120.0;
	float d = 2.0f*pi / float(n);
	for (int i = 0; i <= n; i++) {
		float s = i * 2.0f*pi / float(n);
		for (int j = 0; j <= 1; j++) {
			float t = (float)j;
			float x = cos(s);
			float y = sin(s);
			float z = t * 2.0f - 1.0f;

			LSimpleVertexData.push_back(SimpleVertexType());
			SimpleVertexType &temp = LSimpleVertexData.back();
			temp.vertex.x = x; temp.vertex.y = y; temp.vertex.z = z;
			SetMinMaxPoint(temp.vertex);
			temp.vertexNormal.x = x; temp.vertexNormal.y = y; temp.vertexNormal.z = 0.0;
			temp.vertexTexture.x = s / (2.0f*pi); temp.vertexTexture.y = t;
			temp.vertexTangent.x = -sin(s); temp.vertexTangent.y = cos(s); temp.vertexTangent.z = 0.0f;

			if (i > 0 && j > 0) {
				pushquad2(LFace, (i - 1)*(2) + (j - 1),
					(i - 1)*(2) + (j),
					(i)*(2) + (j),
					(i)*(2) + (j - 1));
			}
		}
	}
	ComputeSize();
	mTricount = LFace.size();
	mPntcount = LSimpleVertexData.size();
	SubMesh submesh;
	submesh.MaterialID = 1;
	submesh.VertexStart = 0;
	submesh.VertexCount = (unsigned int)mPntcount;
	submesh.FaceStart = 0;
	submesh.FaceCount = (unsigned int)LFace.size();
	submesh.DiffuseColor = mDefaultDiffuseColor;
	submesh.SpecularColor = mDefaultSpecularColor;
	submesh.Shininess = mDefaultShininess;
	LSubMeshList.push_back(submesh);
}
/**
2D Disk
*/
Disk::Disk(const int n)
{
	mDefaultDiffuseColor = Vector3(0.1f, 0.1f, 0.1f);
	mDefaultSpecularColor = Vector3(1.0f, 1.0f, 1.0f);
	mDefaultShininess = 120.0;
	// Push center point
	LSimpleVertexData.push_back(SimpleVertexType());
	SimpleVertexType &temp = LSimpleVertexData.back();
	temp.vertex.x = 0.0f; temp.vertex.y = 0.0f; temp.vertex.z = 0.0f;
	SetMinMaxPoint(temp.vertex);
	temp.vertexNormal.x = 0.0f; temp.vertexNormal.y = 0.0f; temp.vertexNormal.z = 1.0f;
	temp.vertexTexture.x = 0.5f; temp.vertexTexture.y = 0.5f;
	temp.vertexTangent.x = 1.0f; temp.vertexTangent.y = 0.0f; temp.vertexTangent.z = 0.0f;

	float d = 2.0f*pi / float(n);
	for (int i = 0; i <= n; i++) {
		float s = i * 2.0f*pi / float(n);
		float x = cos(s);
		float y = sin(s);

		LSimpleVertexData.push_back(SimpleVertexType());
		SimpleVertexType &temp = LSimpleVertexData.back();
		temp.vertex.x = x; temp.vertex.y = y; temp.vertex.z = 0.0f;
		SetMinMaxPoint(temp.vertex);
		temp.vertexNormal.x = 0.0f; temp.vertexNormal.y = 0.0f; temp.vertexNormal.z = 1.0f;
		temp.vertexTexture.x = x * 0.5f + 0.5f; temp.vertexTexture.y = y * 0.5f + 0.5f;
		temp.vertexTangent.x = 1.0f; temp.vertexTangent.y = 0.0f; temp.vertexTangent.z = 0.0f;
		if (i > 0) {
			LFace.push_back(0); LFace.push_back(i + 1); LFace.push_back(i);
		}
	}
	ComputeSize();
	mTricount = LFace.size();
	mPntcount = LSimpleVertexData.size();
	SubMesh submesh;
	submesh.MaterialID = 1;
	submesh.VertexStart = 0;
	submesh.VertexCount = (unsigned int)mPntcount;
	submesh.FaceStart = 0;
	submesh.FaceCount = (unsigned int)LFace.size();
	submesh.DiffuseColor = mDefaultDiffuseColor;
	submesh.SpecularColor = mDefaultSpecularColor;
	submesh.Shininess = mDefaultShininess;
	LSubMeshList.push_back(submesh);
}
/**
3D Sphere
*/
Sphere::Sphere(const int n)
{
	mDefaultDiffuseColor = Vector3(0.5, 0.5, 1.0);
	mDefaultSpecularColor = Vector3(1.0, 1.0, 1.0);
	mDefaultShininess = 120;
	float d = 2.0f*pi / float(n * 2);
	for (int i = 0; i <= n * 2; i++) {
		float s = i * 2.0f*pi / float(n * 2);
		for (int j = 0; j <= n; j++) {
			float t = j * pi / float(n);
			float x = cos(s)*sin(t);
			float y = sin(s)*sin(t);
			float z = cos(t);

			LSimpleVertexData.push_back(SimpleVertexType());
			SimpleVertexType &temp = LSimpleVertexData.back();
			temp.vertex.x = x; temp.vertex.y = y; temp.vertex.z = z;
			SetMinMaxPoint(temp.vertex);
			temp.vertexNormal.x = x; temp.vertexNormal.y = y; temp.vertexNormal.z = z;
			temp.vertexTexture.x = s / (2 * pi); temp.vertexTexture.y = t / pi;
			temp.vertexTangent.x = -sin(s); temp.vertexTangent.y = cos(s); temp.vertexTangent.z = 0.0f;
			if (i > 0 && j > 0) {
				pushquad2(LFace, (i)*(n + 1) + (j - 1),
					(i)*(n + 1) + (j),
					(i - 1)*(n + 1) + (j),
					(i - 1)*(n + 1) + (j - 1));
			}
		}
	}
	ComputeSize();
	mTricount = LFace.size();
	mPntcount = LSimpleVertexData.size();
	SubMesh submesh;
	submesh.MaterialID = 1;
	submesh.VertexStart = 0;
	submesh.VertexCount = (unsigned int)mPntcount;
	submesh.FaceStart = 0;
	submesh.FaceCount = (unsigned int)LFace.size();
	submesh.DiffuseColor = mDefaultDiffuseColor;
	submesh.SpecularColor = mDefaultSpecularColor;
	submesh.Shininess = mDefaultShininess;
	LSubMeshList.push_back(submesh);
}
/**
3D Box
*/
Box::Box(VEC3 p0, VEC3 P1)
{
	mDefaultDiffuseColor = Vector3(0.5, 0.5, 1.0);
	mDefaultSpecularColor = Vector3(1.0, 1.0, 1.0);
	mDefaultShininess = 120;

	float x = abs(p0[0] - P1[0]) / 2.0f;
	float y = abs(p0[1] - P1[1]) / 2.0f;
	float z = abs(p0[2] - P1[2]) / 2.0f;

	float verts[8] = { -1.0f,-1.0f, -1.0f,1.0f, 1.0f, 1.0f,  1.0f,-1.0f };
	float texcd[8] = { 0.0f,0.0f,    0.0f,1.0f,  1.0f, 1.0f, 1.0f, 0.0f };

	//top-bot
	for (int j = 0; j < 2; j++)
	{
		int n = (int)LSimpleVertexData.size();
		for (int i = 0; i < 8; i += 2)
		{
			LSimpleVertexData.push_back(SimpleVertexType());
			SimpleVertexType &temp = LSimpleVertexData.back();
			temp.vertex.x = verts[i] * x; temp.vertex.y = verts[i + 1] * y; temp.vertex.z = (j == 0 ? -1.0f : 1.0f)*z;
			SetMinMaxPoint(temp.vertex);
			temp.vertexNormal.x = 0; temp.vertexNormal.y = 0; temp.vertexNormal.z = (j == 0 ? -1.0f : 1.0f);
			temp.vertexTexture.x = (j == 0 ? texcd[7 - i] : texcd[i]); temp.vertexTexture.y = (j == 0 ? texcd[7 - (i + 1)] : texcd[(i + 1)]);
			temp.vertexTangent.x = (j == 0 ? -1.0f : 1.0f); temp.vertexTangent.y = 0; temp.vertexTangent.z = 0;

		}
		j == 1 ? pushquad2(LFace, n, n + 1, n + 2, n + 3) : pushquad2(LFace, n + 3, n + 2, n + 1, n);
	}


	for (int j = 0; j < 2; j++)
	{
		int n = (int)LSimpleVertexData.size();
		for (int i = 0; i < 8; i += 2)
		{
			LSimpleVertexData.push_back(SimpleVertexType());
			SimpleVertexType &temp = LSimpleVertexData.back();
			temp.vertex.x = (j == 0 ? 1.0f : -1.0f)*x; temp.vertex.y = verts[i + 1] * y; temp.vertex.z = verts[i] * z;
			SetMinMaxPoint(temp.vertex);
			temp.vertexNormal.x = (j == 0 ? 1.0f : -1.0f); temp.vertexNormal.y = 0.0f; temp.vertexNormal.z = 0.0f;
			temp.vertexTexture.x = (j == 0 ? texcd[7 - i] : texcd[i]); temp.vertexTexture.y = (j == 0 ? texcd[7 - (i + 1)] : texcd[(i + 1)]);
			temp.vertexTangent.x = 0; temp.vertexTangent.y = 0; temp.vertexTangent.z = (j == 0 ? 1.0f : -1.0f);
		}
		j == 1 ? pushquad2(LFace, n, n + 1, n + 2, n + 3) : pushquad2(LFace, n + 3, n + 2, n + 1, n);
	}

	for (int j = 0; j < 2; j++)
	{
		int n = (int)LSimpleVertexData.size();
		for (int i = 0; i < 8; i += 2)
		{
			LSimpleVertexData.push_back(SimpleVertexType());
			SimpleVertexType &temp = LSimpleVertexData.back();
			temp.vertex.x = verts[i] * x; temp.vertex.y = (j == 0 ? -1.0f : 1.0f)*y; temp.vertex.z = -verts[i + 1] * z, 1.0f;
			SetMinMaxPoint(temp.vertex);
			temp.vertexNormal.x = 0; temp.vertexNormal.y = (j == 0 ? -1.0f : 1.0f); temp.vertexNormal.z = 0.0f;
			temp.vertexTexture.x = (j == 0 ? texcd[7 - i] : texcd[i]); temp.vertexTexture.y = (j == 0 ? texcd[7 - (i + 1)] : texcd[(i + 1)]);
			temp.vertexTangent.x = (j == 0 ? 1.0f : -1.0f); temp.vertexTangent.y = 0; temp.vertexTangent.z = 0;
		}
		j == 1 ? pushquad2(LFace, n, n + 1, n + 2, n + 3) : pushquad2(LFace, n + 3, n + 2, n + 1, n);
	}
	ComputeSize();
	mTricount = LFace.size();
	mPntcount = LSimpleVertexData.size();
	SubMesh submesh;
	submesh.MaterialID = 1;
	submesh.VertexStart = 0;
	submesh.VertexCount = (unsigned int)mPntcount;
	submesh.FaceStart = 0;
	submesh.FaceCount = (unsigned int)LFace.size();
	submesh.DiffuseColor = mDefaultDiffuseColor;
	submesh.SpecularColor = mDefaultSpecularColor;
	submesh.Shininess = mDefaultShininess;
	LSubMeshList.push_back(submesh);
}


void Shape::StaticProcessNode(aiNode * node, const aiScene * scene)
{

	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		StaticProcessMesh(mesh, scene);
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		StaticProcessNode(node->mChildren[i], scene);
	}


}
void Shape::StaticProcessMesh(aiMesh * mesh, const aiScene * scene)
{
	SubMesh submesh;
	submesh.MaterialID = 1;
	submesh.VertexStart = (unsigned int)LSimpleVertexData.size();
	submesh.VertexCount = mesh->mNumVertices;
	submesh.FaceStart = (unsigned int)LFace.size();
	submesh.FaceCount = mesh->mNumFaces * 3;
	submesh.DiffuseColor = mDefaultDiffuseColor;
	submesh.SpecularColor = mDefaultSpecularColor;
	submesh.Shininess = mDefaultShininess;
	LSubMeshList.push_back(submesh);
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		LSimpleVertexData.push_back(SimpleVertexType());
		SimpleVertexType& temp = LSimpleVertexData.back();
		temp.vertex = Vector3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		temp.vertexNormal = Vector3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		SetMinMaxPoint(temp.vertex);
		if (mesh->mTextureCoords[0])
		{
			temp.vertexTexture = Vector2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}
		else {
			temp.vertexTexture = Vector2(0, 0);
		}
	}
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		LFace.push_back(face.mIndices[0]); LFace.push_back(face.mIndices[1]); LFace.push_back(face.mIndices[2]);
	}
	if (mesh->mMaterialIndex >= 0)
	{

		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		TextureLinker(material, aiTextureType_DIFFUSE);
		TextureLinker(material, aiTextureType_SPECULAR);
		TextureLinker(material, aiTextureType_NORMALS);
		TextureLinker(material, aiTextureType_SHININESS);
		aiColor4D color;
		aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &color);
		LSubMeshList.back().DiffuseColor = Vector3(color.r, color.g, color.b);
		aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &color);
		LSubMeshList.back().SpecularColor = Vector3(color.r, color.g, color.b);
		aiGetMaterialColor(material, AI_MATKEY_SHININESS, &color);
		LSubMeshList.back().Shininess = color.a;
	}
}
/**
Texture Linker
*/
void Shape::TextureLinker(aiMaterial * rpmaterial, aiTextureType rtexturetype)
{
	aiString str;
	aiColor4D color(0.f, 0.f, 0.f, 0.f);

	if (rpmaterial->GetTextureCount(rtexturetype) > 0)
	{
		rpmaterial->GetTexture(rtexturetype, 0, &str);
		string path = str.C_Str();
		if (path.at(0) == '*')
		{
			//read from meney
			path = path.erase(0, 1);
			int index = std::stoi(path);
			if (aiTextureType_DIFFUSE == rtexturetype)
			{
				LSubMeshList.back().DiffuseTextureID = LTextureList[index];
			}
			else if (aiTextureType_SPECULAR == rtexturetype)
			{
				LSubMeshList.back().SpecularTextureID = LTextureList[index];
			}
			else if (aiTextureType_SHININESS == rtexturetype)
			{
				LSubMeshList.back().ShininessTextureID = LTextureList[index];
			}
			else if (aiTextureType_NORMALS == rtexturetype)
			{
				LSubMeshList.back().NormalTextureID = LTextureList[index];
			}
		}
		else
		{
			auto directX_handler = GraphicsManager::GetInstance()->GetDirectXHandler();
			auto device = directX_handler->GetDevice();
			auto device_context = directX_handler->GetDeviceContext();
			//read form the path
			LTextureList.push_back(new DXTexture());
			LTextureList.back()->Initialize(device, device_context, StringtoWstring("./Resource/Image/" + path).c_str());
			int index = (int)LTextureList.size() - 1;
			if (aiTextureType_DIFFUSE == rtexturetype)
			{
				LSubMeshList.back().DiffuseTextureID = LTextureList[index];
			}
			else if (aiTextureType_SPECULAR == rtexturetype)
			{
				LSubMeshList.back().SpecularTextureID = LTextureList[index];
			}
			else if (aiTextureType_SHININESS == rtexturetype)
			{
				LSubMeshList.back().ShininessTextureID = LTextureList[index];
			}
			else if (aiTextureType_NORMALS == rtexturetype)
			{
				LSubMeshList.back().NormalTextureID = LTextureList[index];
			}
		}
	}
}
/**
OBJ loader
*/
OBJ::OBJ(string filename)
{
	mDefaultDiffuseColor = Vector3(1.0, 0, 0);
	mDefaultSpecularColor = Vector3(1.0, 1.0, 1.0);
	mDefaultShininess = 120;
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(filename.c_str(),
		aiProcess_Triangulate | aiProcess_GenNormals);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		return;
	}
	auto directX_handler = GraphicsManager::GetInstance()->GetDirectXHandler();
	auto device = directX_handler->GetDevice();
	auto device_context = directX_handler->GetDeviceContext();
	if (scene->HasTextures())
	{
		for (size_t i = 0; i < scene->mNumTextures; i++)
		{
			LTextureList.push_back(new DXTexture());
			LTextureList.back()->Initialize(device, device_context,
				(unsigned char *)(scene->mTextures[i]->pcData),
				scene->mTextures[i]->mWidth);
		}
	}
	StaticProcessNode(scene->mRootNode, scene);
	ComputeSize();
	ComputeTanget();
	mPntcount = LSimpleVertexData.size();
	mTricount = LFace.size();

}
/**
FBX loader
*/
FBX::FBX(string filename, const vector<string>& animationnamelist)
{
	mDefaultDiffuseColor = Vector3(0.0, 0.0, 1.0);
	mDefaultSpecularColor = Vector3(1.0, 1.0, 1.0);
	mDefaultShininess = 120;
	Assimp::Importer importer;
	ReadAssimpFile(filename);
	const aiScene* scene = importer.ReadFile(filename.c_str(),
		aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FlipWindingOrder);
	
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		return;
	}
	//get texture
	auto directX_handler = GraphicsManager::GetInstance()->GetDirectXHandler();
	auto device = directX_handler->GetDevice();
	auto device_context = directX_handler->GetDeviceContext();
	if (scene->HasTextures())
	{
		for (size_t i = 0; i < scene->mNumTextures; i++)
		{
			LTextureList.push_back(new DXTexture());
			LTextureList.back()->Initialize(device, device_context,
				(unsigned char *)(scene->mTextures[i]->pcData),
				scene->mTextures[i]->mWidth);
		}
	}
	//bone
	mRootBone = ReadBoneHierarchy(scene->mRootNode);

	Matrix rootMatrix = Matrix();

	mRootBone.calcInverseBindTransform(rootMatrix);
	if (LBoneName.size() > 0)mHasBone = true;
	LBoneTr.resize(LBoneName.size());
	LBonePnt.resize(LBoneName.size());//remove for game
	//mesh
	ProcessNode(scene->mRootNode, scene);
	if (scene->mNumAnimations > 0)mHasAnimation = true;
	ComputeSize();

	ComputeTanget();

	//mSimpleVShader->Use();
	AddBoneNodeToArray(mRootBone, LBoneTr, LBonePnt, Matrix()/*::CreateScale(mModelSize)*/);

	mPntcount = LAnimedVertexData.size();
	mTricount = LFace.size();


	
	for (unsigned int i = 0; i < scene->mNumAnimations; i++)
	{
		if (i < animationnamelist.size())
		{
			LoadAnimation(scene->mAnimations[i], animationnamelist[i]);
		}
		else
		{
			LoadAnimation(scene->mAnimations[i]);
		}
	}
}
/**
FBX update
*/
void FBX::Update(float time, string animationName, bool isLoop, float speed)
{
	//mPreviousAnimation = "idle";
	mCurrentAnimation = animationName;
	//LAnimationlist[mPreviousAnimation].Update(time, false, speed);
	LAnimationlist[mCurrentAnimation].Update(time, isLoop, speed);
	//LAnimationlist["idle"].Update(time, true, speed);
	AddBoneNodeToArray(mRootBone, LBoneTr, LBonePnt, Matrix());
	//AddBoneToArray(mRootBone, LBonePnt, mModelTr);//for debug
}
void FBX::Blending(string animationName1, string animationName2,float blendingValue, float time, float speed)
{
	mPreviousAnimation = animationName1;
	mCurrentAnimation = animationName2;
	mBlendingValue = blendingValue;
	LAnimationlist[mPreviousAnimation].Update(time, false, speed);
	LAnimationlist[mCurrentAnimation].Update(time, true, speed);
	AddBlendingToArray(mRootBone, LBoneTr, LBonePnt, Matrix());
}
/**
clear FBX
*/
FBX::~FBX()
{
	LAnimationlist.clear();
	LBoneNametoindex.clear();
	LBoneOffsetMatrix.clear();
	LBoneName.clear();

}
/**
load Animation
*/
void FBX::LoadAnimation(aiAnimation* anim, string animName)
{
	printf("Animation: %s\n  duration (in ticks): %f\n  tickspersecond: %f\n  numchannels: %d\n",
		anim->mName.C_Str(),
		anim->mDuration,
		anim->mTicksPerSecond,
		anim->mNumChannels);
	string useName = anim->mName.C_Str();
	if (animName != "")
	{
		useName = animName;
	}
	LAnimationlist[useName] = AnimationClips(anim->mDuration, anim->mTicksPerSecond, anim->mNumChannels);
	for (unsigned int i = 0; i < anim->mNumChannels; i++) {
		aiNodeAnim* chan = anim->mChannels[i];
		LAnimationlist[useName].addChennel(string(chan->mNodeName.C_Str()), AnimationChannel());
		for (unsigned int i = 0; i < chan->mNumPositionKeys; i++) {
			aiVectorKey key = chan->mPositionKeys[i];
			LAnimationlist[useName].getChennel(string(chan->mNodeName.C_Str())).AddPositionKey(key.mTime,
				key.mValue[0], key.mValue[1], key.mValue[2]);
		}
		for (unsigned int i = 0; i < chan->mNumRotationKeys; i++) {
			aiQuatKey key = chan->mRotationKeys[i];
			LAnimationlist[useName].getChennel(string(chan->mNodeName.C_Str())).AddRotationKey(key.mTime,
				key.mValue.x, key.mValue.y, key.mValue.z, key.mValue.w);
		}
		for (unsigned int i = 0; i < chan->mNumScalingKeys; i++) {
			aiVectorKey key = chan->mScalingKeys[i];
			LAnimationlist[useName].getChennel(string(chan->mNodeName.C_Str())).AddScalingKey(key.mTime,
				key.mValue[0], key.mValue[1], key.mValue[2]);
		}
	}
}

/**
read Bone Hierarchy
*/
BoneNode FBX::ReadBoneHierarchy(const aiNode* node)
{
	LBoneName.push_back(string(node->mName.C_Str()));
	size_t slefid = LBoneName.size() - 1;
	LBoneNametoindex[string(node->mName.C_Str())] = (int)slefid;
	BoneNode boneNode = BoneNode((int)slefid, string(node->mName.C_Str()), Matrix(&node->mTransformation[0][0]));
	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		boneNode.addChild(ReadBoneHierarchy(node->mChildren[i]));
	}
	return boneNode;
}

void FBX::AddAnimationFromFile(string filename, string animationname)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename.c_str(),
		aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FlipWindingOrder);
	if (!scene)
	{
		return;
	}
	LoadAnimation(scene->mAnimations[0], animationname);
}

/**
add Bone Node To Array
*/
void FBX::AddBoneNodeToArray(BoneNode & rootnode, vector<Matrix>& jointMatrices, vector<Matrix>& jointMatrices2, Matrix parentBindTransform)
{
	string name = LBoneName[rootnode.mIndex];
	Matrix bindTransform;

	unordered_map<string, AnimationChannel>& list1 = LAnimationlist[mCurrentAnimation].getChennelList();
	if (list1.find(name) == list1.end())
	{
		bindTransform = parentBindTransform * rootnode.GetLocalBindTransform();
	}
	else
	{
		bindTransform = parentBindTransform * list1[name].GetFinalInterpolate();
	}
	jointMatrices[rootnode.mIndex] = bindTransform * LBoneOffsetMatrix[rootnode.mName];
	jointMatrices2[rootnode.mIndex] = bindTransform;
	for (int i = 0; i < rootnode.LChildren.size(); i++)
	{
		AddBoneNodeToArray(rootnode.LChildren[i], jointMatrices, jointMatrices2, bindTransform);
	}
}

void FBX::AddBlendingToArray(BoneNode & rootnode, vector<Matrix>& jointMatrices, vector<Matrix>& jointMatrices2, Matrix parentBindTransform)
{
	string name = LBoneName[rootnode.mIndex];
	Matrix bindTransform;
	Vector3 FinalTranslation1;
	MyQuaternion FinalRotation1;
	Vector3 FinalScale1;

	Vector3 FinalTranslation2;
	MyQuaternion FinalRotation2;
	Vector3 FinalScale2;

	unordered_map<string, AnimationChannel>& list1 = LAnimationlist[mPreviousAnimation].getChennelList();
	if (list1.find(name) != list1.end())
	{
		FinalTranslation1 = list1[name].GetFinalTranslation();
		FinalRotation1 = list1[name].GetFinalRotation();
		FinalScale1 = list1[name].GetFinalScale();
	}

	unordered_map<string, AnimationChannel>& list2 = LAnimationlist[mCurrentAnimation].getChennelList();
	if (list2.find(name) != list2.end())
	{
		FinalTranslation2 = list2[name].GetFinalTranslation();
		FinalRotation2 = list2[name].GetFinalRotation();
		FinalScale2 = list2[name].GetFinalScale();
	}


	if (list2.find(name) == list2.end())
	{
		bindTransform = parentBindTransform * rootnode.GetLocalBindTransform();
	}
	else
	{
		auto p = TranslationLERP(FinalTranslation1, FinalTranslation2, mBlendingValue);
		auto r = MyQuaternion::Slerp(mBlendingValue, FinalRotation1, FinalRotation2);
		auto s = ScaleLERP(FinalScale1, FinalScale2, mBlendingValue);

		Matrix posM = Matrix::CreateTranslation(p).Transpose();
		Matrix rotationM = r.ToMatrix();
		Matrix scaM = Matrix::CreateScale(s);
		auto FinalInterpolate = posM * rotationM*scaM;
		bindTransform = parentBindTransform * posM * rotationM*scaM;
	}
	jointMatrices[rootnode.mIndex] = bindTransform * LBoneOffsetMatrix[rootnode.mName];
	jointMatrices2[rootnode.mIndex] = bindTransform;
	for (int i = 0; i < rootnode.LChildren.size(); i++)
	{
		AddBlendingToArray(rootnode.LChildren[i], jointMatrices, jointMatrices2, bindTransform);
	}
}

/**
Set BoneInfo
*/
void FBX::SetBoneInfo(int vertexid, int boneId, float WeightID)
{
	XMUINT4& tempId = LAnimedVertexData[vertexid].boneId;
	Vector4& tempWeight = LAnimedVertexData[vertexid].boneWeight;
	if (LNumBoneInfo[vertexid] == 0)
	{
		tempId.x = boneId;
		tempWeight.x = WeightID;
		LNumBoneInfo[vertexid]++;
	}
	else if (LNumBoneInfo[vertexid] == 1)
	{
		tempId.y = boneId;
		tempWeight.y = WeightID;
		LNumBoneInfo[vertexid]++;
	}
	else if (LNumBoneInfo[vertexid] == 2)
	{
		tempId.z = boneId;
		tempWeight.z = WeightID;
		LNumBoneInfo[vertexid]++;
	}
	else
	{
		tempId.w = boneId;
		tempWeight.w = WeightID;
		LNumBoneInfo[vertexid]++;
	}

}
/**
Process Node
*/
void FBX::ProcessNode(aiNode * node, const aiScene * scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh(mesh, scene);
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}
/**
Process Mesh
*/
void FBX::ProcessMesh(aiMesh * mesh, const aiScene * scene)
{
	SubMesh submesh;
	submesh.MaterialID = 1;
	submesh.VertexStart = (unsigned int)LAnimedVertexData.size();
	submesh.VertexCount = mesh->mNumVertices;
	submesh.FaceStart = (unsigned int)LFace.size();
	submesh.FaceCount = mesh->mNumFaces * 3;
	submesh.DiffuseColor = mDefaultDiffuseColor;
	submesh.SpecularColor = mDefaultSpecularColor;
	submesh.Shininess = mDefaultShininess;
	LSubMeshList.push_back(submesh);

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		LAnimedVertexData.push_back(AnimationVertexType());
		AnimationVertexType& temp = LAnimedVertexData.back();
		temp.vertex = Vector3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		temp.vertexNormal = Vector3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		SetMinMaxPoint(temp.vertex);
		LNumBoneInfo.push_back(0);
		if (mesh->mTextureCoords[0])
		{
			temp.vertexTexture = Vector2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}
		else {
			temp.vertexTexture = Vector2(0, 0);
		}
	}
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		LFace.push_back(face.mIndices[0]); LFace.push_back(face.mIndices[1]); LFace.push_back(face.mIndices[2]);
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		TextureLinker(material, aiTextureType_DIFFUSE);
		TextureLinker(material, aiTextureType_SPECULAR);
		TextureLinker(material, aiTextureType_NORMALS);
		TextureLinker(material, aiTextureType_SHININESS);
		aiColor4D color;
		aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &color);
		LSubMeshList.back().DiffuseColor = Vector3(color.r, color.g, color.b);
		aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &color);
		LSubMeshList.back().SpecularColor = Vector3(color.r, color.g, color.b);
		aiGetMaterialColor(material, AI_MATKEY_SHININESS, &color);
		LSubMeshList.back().Shininess = color.a;
	}
	for (unsigned int i = 0; i < mesh->mNumBones; i++) {
		aiBone* bone = mesh->mBones[i];
		string name = string(bone->mName.C_Str());
		int boneID = LBoneNametoindex[name];
		LBoneOffsetMatrix[name] = Matrix(&bone->mOffsetMatrix[0][0]);
		for (unsigned int boneWeight = 0; boneWeight < bone->mNumWeights; boneWeight++) {
			SetBoneInfo(submesh.VertexStart + bone->mWeights[boneWeight].mVertexId, boneID, bone->mWeights[boneWeight].mWeight);
		}

	}
}
/**
create Particles
*/
Particles::Particles(size_t rmaxparticlesnum)
{
	mDefaultDiffuseColor = Vector3(0.0, 1.0, 0.0);
	mDefaultSpecularColor = Vector3(1.0, 1.0, 1.0);
	mDefaultShininess = 120;
	srand((unsigned int)time(NULL));
	mMaxParticles = rmaxparticlesnum;
	mLifeTime = Vector2(0.7f, 0.7f);
	mSpawnNumber = 10;
	mMinVelocity = Vector3(0, 0, 10);
	mMaxVelocity = Vector3(0, 0, 30);
	mMinOffset = Vector3(0, 0, 0);
	mMaxOffset = Vector3(10, 10, 10);
	mParticlelist.resize(mMaxParticles);
	/////////////////////////////////////////

	LSimpleVertexData.resize(mMaxParticles * 6);
	LFace.resize(mMaxParticles * 6);
	for (unsigned int i = 0; i < mMaxParticles * 6; i++)
	{
		LFace[i] = i;
	}
	SubMesh submesh;
	submesh.MaterialID = 2;
	submesh.VertexStart = 0;
	submesh.VertexCount = (unsigned int)mMaxParticles * 6;
	submesh.FaceStart = 0;
	submesh.FaceCount = (unsigned int)mMaxParticles * 6;
	submesh.DiffuseColor = mDefaultDiffuseColor;
	submesh.SpecularColor = mDefaultSpecularColor;
	submesh.Shininess = mDefaultShininess;
	/////////////////
	auto directX_handler = GraphicsManager::GetInstance()->GetDirectXHandler();
	auto device = directX_handler->GetDevice();
	auto device_context = directX_handler->GetDeviceContext();
	LTextureList.push_back(new DXTexture());
	LTextureList.back()->Initialize(device, device_context, StringtoWstring("./Resource/Image/rain.png").c_str());
	//////////////////////
	submesh.DiffuseTextureID = LTextureList[0];
	LSubMeshList.push_back(submesh);
	mPntcount = LSimpleVertexData.size();
	mTricount = LFace.size();

}
/**
clean Particles
*/
Particles::~Particles()
{
	if (mParticlelist.size() > 0)
	{
		mParticlelist.clear();
	}
}
/**
Update Particles
*/
void Particles::Update(float time)
{
	if (mStop == true&& !mActive)
	{
		return;
	}
	if (!mActive&&mStop == false)
	{
		mTotalTime = 0;
		for (size_t i = 0; i < mParticlelist.size(); i++)
		{
			mParticlelist[i].active = false;
		}
		mStop = true;
		return;
	}

	mStop = false;
	mTotalTime += time;
	//update age
	for (size_t i = 0; i < mParticlelist.size(); i++)
	{
		if (mParticlelist[i].active == true)
		{
			mParticlelist[i].age -= time;
		}
	}
	//kill the Particles which are old and out the offset
	KillParticles();
	if (mTotalTime > mSpawnTime)
	{
		//create
		for (size_t i = 0; i < mSpawnNumber; i++)
		{
			EmitParticles();
		}
		mTotalTime = 0;
	}
	//update position
	for (size_t i = 0; i < mParticlelist.size(); i++)
	{
		if (mParticlelist[i].active == true)
		{
			mParticlelist[i].position =
				mParticlelist[i].position +
				time * mParticlelist[i].velocitydir;
		}
	}

}
/**
Get Particle list
*/
vector<ParticleInfo>& Particles::GetParticlelist()
{
	return mParticlelist;
}

/**
Emit Particles
*/
void Particles::EmitParticles()
{
	for (size_t i = 0; i < mParticlelist.size(); i++)
	{
		if (mParticlelist[i].active == false)
		{
			mParticlelist[i].active = true;
			mParticlelist[i].position = Vector3(RandomFloatRange(mMaxOffset.x, mMinOffset.x),
				RandomFloatRange(mMaxOffset.y, mMinOffset.y)
				, 0);
			mParticlelist[i].velocitydir =
				Vector3(RandomFloatRange(mMaxVelocity.x, mMinVelocity.x),
					RandomFloatRange(mMaxVelocity.y, mMinVelocity.y),
					RandomFloatRange(mMaxVelocity.z, mMinVelocity.z));
			mParticlelist[i].age = RandomFloatRange(mLifeTime.x, mLifeTime.y);
			mParticlelist[i].size = 1;
			break;
		}
	}
}
/**
Kill Particles
*/
void Particles::KillParticles()
{
	for (size_t i = 0; i < mParticlelist.size(); i++)
	{
		if (mParticlelist[i].active == true)
		{
			//end life
			if (mParticlelist[i].age <= 0)
			{
				mParticlelist[i].active = false;
			}
			//out the range
			else if ((mParticlelist[i].position.x > mMaxOffset.x ||
				mParticlelist[i].position.y > mMaxOffset.y ||
				mParticlelist[i].position.z > mMaxOffset.z) ||
				(mParticlelist[i].position.x < mMinOffset.x ||
					mParticlelist[i].position.y < mMinOffset.y ||
					mParticlelist[i].position.z < mMinOffset.z))
			{
				mParticlelist[i].active = false;
			}
		}
	}
}
/**
creat Torus
*/
Torus::Torus(const int n)
{
	mDefaultDiffuseColor = Vector3(1.0, 0.0, 1.0);
	mDefaultSpecularColor = Vector3(1.0, 1.0, 1.0);
	mDefaultShininess = 120;

	const float PI = 4.0f*atan(1.0f);
	const float thickness = 0.5f;
	const long localsize = n;
	float major_radius = 1.0f / (1.0f + thickness),
		minor_radius = thickness * major_radius;
	long vertex_count = n * n;
	for (int j = 0; j < localsize; ++j) {
		float phi = 2 * PI*j / localsize;
		for (int i = 0; i < localsize; ++i) {
			int index = localsize * j + i;
			float theta = 2 * PI*i / localsize,
				rho = major_radius + minor_radius * cos(theta);

			LSimpleVertexData.push_back(SimpleVertexType());
			SimpleVertexType& temp = LSimpleVertexData.back();
			temp.vertex = Vector3(rho * cos(phi),
				rho * sin(phi),
				minor_radius * sin(theta));
			SetMinMaxPoint(temp.vertex);
		}
	}
	for (int j = 0; j < localsize; ++j) {
		float phi = 2 * PI*j / localsize;
		for (int i = 0; i < localsize; ++i) {
			int index = localsize * j + i;
			float theta = 2 * PI*i / localsize;
			LSimpleVertexData[index].vertexNormal =
				Vector3(cos(theta)*cos(phi),
					cos(theta)*sin(phi),
					sin(theta));
		}
	}
	for (int j = 0; j < localsize; ++j) {
		int jp1 = (j + 1) % localsize;
		for (int i = 0; i < localsize; ++i) {
			int index = 2 * (localsize*j + i),
				ip1 = (i + 1) % localsize;
			LFace.push_back(localsize * j + i);
			LFace.push_back(localsize * jp1 + i);
			LFace.push_back(localsize * jp1 + ip1);
			LFace.push_back(localsize * j + i);
			LFace.push_back(localsize * jp1 + ip1);
			LFace.push_back(localsize * j + ip1);
		}
	}
	ComputeSize();
	mTricount = LFace.size();
	mPntcount = LSimpleVertexData.size();
	SubMesh submesh;
	submesh.MaterialID = 1;
	submesh.VertexStart = 0;
	submesh.VertexCount = (unsigned int)mPntcount;
	submesh.FaceStart = 0;
	submesh.FaceCount = (unsigned int)LFace.size();
	submesh.DiffuseColor = mDefaultDiffuseColor;
	submesh.SpecularColor = mDefaultSpecularColor;
	submesh.Shininess = mDefaultShininess;
	LSubMeshList.push_back(submesh);
}

const int MAX = 3; // Loops will print at most MAX entries followed by an elipsis.

// Prints a mesh's info; A mesh contains vertices, faces, normals and
// more as needed for graphics.  Vertices are tied to bones with
// weights.
void showMesh(aiMesh* mesh)
{
	// Mesh name and some counts
	printf("Mesh %s: %d vertices, %d faces,  %d bones\n", mesh->mName.C_Str(),
		mesh->mNumVertices, mesh->mNumFaces, mesh->mNumBones);
}

// Prints an animation.  An animation contains a few timing parameters
// and then channels for a number of animated bones.  Each channel
// contains a V, Q, and S keyframe sequences.
void showAnimation(aiAnimation* anim)
{
	printf("Animation: %s\n  duration (in ticks): %f\n  tickspersecond: %f\n  numchannels: %d\n",
		anim->mName.C_Str(),
		anim->mDuration,
		anim->mTicksPerSecond,
		anim->mNumChannels);
}

// Prints the bone hierarchy and relevant info with a graphical
// representation of the hierarchy.
void showBoneHierarchy(const aiScene* scene, const aiNode* node, const int level = 0)
{
}

void ReadAssimpFile(const string& path)
{
	printf("Reading %s\n", path.c_str());
	Assimp::Importer importer;

	// A single call returning a single structure for the complete file.
	const aiScene* scene = importer.ReadFile(path.c_str(),
		aiProcess_Triangulate | aiProcess_GenNormals);

	printf("  %d animations\n", scene->mNumAnimations); // This is what 460/560 is all about
	printf("  %d meshes\n", scene->mNumMeshes);         // Verts and faces for the skin.
	printf("  %d materials\n", scene->mNumMaterials);   // More graphics info
	printf("  %d textures\n", scene->mNumTextures);     // More graphics info
	printf("\n");

	// Prints a graphical representation of the bone hierarchy.
	showBoneHierarchy(scene, scene->mRootNode);

	// Prints all the animation info for each animation in the file
	printf("\n");
	for (int i = 0; i < scene->mNumAnimations; i++)
		showAnimation(scene->mAnimations[i]);

	// Prints all the mesh info for each mesh in the file
	printf("\n");
	for (int i = 0; i < scene->mNumMeshes; i++)
		showMesh(scene->mMeshes[i]);
}