/**
 * @file DeferredRendering.h
 * @author Ching-Yen Lin.
 * @date 10/5/19
 * @brief File to handle deferred shading
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#include "../../pch.h"


#include "DeferredRendering.h"
#include "../../../Manager/GraphicsManager.h"
DeferredRendering::DeferredRendering()
{
	int i;

	for (i = 0; i < BUFFER_COUNT; i++)
	{
		pRenderTargetTextureArray[i] = 0;
		pRenderTargetViewArray[i] = 0;
		pShaderResourceViewArray[i] = 0;
	}
	pDepthStencilBuffer = 0;
	pDepthStencilView = 0;
	ZeroMemory(&mViewport, sizeof(D3D11_VIEWPORT));
}
/**
	Initializes the context for deferred shading
*/
void DeferredRendering::Initialize(int rwitgh, int rheight)
{
	auto directX_handler = GraphicsManager::GetInstance()->GetDirectXHandler();
	auto device = directX_handler->GetDevice();
	auto device_context = directX_handler->GetDeviceContext();
	pDeviceContext = device_context;


	HRESULT result;
	// Store the mWidth and mHeight of the render texture.
	mWidth = rwitgh;
	mHeight = rheight;

	//position normal diffuse
	D3D11_TEXTURE2D_DESC rgb_texture_desc;
	// Setup the render target texture description.
	rgb_texture_desc.Width = mWidth;
	rgb_texture_desc.Height = mHeight;
	rgb_texture_desc.MipLevels = 1;
	rgb_texture_desc.ArraySize = 1;
	rgb_texture_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	rgb_texture_desc.SampleDesc.Count = 1;
	rgb_texture_desc.SampleDesc.Quality = 0;
	rgb_texture_desc.Usage = D3D11_USAGE_DEFAULT;
	rgb_texture_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	rgb_texture_desc.CPUAccessFlags = 0;
	rgb_texture_desc.MiscFlags = 0;

	D3D11_TEXTURE2D_DESC rgb_texture_desc2;
	// Setup the render target texture description.
	rgb_texture_desc2 = rgb_texture_desc;
	rgb_texture_desc2.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;


	for (size_t i = 0; i < BUFFER_COUNT; i++)
	{
		if (i == 0)
			result = device->CreateTexture2D(&rgb_texture_desc, 0, &(pRenderTargetTextureArray[i]));
		else
			result = device->CreateTexture2D(&rgb_texture_desc2, 0, &(pRenderTargetTextureArray[i]));
	}

	D3D11_RENDER_TARGET_VIEW_DESC rgb_render_target_view_desc;
	// Setup the description of the render target view.
	rgb_render_target_view_desc.Format = rgb_texture_desc.Format;
	rgb_render_target_view_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rgb_render_target_view_desc.Texture2D.MipSlice = 0;

	D3D11_RENDER_TARGET_VIEW_DESC rgb_render_target_view_desc2;
	// Setup the description of the render target view.
	rgb_render_target_view_desc2.Format = rgb_texture_desc2.Format;
	rgb_render_target_view_desc2.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rgb_render_target_view_desc2.Texture2D.MipSlice = 0;

	for (size_t i = 0; i < BUFFER_COUNT; i++)
	{
		if (i == 0)
			result = device->CreateRenderTargetView(pRenderTargetTextureArray[i], &rgb_render_target_view_desc, &pRenderTargetViewArray[i]);
		else
			result = device->CreateRenderTargetView(pRenderTargetTextureArray[i], &rgb_render_target_view_desc2, &pRenderTargetViewArray[i]);
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC rgb_shader_resource_view_desc;
	// Setup the description of the shader resource view.
	rgb_shader_resource_view_desc.Format = rgb_texture_desc.Format;
	rgb_shader_resource_view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	rgb_shader_resource_view_desc.Texture2D.MostDetailedMip = 0;
	rgb_shader_resource_view_desc.Texture2D.MipLevels = 1;

	D3D11_SHADER_RESOURCE_VIEW_DESC rgb_shader_resource_view_desc2;
	// Setup the description of the shader resource view.
	rgb_shader_resource_view_desc2.Format = rgb_texture_desc2.Format;
	rgb_shader_resource_view_desc2.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	rgb_shader_resource_view_desc2.Texture2D.MostDetailedMip = 0;
	rgb_shader_resource_view_desc2.Texture2D.MipLevels = 1;

	// Create the shader resource views.
	for (size_t i = 0; i < BUFFER_COUNT; i++)
	{
		if (i == 0)
			result = device->CreateShaderResourceView(pRenderTargetTextureArray[i], &rgb_shader_resource_view_desc, &pShaderResourceViewArray[i]);
		else
			result = device->CreateShaderResourceView(pRenderTargetTextureArray[i], &rgb_shader_resource_view_desc2, &pShaderResourceViewArray[i]);
	}

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	// Set up the description of the mDepth buffer.
	depthBufferDesc.Width = mWidth;
	depthBufferDesc.Height = mHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;
	// Create the texture for the mDepth buffer using the filled out description.
	result = device->CreateTexture2D(&depthBufferDesc, NULL, &pDepthStencilBuffer);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	// Set up the mDepth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the mDepth stencil view.
	result = device->CreateDepthStencilView(pDepthStencilBuffer, &depthStencilViewDesc, &pDepthStencilView);

	// Setup the viewport for rendering.
	mViewport.Width = (float)mWidth;
	mViewport.Height = (float)mHeight;
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;
	mViewport.TopLeftX = 0.0f;
	mViewport.TopLeftY = 0.0f;
}

DeferredRendering::~DeferredRendering()
{
	for (size_t i = 0; i < BUFFER_COUNT; i++)
	{
		RELEASE_COM(pRenderTargetTextureArray[i]);
		RELEASE_COM(pRenderTargetViewArray[i]);
		RELEASE_COM(pShaderResourceViewArray[i]);
	}
	RELEASE_COM(pDepthStencilBuffer);
	RELEASE_COM(pDepthStencilView);
}
/**
	Uses deferred shading context/shader
*/
void DeferredRendering::Use(float rred, float rgreen, float rblue, float ralpha)
{
	//clean
	float color[4] = { rred, rgreen, rblue, ralpha };
	for (size_t i = 0; i < BUFFER_COUNT; i++)
	{
		pDeviceContext->ClearRenderTargetView(pRenderTargetViewArray[i], color);
	}
	pDeviceContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	pDeviceContext->RSSetViewports(1, &mViewport);
	pDeviceContext->OMSetRenderTargets(BUFFER_COUNT, pRenderTargetViewArray, pDepthStencilView);
}
void DeferredRendering::Reuse()
{
	pDeviceContext->RSSetViewports(1, &mViewport);
	pDeviceContext->OMSetRenderTargets(BUFFER_COUNT, pRenderTargetViewArray, pDepthStencilView);
}


void DeferredRendering::UseDepthStencilView()//not useful
{
	auto directX_handler = GraphicsManager::GetInstance()->GetDirectXHandler();
	directX_handler->SetDepthStencilView(pDepthStencilView);
}

void DeferredRendering::Unuse()
{
	//reset Render Target
	auto directX_handler = GraphicsManager::GetInstance()->GetDirectXHandler();
	ID3D11RenderTargetView* pNullRTV[BUFFER_COUNT] = { nullptr,nullptr,nullptr,nullptr,nullptr,nullptr };
	pDeviceContext->OMSetRenderTargets(BUFFER_COUNT, pNullRTV, nullptr);
	pDeviceContext->OMSetRenderTargets(BUFFER_COUNT, pRenderTargetViewArray, nullptr);
	directX_handler->ResetRenderTarget();
}

void DeferredRendering::BindGBuffer(int rtextureid)
{
	pDeviceContext->PSSetShaderResources(rtextureid, BUFFER_COUNT, pShaderResourceViewArray);
}

void DeferredRendering::UnBindGBuffer(int rtextureid)
{
	ID3D11ShaderResourceView* nullResourceViewArray[BUFFER_COUNT] = { nullptr,nullptr,nullptr,nullptr,nullptr,nullptr };
	pDeviceContext->PSSetShaderResources(rtextureid, BUFFER_COUNT, nullResourceViewArray);
}

ID3D11ShaderResourceView ** DeferredRendering::getParticleLevel()
{
	return &pShaderResourceViewArray[5];
}

void DeferredRendering::Resize(int rwitgh, int rheight)
{
	auto directX_handler = GraphicsManager::GetInstance()->GetDirectXHandler();
	auto device = directX_handler->GetDevice();
	auto device_context = directX_handler->GetDeviceContext();
	pDeviceContext = device_context;


	HRESULT result;
	// Store the mWidth and mHeight of the render texture.
	mWidth = rwitgh;
	mHeight = rheight;

	//position normal diffuse
	D3D11_TEXTURE2D_DESC rgb_texture_desc;
	// Setup the render target texture description.
	rgb_texture_desc.Width = mWidth;
	rgb_texture_desc.Height = mHeight;
	rgb_texture_desc.MipLevels = 1;
	rgb_texture_desc.ArraySize = 1;
	rgb_texture_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	rgb_texture_desc.SampleDesc.Count = 1;
	rgb_texture_desc.SampleDesc.Quality = 0;
	rgb_texture_desc.Usage = D3D11_USAGE_DEFAULT;
	rgb_texture_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	rgb_texture_desc.CPUAccessFlags = 0;
	rgb_texture_desc.MiscFlags = 0;

	//normal
	D3D11_TEXTURE2D_DESC rgb_texture_desc2;
	rgb_texture_desc2 = rgb_texture_desc;
	rgb_texture_desc2.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

	for (size_t i = 0; i < BUFFER_COUNT; i++)
	{
		RELEASE_COM(pRenderTargetTextureArray[i]);
	}
	for (size_t i = 0; i < BUFFER_COUNT; i++)
	{
		if (i == 0)
			result = device->CreateTexture2D(&rgb_texture_desc, 0, &(pRenderTargetTextureArray[i]));
		else
			result = device->CreateTexture2D(&rgb_texture_desc2, 0, &(pRenderTargetTextureArray[i]));
	}

	D3D11_RENDER_TARGET_VIEW_DESC rgb_render_target_view_desc;
	// Setup the description of the render target view.
	rgb_render_target_view_desc.Format = rgb_texture_desc.Format;
	rgb_render_target_view_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rgb_render_target_view_desc.Texture2D.MipSlice = 0;

	D3D11_RENDER_TARGET_VIEW_DESC rgb_render_target_view_desc2;
	// Setup the description of the render target view.
	rgb_render_target_view_desc2.Format = rgb_texture_desc2.Format;
	rgb_render_target_view_desc2.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rgb_render_target_view_desc2.Texture2D.MipSlice = 0;

	for (size_t i = 0; i < BUFFER_COUNT; i++)
	{
		RELEASE_COM(pRenderTargetViewArray[i]);
	}
	for (size_t i = 0; i < BUFFER_COUNT; i++)
	{
		for (size_t i = 0; i < BUFFER_COUNT; i++)
		{
			if (i == 0)
				result = device->CreateRenderTargetView(pRenderTargetTextureArray[i], &rgb_render_target_view_desc, &pRenderTargetViewArray[i]);
			else
				result = device->CreateRenderTargetView(pRenderTargetTextureArray[i], &rgb_render_target_view_desc2, &pRenderTargetViewArray[i]);
		}
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC rgb_shader_resource_view_desc;
	// Setup the description of the shader resource view.
	rgb_shader_resource_view_desc.Format = rgb_texture_desc.Format;
	rgb_shader_resource_view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	rgb_shader_resource_view_desc.Texture2D.MostDetailedMip = 0;
	rgb_shader_resource_view_desc.Texture2D.MipLevels = 1;

	D3D11_SHADER_RESOURCE_VIEW_DESC rgb_shader_resource_view_desc2;
	// Setup the description of the shader resource view.
	rgb_shader_resource_view_desc2.Format = rgb_texture_desc2.Format;
	rgb_shader_resource_view_desc2.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	rgb_shader_resource_view_desc2.Texture2D.MostDetailedMip = 0;
	rgb_shader_resource_view_desc2.Texture2D.MipLevels = 1;

	for (size_t i = 0; i < BUFFER_COUNT; i++)
	{
		RELEASE_COM(pShaderResourceViewArray[i]);
	}
	// Create the shader resource views.
	for (size_t i = 0; i < BUFFER_COUNT; i++)
	{
		if (i == 0)
			result = device->CreateShaderResourceView(pRenderTargetTextureArray[i], &rgb_shader_resource_view_desc, &pShaderResourceViewArray[i]);
		else
			result = device->CreateShaderResourceView(pRenderTargetTextureArray[i], &rgb_shader_resource_view_desc2, &pShaderResourceViewArray[i]);
	}

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	// Set up the description of the mDepth buffer.
	depthBufferDesc.Width = mWidth;
	depthBufferDesc.Height = mHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;
	RELEASE_COM(pDepthStencilBuffer);
	// Create the texture for the mDepth buffer using the filled out description.
	result = device->CreateTexture2D(&depthBufferDesc, NULL, &pDepthStencilBuffer);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	// Set up the mDepth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	RELEASE_COM(pDepthStencilView);
	// Create the mDepth stencil view.
	result = device->CreateDepthStencilView(pDepthStencilBuffer, &depthStencilViewDesc, &pDepthStencilView);

	// Setup the viewport for rendering.
	mViewport.Width = (float)mWidth;
	mViewport.Height = (float)mHeight;
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;
	mViewport.TopLeftX = 0.0f;
	mViewport.TopLeftY = 0.0f;
}