/**
 * @file DXTexture.cpp
 * @author Ching-Yen Lin
 * @date 9/23/19
 * @brief This file content DXTexture member
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#include "../pch.h"
#include "DXTexture.h"

 /**
  Initial the DXTexture from file
 */
void DXTexture::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const wchar_t* filepath)
{
	mTextureSize = Vector2(1, 1);
	mTextureResizeMode = 0;
	Microsoft::WRL::ComPtr<ID3D11Resource>  res;
	pDeviceContext = deviceContext;
	HRESULT result = CreateWICTextureFromFile(device, filepath, &res, &pTexture);
	
}

void DXTexture::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	pDeviceContext = deviceContext;
}
/**
 Initial the DXTexture from memory
*/
void DXTexture::Initialize(ID3D11Device * device, ID3D11DeviceContext * devicecontext, const unsigned char * databuff, size_t buffsize)
{
	mTextureSize = Vector2(1, 1);
	mTextureResizeMode = 0;
	Microsoft::WRL::ComPtr<ID3D11Resource>  res;
	pDeviceContext = devicecontext;
	HRESULT result = CreateWICTextureFromMemory(device, databuff, buffsize, &res, &pTexture);
}

void DXTexture::Initialize(ID3D11Device * device, ID3D11DeviceContext * devicecontext, FT_Bitmap* bitmap)
{
	mTextureSize = Vector2(1, 1);
	mTextureResizeMode = 0;
	pDeviceContext = devicecontext;
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = bitmap->width;
	desc.Height = bitmap->rows;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8_UNORM;
	desc.SampleDesc.Quality = 0;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	HRESULT result;
	ID3D11Texture2D *Texture2D;
	ZeroMemory(&Texture2D, sizeof(Texture2D));
	D3D11_SUBRESOURCE_DATA subResource;
	subResource.pSysMem = bitmap->buffer;
	subResource.SysMemPitch = bitmap->pitch;
	subResource.SysMemSlicePitch = 0;
	result=device->CreateTexture2D(&desc, &subResource, &Texture2D);

	// Create texture view
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	//ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	result = device->CreateShaderResourceView(Texture2D, &srvDesc, &pTexture);
}

/**
clean menber
*/
DXTexture::~DXTexture()
{
	if (pTexture)
	{
		pTexture->Release();
		pTexture = 0;
	}
}
/**
Get Texture pointer
*/
ID3D11ShaderResourceView * DXTexture::GetTexture()
{
	return pTexture;
}
/**
use Texture with slot id
*/
void DXTexture::Bind(int textureID)
{
	pDeviceContext->PSSetShaderResources(textureID, 1, &pTexture);
}
/**
unuse Texture with slot id
*/
void DXTexture::UnBind(int rtextureid)
{
	ID3D11ShaderResourceView* pnull_srv = nullptr;
	pDeviceContext->PSSetShaderResources(rtextureid, 1, &pnull_srv);
}
void DXTexture::SetTextureResizeMode(bool mode)
{
	mTextureResizeMode = mode;//0 no resize //1 manually resize
}
bool DXTexture::GetTextureResizeMode()
{
	return mTextureResizeMode;
}
void DXTexture::SetTextureSize(Vector2 wh)
{
	mTextureSize = wh;
}
Vector4& DXTexture::GetTextureSize()
{
	return Vector4(mTextureSize);
}