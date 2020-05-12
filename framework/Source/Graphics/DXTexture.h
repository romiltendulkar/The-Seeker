/**
 * @file DXTexture.h
 * @author Ching-Yen Lin
 * @date 9/23/19
 * @brief This file content DXTexture member
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#ifndef DXTEXTURE_H
#define DXTEXTURE_H
//#define _HAS_STD_BYTE 0
#include "../pch.h"
using namespace std;
class DXTexture
{
public:
	DXTexture() {
		pDeviceContext = 0; pTexture = 0;
	}
	/**
	 Initial the DXTexture from file
	*/
	void Initialize(ID3D11Device* device, ID3D11DeviceContext* devicecontext, const wchar_t* filepath);

	void Initialize(ID3D11Device* device, ID3D11DeviceContext* devicecontext);
	/**
	 Initial the DXTexture from memory
	*/
	void Initialize(ID3D11Device* device, ID3D11DeviceContext* devicecontext, const unsigned char* databuff, size_t buffsize);
	
	/**
	 Initial the DXTexture from memory
	*/
	void Initialize(ID3D11Device* device, ID3D11DeviceContext* devicecontext, FT_Bitmap* );

	/**
	clean menber
	*/
	~DXTexture();

	/**
	Get Texture pointer
	*/
	ID3D11ShaderResourceView* GetTexture();

	/**
	use Texture with slot id
	*/
	void Bind(int);

	/**
	unuse Texture with slot id
	*/
	void UnBind(int);

	void SetTextureResizeMode(bool);
	bool GetTextureResizeMode();
	void SetTextureSize(Vector2);
	Vector4& GetTextureSize();
	ID3D11ShaderResourceView* pTexture;

private:
	ID3D11DeviceContext* pDeviceContext;
	bool mTextureResizeMode = 0;
	Vector2 mTextureSize;


};
#endif

