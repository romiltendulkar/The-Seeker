/**
 * @file DXShader.h
 * @author Ching-Yen Lin
 * @date 9/01/19
 * @brief This file content DXObject member
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#ifndef DXSHADER_H
#define DXSHADER_H
//#define _HAS_STD_BYTE 0

#include "../pch.h"
using namespace std;
enum SHADERTYPE { PS, VS, CS, GS, FX };
class DXShader
{
public:
	/**
	initial member to null
	*/
	DXShader(ID3D11Device*, ID3D11DeviceContext*);

	/**
	clean member to null
	*/
	~DXShader();

	/**
	Add Shader
	*/
	bool AddShader(const wchar_t* shaderFile,
		SHADERTYPE shaderModel, HWND* hwnd = 0);

	/**
	Add Layout
	*/
	bool AddLayout(vector<D3D11_INPUT_ELEMENT_DESC>);

	/**
	 use shader
	*/
	void Use();

	/**
	 unuse shader
	*/
	void Unuse();
private:
	ID3D11Device* mDevice;
	ID3D11DeviceContext* mDeviceContext;
	ID3D11InputLayout* mLayout;
	ID3D11VertexShader* mVShader;
	ID3D11PixelShader* mPShader;
	ID3D11ComputeShader* mCShader;
	ID3D11GeometryShader* mGShader;
	vector<uint8_t> LVectexBuffer;
};
#endif // 
