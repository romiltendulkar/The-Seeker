/**
 * @file Shadow.h
 * @author Ching-Yen Lin.
 * @date 10/5/19
 * @brief File to handle shadows
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */


#include "../../pch.h"
#include "Shadow.h"
#include "../../../Manager/GraphicsManager.h"

 /**
	 Initializes the context for shadows
 */

void Shadow::Initialize(float rwitgh,float rheight)
{
	HRESULT result;
	ZeroMemory(&ViewPort, sizeof(D3D11_VIEWPORT));
	ViewPort.TopLeftX = 0;
	ViewPort.TopLeftY = 0;
	ViewPort.Width = static_cast<float>(rwitgh);
	ViewPort.Height = static_cast<float>(rheight);
	ViewPort.MinDepth = 0.0f;
	ViewPort.MaxDepth = 1.0f;

	auto directX_handler = GraphicsManager::GetInstance()->GetDirectXHandler();
	auto device = directX_handler->GetDevice();
	auto device_context = directX_handler->GetDeviceContext();
	pDeviceContext = device_context;
	
	//create texture for shadow map
	D3D11_TEXTURE2D_DESC texture_desc; 
	texture_desc.Width = (UINT)rwitgh;
	texture_desc.Height = (UINT)rheight;
	texture_desc.MipLevels = 1;
	texture_desc.ArraySize = 1;
	texture_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texture_desc.SampleDesc.Count = 1;
	texture_desc.SampleDesc.Quality = 0;
	texture_desc.Usage = D3D11_USAGE_DEFAULT;
	texture_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texture_desc.CPUAccessFlags = 0;
	texture_desc.MiscFlags = 0;
	result = device->CreateTexture2D(&texture_desc,0, &pRenderTargetTexture);

	D3D11_RENDER_TARGET_VIEW_DESC rgb_render_target_view_desc;
	// Setup the description of the render target view.
	rgb_render_target_view_desc.Format = texture_desc.Format;
	rgb_render_target_view_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rgb_render_target_view_desc.Texture2D.MipSlice = 0;


	result = device->CreateRenderTargetView(pRenderTargetTexture, &rgb_render_target_view_desc, &pRenderTargetView);

	D3D11_SHADER_RESOURCE_VIEW_DESC rgb_shader_resource_view_desc;
	// Setup the description of the shader resource view.
	rgb_shader_resource_view_desc.Format = texture_desc.Format;
	rgb_shader_resource_view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	rgb_shader_resource_view_desc.Texture2D.MostDetailedMip = 0;
	rgb_shader_resource_view_desc.Texture2D.MipLevels = 1;

	result = device->CreateShaderResourceView(pRenderTargetTexture, &rgb_shader_resource_view_desc, &pDepthMapSRV);

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	// Set up the description of the mDepth buffer.
	depthBufferDesc.Width =(UINT) rwitgh;
	depthBufferDesc.Height = (UINT)rheight;
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
	result = device->CreateTexture2D(&depthBufferDesc, NULL, &pDepthBuff);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	// Set up the mDepth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the mDepth stencil view.
	result = device->CreateDepthStencilView(pDepthBuff, &depthStencilViewDesc, &pDepthMapDSV);
}
/**
	Starts drawing shadow
*/
void Shadow::StartDrawShadow()
{
	//set clean Render Target
	float color[4] = { 0, 0, 0, 0 };
	pDeviceContext->ClearRenderTargetView(pRenderTargetView, color);
	pDeviceContext->ClearDepthStencilView(pDepthMapDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
	pDeviceContext->RSSetViewports(1, &ViewPort);
	pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthMapDSV);
}
void Shadow::ContinousDrawShadow()
{
	pDeviceContext->RSSetViewports(1, &ViewPort);
	pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthMapDSV);
}
/**
	Finishes drawing shadows
*/
void Shadow::FinishDrawShadow()
{
	//reset Render Target
	auto directX_handler = GraphicsManager::GetInstance()->GetDirectXHandler();
	directX_handler->ResetRenderTarget();
}

/**
	Binds FBO
*/
void Shadow::BindShadowMap(int rtextureid)
{
	pDeviceContext->PSSetShaderResources(rtextureid, 1, &pDepthMapSRV);
}

/**
	UnBinds FBO
*/
void Shadow::UnBindShadowMap(int rtextureid)
{
	ID3D11ShaderResourceView* pnull_srv = nullptr;
	pDeviceContext->PSSetShaderResources(rtextureid, 1, &pnull_srv);
}

Shadow::~Shadow()
{
	RELEASE_COM(pRenderTargetTexture);
	RELEASE_COM(pRenderTargetView);
	RELEASE_COM(pDepthBuff);
	RELEASE_COM(pDepthMapDSV);
	RELEASE_COM(pDepthMapSRV);
}
