/**
 * @file DXShader.cpp
 * @author Ching-Yen Lin
 * @date 9/01/19
 * @brief This file content DXShader function like initial and use
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#include "../pch.h"
#include "DXShader.h"
//#define _HAS_STD_BYTE 0
using namespace std;
/**
initial member to null
*/
DXShader::DXShader(ID3D11Device* device, ID3D11DeviceContext*devicecontext)
{
	mDevice = device;
	mDeviceContext = devicecontext;
	//VectexBuffer = 0;
	mVShader = 0;
	mCShader = 0;
	mPShader = 0;
	mGShader = 0;
	mLayout = 0;
}

/**
clean member to null
*/
DXShader::~DXShader()
{
	mDevice = nullptr;
	mDeviceContext = nullptr;
	if (mVShader)mVShader->Release(); mVShader = 0;
	if (mPShader)mPShader->Release(); mPShader = 0;
	if (mCShader)mCShader->Release(); mCShader = 0;
	if (mGShader)mGShader->Release(); mVShader = 0;
	if (mLayout)mLayout->Release(); mLayout = 0;
}
/**
Add Shader
*/
bool DXShader::AddShader(const wchar_t* shaderFile, SHADERTYPE shaderModel, HWND* hwnd)
{
	HRESULT result;
	ID3D10Blob* error = 0;

	vector<uint8_t> shaderdata;
	if (shaderModel == VS)
	{
		//VERTEX_SHADER:
		shaderdata = DX::ReadData(shaderFile);
		result = mDevice->CreateVertexShader(shaderdata.data(), shaderdata.size(),
			NULL, &mVShader);
		LVectexBuffer = shaderdata;
	}
	else if (shaderModel == PS)
	{
		//FRAGMENT_SHADER:
		shaderdata = DX::ReadData(shaderFile);
		result = mDevice->CreatePixelShader(shaderdata.data(), shaderdata.size(),
			NULL, &mPShader);
	}
	else if (shaderModel == CS)
	{
		//COMPUTE_SHADER:
		shaderdata = DX::ReadData(shaderFile);
		auto hr =mDevice->CreateComputeShader(shaderdata.data(), shaderdata.size(),
			NULL, &mCShader);
	}
	else if (shaderModel == GS)
	{
		//GEOMETRY_SHADER:
		shaderdata = DX::ReadData(shaderFile);
		mDevice->CreateGeometryShader(shaderdata.data(), shaderdata.size(),
			NULL, &mGShader);
	}
	else if (shaderModel == FX)
	{
		shaderdata = DX::ReadData(shaderFile);
	}
	else return false;


	if (shaderModel != VS)
	{
		shaderdata.clear();
	}

	return true;
}
/**
Add Layout
*/
bool DXShader::AddLayout(vector<D3D11_INPUT_ELEMENT_DESC> polygonLayout)
{
	HRESULT result;
	//size_t num = polygonLayout.size();
	result = mDevice->CreateInputLayout(
		&polygonLayout[0],
		UINT(polygonLayout.size()),
		LVectexBuffer.data(),
		LVectexBuffer.size(),
		&mLayout);
	if (FAILED(result))
	{
		return false;
	}
	LVectexBuffer.clear();
	return true;
}

/**
 use shader
*/
void DXShader::Use()
{

	if (mVShader)
	{
		mDeviceContext->IASetInputLayout(mLayout);
		mDeviceContext->VSSetShader(mVShader, NULL, 0);
	}
	else if (mPShader)
	{
		mDeviceContext->PSSetShader(mPShader, NULL, 0);
	}
	else if (mCShader)
	{
		mDeviceContext->CSSetShader(mCShader, NULL, 0);
	}
	else if (mGShader)
	{
		mDeviceContext->GSSetShader(mGShader, NULL, 0);
	}

}
/**
 unuse shader
*/
void DXShader::Unuse()
{
	if (mVShader)
	{
		mDeviceContext->IASetInputLayout(0);
		mDeviceContext->VSSetShader(NULL, NULL, 0);
	}
	else if (mPShader)
	{
		mDeviceContext->PSSetShader(NULL, NULL, 0);
	}
	else if (mCShader)
	{
		mDeviceContext->CSSetShader(NULL, NULL, 0);
	}
	else if (mGShader)
	{
		mDeviceContext->GSSetShader(NULL, NULL, 0);
	}
}