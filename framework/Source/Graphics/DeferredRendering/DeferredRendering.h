/**
 * @file DeferredRendering.h
 * @author Ching-Yen Lin.
 * @date 10/5/19
 * @brief File to handle deferred shading
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */

#pragma once
#include "../../pch.h"
#define BUFFER_COUNT 6
class DeferredRendering
{
public:
	DeferredRendering();

	/**
	Initializes the context for deferred shading
	*/
	void Initialize(int, int);
	~DeferredRendering();
	void Use(float, float, float, float);
	void Reuse();
	/**
		Resize FBO on screen change
	*/
	void Resize(int, int);
	void UseDepthStencilView();

	/**
	Unuses deferred shading context/shader.
	*/
	void Unuse();

	/**
		Binds GBuffer FBO
	*/
	void BindGBuffer(int);

	/**
		Unbinds GBuffer FBO
	*/
	void UnBindGBuffer(int);
	ID3D11ShaderResourceView** getParticleLevel();
private:
	int mWidth, mHeight;
	ID3D11DeviceContext* pDeviceContext;
	ID3D11Texture2D* pRenderTargetTextureArray[BUFFER_COUNT];
	ID3D11RenderTargetView* pRenderTargetViewArray[BUFFER_COUNT];
	ID3D11ShaderResourceView* pShaderResourceViewArray[BUFFER_COUNT];
	ID3D11Texture2D* pDepthStencilBuffer;
	ID3D11DepthStencilView* pDepthStencilView;
	D3D11_VIEWPORT mViewport;
};

