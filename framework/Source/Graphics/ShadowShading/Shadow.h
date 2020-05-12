/**
 * @file Shadow.h
 * @author Ching-Yen Lin.
 * @date 10/5/19
 * @brief File to handle shadows
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */



#pragma once
#include "../../pch.h"
class Shadow
{
public:
	Shadow() {}
	void Initialize(float, float);
	Shadow(const Shadow&) {}
	void StartDrawShadow();
	void ContinousDrawShadow();

	void FinishDrawShadow();
	void BindShadowMap(int);
	void UnBindShadowMap(int);
	~Shadow();
private:

	ID3D11DeviceContext* pDeviceContext;
	ID3D11Texture2D* pRenderTargetTexture;
	ID3D11RenderTargetView* pRenderTargetView;
	ID3D11ShaderResourceView* pDepthMapSRV;

	ID3D11Texture2D* pDepthBuff;
	ID3D11DepthStencilView* pDepthMapDSV;
	D3D11_VIEWPORT ViewPort;

};

