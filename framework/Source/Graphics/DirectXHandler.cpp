/**
 * @file DirectXHandler.cpp
 * @author Ching-Yen Lin
 * @date 9/01/19
 * @brief This file content useful function to interactive DirectX
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#include "../pch.h"
#include "DirectXHandler.h"
 /**
  initial all member value to null
 */
DirectXHandler::DirectXHandler()
{
	mFullScreen = 0;
	mScreenWidth = 0;
	mScreenHeight = 0;
	mFieldOfView = 0;
	mScreenDepth = 0;
	mScreenNear = 0;
	mMSAACount = 0;
	mMSAAQuality = 0;
	mEnableMSAA = 0;
	mVsyncEnabled = 0;
	mVideoCardMemory = 0;
	mNumerator = 0;
	mDenominator = 0;
	pDevice = 0;
	pDeviceContext = 0;
	pSwapChain = 0;
	pRenderTargetView = 0;
	pDepthStencilState = 0;
	pDepthDisabledStencilState = 0;
	pDepthStencilView = 0;
	pDepthStencilBuffer = 0;
	pCullBack = 0;
	pNoCull = 0;
	pOneOneBlendingState = 0;
	pTransparentBlendingState = 0;
	ZeroMemory(&mViewPort, sizeof(D3D11_VIEWPORT));
}


DirectXHandler::~DirectXHandler()
{
}


bool DirectXHandler::Initialize(int rScreenWidth, int rScreenHeight, bool rVsync, HWND rHWND, bool rFullScreen,
	float rScreenDepth, float rScreenNear)
{
	mFullScreen = rFullScreen;
	mScreenWidth = rScreenWidth;
	mScreenHeight = rScreenHeight;
	mVsyncEnabled = rVsync;
	mScreenDepth = rScreenDepth;
	mScreenNear = rScreenNear;
	
	UINT create_device_flags = 0;
//#if defined(DEBUG) || defined(_DEBUG)  
//	create_device_flags |= D3D11_CREATE_DEVICE_DEBUG;
//	std::cout << "DEBUG" << std::endl;
//#endif
	
	D3D_FEATURE_LEVEL feature_level;
	HRESULT result = D3D11CreateDevice(
		0,                 // default adapter
		D3D_DRIVER_TYPE_HARDWARE,
		0,                 // no software device
		create_device_flags,
		0, 0,              // default feature level array
		D3D11_SDK_VERSION,
		&pDevice,
		&feature_level,
		&pDeviceContext);
	//check directX is working or not
	if (FAILED(result))
	{
		MessageBox(0, "D3D11CreateDevice Failed.", 0, 0);
		return false;
	}
	//check directX 11 is supported
	if (feature_level != D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(0, "Direct3D Feature Level 11 unsupported.", 0, 0);
		return false;
	}
	//check Msaa X4 is supported
	pDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &mMSAAQuality);
	if (mMSAAQuality > 0)
	{
		mEnableMSAA = true;
		MsaaCount(4);

	}
	pDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 8, &mMSAAQuality);
	if (mMSAAQuality > 0)
	{
		mEnableMSAA = true;
		MsaaCount(8);
	}

	//create factory to get the data about Display device
	IDXGIDevice* dxgiDevice = 0;
	pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);

	IDXGIAdapter* dxgiAdapter = 0;
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);

	IDXGIFactory* dxgiFactory = 0;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

	// Enumerate the primary adapter output (monitor).
	IDXGIAdapter* padapter = 0;
	result = dxgiFactory->EnumAdapters(0, &padapter);
	IDXGIOutput* padapter_output = 0;
	result = padapter->EnumOutputs(0, &padapter_output);
	unsigned int num_modes = 0;
	padapter_output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &num_modes, NULL);//DXGI_FORMAT_R8G8B8A8_UNORM_SRGB
	DXGI_MODE_DESC* pdisplay_mode_list;
	pdisplay_mode_list = new DXGI_MODE_DESC[num_modes];
	result = padapter_output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &num_modes, pdisplay_mode_list);//DXGI_FORMAT_R8G8B8A8_UNORM_SRGB


	//check monitor is supported
	for (size_t i = 0; i < num_modes; i++)
	{
	/*	std::cout << "W " << pdisplay_mode_list[i].Width << " H " << pdisplay_mode_list[i].Height << std::endl;
		std::cout << "N " << pdisplay_mode_list[i].RefreshRate.Numerator << "De " << pdisplay_mode_list[i].RefreshRate.Denominator << std::endl << std::endl;*/

		if (pdisplay_mode_list[i].Width >= (unsigned int)mScreenWidth)
		{
			if (pdisplay_mode_list[i].Height >= (unsigned int)mScreenHeight)
			{
				if (pdisplay_mode_list[i].RefreshRate.Numerator > 30 && pdisplay_mode_list[i].RefreshRate.Numerator < 120)
				{
					mNumerator = pdisplay_mode_list[i].RefreshRate.Numerator;
					mDenominator = pdisplay_mode_list[i].RefreshRate.Denominator;
					break;
				}
			}
		}
	}

	//get the data about VideoCard
	DXGI_ADAPTER_DESC adapter_desc;
	result = padapter->GetDesc(&adapter_desc);
	mVideoCardMemory = (int)(adapter_desc.DedicatedVideoMemory / 1024 / 1024);
	size_t stringLength;
	wcstombs_s(&stringLength, LVideoCardDescription, 128, adapter_desc.Description, 128);
	delete[] pdisplay_mode_list;
	pdisplay_mode_list = 0;
	RELEASE_COM(padapter_output);
	RELEASE_COM(padapter);
	RELEASE_COM(dxgiFactory);

	CreateSwapChain(rHWND);
	
	//create normal depth mode
	D3D11_DEPTH_STENCIL_DESC depth_stencil_desc;
	ZeroMemory(&depth_stencil_desc, sizeof(depth_stencil_desc));

	// Set up the description of the stencil state.
	depth_stencil_desc.DepthEnable = true;
	depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS;

	depth_stencil_desc.StencilEnable = true;
	depth_stencil_desc.StencilReadMask = 0xFF;
	depth_stencil_desc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depth_stencil_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depth_stencil_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depth_stencil_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depth_stencil_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	result = pDevice->CreateDepthStencilState(&depth_stencil_desc, &pDepthStencilState);

	//create disable depth mode
	D3D11_DEPTH_STENCIL_DESC depth_disabled_stencil_desc;
	ZeroMemory(&depth_disabled_stencil_desc, sizeof(depth_disabled_stencil_desc));
	depth_disabled_stencil_desc.DepthEnable = false;
	depth_disabled_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depth_disabled_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS;
	depth_disabled_stencil_desc.StencilEnable = true;
	depth_disabled_stencil_desc.StencilReadMask = 0xFF;
	depth_disabled_stencil_desc.StencilWriteMask = 0xFF;
	depth_disabled_stencil_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_disabled_stencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depth_disabled_stencil_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_disabled_stencil_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depth_disabled_stencil_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_disabled_stencil_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depth_disabled_stencil_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_disabled_stencil_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	result = pDevice->CreateDepthStencilState(&depth_disabled_stencil_desc, &pDepthDisabledStencilState);

	EnableZBuffer(true);

	// Initailze the depth stencil view.
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = pDevice->CreateDepthStencilView(pDepthStencilBuffer, &depthStencilViewDesc, &pDepthStencilView);


	//create FILL WIREFRAME mode
	D3D11_RASTERIZER_DESC wire_frame_desc;
	ZeroMemory(&wire_frame_desc, sizeof(D3D11_RASTERIZER_DESC));
	wire_frame_desc.FillMode = D3D11_FILL_WIREFRAME;
	wire_frame_desc.CullMode = D3D11_CULL_BACK;
	wire_frame_desc.FrontCounterClockwise = false;
	wire_frame_desc.DepthClipEnable = true;
	wire_frame_desc.MultisampleEnable = true;
	wire_frame_desc.DepthBias = 0;
	wire_frame_desc.DepthBiasClamp = 0.0f;
	wire_frame_desc.SlopeScaledDepthBias = 0.0f;
	wire_frame_desc.ScissorEnable = false;
	wire_frame_desc.MultisampleEnable = false;
	wire_frame_desc.AntialiasedLineEnable = false;
	pDevice->CreateRasterizerState(&wire_frame_desc, &pWireframe);

	//create normal drawing mode
	D3D11_RASTERIZER_DESC no_cull_desc;
	ZeroMemory(&no_cull_desc, sizeof(D3D11_RASTERIZER_DESC));
	no_cull_desc.FillMode = D3D11_FILL_SOLID;
	no_cull_desc.CullMode = D3D11_CULL_NONE;//D3D11_CULL_BACK//D3D11_CULL_NONE//D3D11_CULL_FRONT
	no_cull_desc.FrontCounterClockwise = false;
	no_cull_desc.DepthClipEnable = true;
	pDevice->CreateRasterizerState(&no_cull_desc, &pNoCull);
	pDeviceContext->RSSetState(pNoCull);

	//create normal drawing mode CULL Back
	ZeroMemory(&no_cull_desc, sizeof(D3D11_RASTERIZER_DESC));
	no_cull_desc.FillMode = D3D11_FILL_SOLID;
	no_cull_desc.CullMode = D3D11_CULL_BACK;//D3D11_CULL_BACK//D3D11_CULL_NONE//D3D11_CULL_FRONT
	no_cull_desc.FrontCounterClockwise = false;
	no_cull_desc.DepthClipEnable = true;
	pDevice->CreateRasterizerState(&no_cull_desc, &pCullBack);

	//create normal drawing mode CULL FRONT
	no_cull_desc.FillMode = D3D11_FILL_SOLID;
	no_cull_desc.CullMode = D3D11_CULL_FRONT;//D3D11_CULL_BACK//D3D11_CULL_NONE//D3D11_CULL_FRONT
	no_cull_desc.FrontCounterClockwise = false;
	no_cull_desc.DepthClipEnable = true;
	pDevice->CreateRasterizerState(&no_cull_desc, &pCullFront);

	D3D11_BLEND_DESC blend_state_desc;
	ZeroMemory(&blend_state_desc, sizeof(D3D11_BLEND_DESC));

	// Create an alpha enabled blend state description.
	blend_state_desc.AlphaToCoverageEnable = false;
	blend_state_desc.IndependentBlendEnable = false;
	blend_state_desc.RenderTarget[0].BlendEnable = true;
	blend_state_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blend_state_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blend_state_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blend_state_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blend_state_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blend_state_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blend_state_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	result = pDevice->CreateBlendState(&blend_state_desc, &pOneOneBlendingState);

	D3D11_BLEND_DESC transparent_desc = { 0 };
	transparent_desc.AlphaToCoverageEnable = false;
	transparent_desc.IndependentBlendEnable = false;

	transparent_desc.RenderTarget[0].BlendEnable = true;
	transparent_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	transparent_desc.RenderTarget[0].DestBlend = D3D11_BLEND_SRC_ALPHA;
	transparent_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	transparent_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	transparent_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	transparent_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	transparent_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	result = pDevice->CreateBlendState(&transparent_desc, &pTransparentBlendingState);

	SetAlphaBlending(NO_BLEND);


	ViewPort(float(mScreenWidth), float(mScreenHeight));

	// Setup the projection matrix.
	mFieldOfView = (float)XM_PI / 4.0f;
	float screen_aspect = (float)mScreenWidth / (float)mScreenHeight;

	// Create the projection matrix for 3D rendering.
	mProjectionMatrix = Matrix::CreatePerspectiveFieldOfView(mFieldOfView, screen_aspect, rScreenNear, rScreenDepth);
	// Initialize the world matrix to the identity matrix.

	// Create an orthographic projection matrix for 2D rendering.
	mOrthoMatrix = Matrix::CreateOrthographic((float)mScreenWidth, (float)mScreenHeight, rScreenNear, rScreenDepth);


	ResizeScreen(mScreenWidth, mScreenHeight);
	return true;
}

void DirectXHandler::Shutdown()
{
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	if (pSwapChain)
	{
		pSwapChain->SetFullscreenState(false, NULL);
	}
	RELEASE_COM(pDepthStencilView);
	RELEASE_COM(pDepthStencilState);
	RELEASE_COM(pDepthDisabledStencilState);
	RELEASE_COM(pDepthStencilBuffer);
	RELEASE_COM(pRenderTargetView);
	RELEASE_COM(pDeviceContext);
	RELEASE_COM(pDevice);
	RELEASE_COM(pSwapChain);
	RELEASE_COM(pOneOneBlendingState);
	RELEASE_COM(pTransparentBlendingState);

	return;
}

void DirectXHandler::BeginScene(float rRed, float rGreen, float rBlue, float rAlpha)
{
	float color[4];


	// Setup the color to clear the buffer to.
	color[0] = rRed;
	color[1] = rGreen;
	color[2] = rBlue;
	color[3] = rAlpha;

	// Clear the back buffer.
	pDeviceContext->ClearRenderTargetView(pRenderTargetView, color);

	// Clear the depth buffer.
	pDeviceContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}

void DirectXHandler::EndScene()
{
	// Present the back buffer to the screen since rendering is complete.
	//if (mVsyncEnabled)
	//{
	//	// Lock to screen refresh rate.
	//	pSwapChain->Present(1, 0);
	//}
	//else
	{
		// Present as fast as possible.
		pSwapChain->Present(0, 0);
	}

	return;
}

void DirectXHandler::SetRasterizer(CULLTYPE rtype)
{
	if (WIREFRAME == rtype)
	{
		pDeviceContext->RSSetState(pWireframe);
	}
	else if (BACK_CULL == rtype)
	{
		pDeviceContext->RSSetState(pCullBack);
	}
	else if (FRONT_CULL == rtype)
	{
		pDeviceContext->RSSetState(pCullFront);
	}
	else if (NO_CULL == rtype)
	{
		pDeviceContext->RSSetState(pNoCull);
	}
	else
	{
		pDeviceContext->RSSetState(0);
	}
}

void DirectXHandler::ResizeScreen(int rScreenWidth, int rScreenHeight)
{
	//return;
	HRESULT result;
	mScreenWidth = rScreenWidth;
	mScreenHeight = rScreenHeight;

	// Release the old views
	RELEASE_COM(pRenderTargetView);
	RELEASE_COM(pDepthStencilView);
	RELEASE_COM(pDepthStencilBuffer);

	// Resize the swap chain and recreate the render target view.

	result = pSwapChain->ResizeBuffers(1, mScreenWidth, mScreenHeight, DXGI_FORMAT_R8G8B8A8_UNORM , 0);//DXGI_FORMAT_R8G8B8A8_UNORM_SRGB
	ID3D11Texture2D* pback_buffer;
	result = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pback_buffer));
	result = pDevice->CreateRenderTargetView(pback_buffer, 0, &pRenderTargetView);
	RELEASE_COM(pback_buffer);

	// Create the depth buffer and view.
	D3D11_TEXTURE2D_DESC depth_stencil_desc;

	depth_stencil_desc.Width = mScreenWidth;
	depth_stencil_desc.Height = mScreenHeight;
	depth_stencil_desc.MipLevels = 1;
	depth_stencil_desc.ArraySize = 1;
	depth_stencil_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// Use MSAA
	if (mEnableMSAA && mMSAACount > 1)
	{
		depth_stencil_desc.SampleDesc.Count = mMSAACount;
		depth_stencil_desc.SampleDesc.Quality = mMSAAQuality - 1;
	}
	else
	{
		depth_stencil_desc.SampleDesc.Count = 1;
		depth_stencil_desc.SampleDesc.Quality = 0;
	}
	depth_stencil_desc.Usage = D3D11_USAGE_DEFAULT;
	depth_stencil_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depth_stencil_desc.CPUAccessFlags = 0;
	depth_stencil_desc.MiscFlags = 0;

	result = pDevice->CreateTexture2D(&depth_stencil_desc, 0, &pDepthStencilBuffer);
	result = pDevice->CreateDepthStencilView(pDepthStencilBuffer, 0, &pDepthStencilView);

	// Bind the render target view and depth/stencil view
	pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);

	ViewPort(float(mScreenWidth), float(mScreenHeight));


	mFieldOfView = (float)XM_PI / 4.0f;
	float screen_aspect = float(mScreenWidth) / float(mScreenHeight);
	mProjectionMatrix = Matrix::CreatePerspectiveFieldOfView(mFieldOfView, screen_aspect, mScreenNear, mScreenDepth);
	mOrthoMatrix = Matrix::CreateOrthographic(float(mScreenWidth), float(mScreenHeight), mScreenNear, mScreenDepth);
}


void DirectXHandler::MsaaCount(UINT rMsaaCount)
{
	mMSAACount = rMsaaCount;
}

UINT DirectXHandler::MsaaCount()
{
	return mMSAACount;
}
/**
 get ViewPort
*/
void DirectXHandler::ViewPort(int& rwidth, int& rheight)
{
	rwidth = mScreenWidth;
	rheight = mScreenHeight;
}
/**
 set ViewPort
*/
void DirectXHandler::ViewPort(float rwidth, float rheight)
{
	mScreenWidth = int(rwidth);
	mScreenHeight = int(rheight);
	mViewPort.TopLeftX = 0;
	mViewPort.TopLeftY = 0;
	mViewPort.Width = static_cast<float>(mScreenWidth);
	mViewPort.Height = static_cast<float>(mScreenHeight);
	mViewPort.MinDepth = 0.0f;
	mViewPort.MaxDepth = 1.0f;
	pDeviceContext->RSSetViewports(1, &mViewPort);
}
/**
 get Device pointer
*/
ID3D11Device * DirectXHandler::GetDevice()
{
	return pDevice;
}
/**
 get Device Context pointer
*/
ID3D11DeviceContext * DirectXHandler::GetDeviceContext()
{
	return pDeviceContext;
}
/**
Reset Render Target and ViewPort
*/
void DirectXHandler::ResetRenderTarget()
{
	//reset to defult Render Target and Depth buffer
	ViewPort(float(mScreenWidth), float(mScreenHeight));
	pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);
}
/**
interactive ZBuffer
*/
void DirectXHandler::EnableZBuffer(bool rEnable)
{
	pDeviceContext->OMSetDepthStencilState((rEnable) ?
		pDepthStencilState : pDepthDisabledStencilState, 1);
	return;
}

/**
set different Alpha Blending
*/
void DirectXHandler::SetAlphaBlending(BLENDTYPE rinput)
{
	float blendFactor[4];
	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;
	if (ONE_ONE_BLEND == rinput)
	{
		pDeviceContext->OMSetBlendState(pOneOneBlendingState, blendFactor, 0xffffffff);
	}
	else if (ALPHA_BLEND == rinput)
	{
		pDeviceContext->OMSetBlendState(pTransparentBlendingState, blendFactor, 0xffffffff);
	}
	else
	{
		pDeviceContext->OMSetBlendState(0, blendFactor, 0xffffffff);
	}
}
/**
Set Depth Stencil View
*/
void DirectXHandler::SetDepthStencilView(ID3D11DepthStencilView * rpDepthStencilView)
{

	pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, rpDepthStencilView);
}
/**
Get Aspect Ratio
*/
float DirectXHandler::GetAspectRatio() const
{
	return static_cast<float>(mScreenWidth) / mScreenHeight;
}
/**
Get Projection Matrix
*/
void DirectXHandler::GetProjectionMatrix(Matrix & rProjectionMatrix)
{
	rProjectionMatrix = mProjectionMatrix;
	return;
}
/**
Get World Matrix
*/
void DirectXHandler::GetWorldMatrix(Matrix & rWorldMatrix)
{
	rWorldMatrix = mWorldMatrix;
	return;
}
/**
Get Ortho Matrix
*/
void DirectXHandler::GetOrthoMatrix(Matrix & rOrthoMatrix)
{
	rOrthoMatrix = mOrthoMatrix;
	return;
}
/**
Get VideoCard Info like name and memory size
*/
void DirectXHandler::GetVideoCardInfo(char* rCardName, int& rMemory)
{
	strcpy_s(rCardName, 128, LVideoCardDescription);
	rMemory = mVideoCardMemory;

	return;
}
void DirectXHandler::Fullscreen(bool b, int rScreenWidth, int rScreenHeight)
{
	mFullScreen = b;
	pSwapChain->SetFullscreenState(b, nullptr);
	ResizeScreen(rScreenWidth, rScreenHeight);
}
/**
Create Swap Chain
*/
void DirectXHandler::CreateSwapChain(HWND rHWND)
{
	DXGI_SWAP_CHAIN_DESC scd;
	scd.BufferDesc.Width = mScreenWidth;
	scd.BufferDesc.Height = mScreenHeight;
	//if (mVsyncEnabled == true)
	//{
	//	scd.BufferDesc.RefreshRate.Numerator = mNumerator;
	//	scd.BufferDesc.RefreshRate.Denominator = mDenominator;
	//}
	//else
	{
		scd.BufferDesc.RefreshRate.Numerator = 0;
		scd.BufferDesc.RefreshRate.Denominator = 0;
	}


	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//DXGI_FORMAT_R8G8B8A8_UNORM_SRGB
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	//// Use 4X MSAA? 
	if (mEnableMSAA)
	{
		scd.SampleDesc.Count = mMSAACount;
		scd.SampleDesc.Quality = mMSAAQuality - 1;
	}
	// No MSAA
	else
	{
		scd.SampleDesc.Count = 1;
		scd.SampleDesc.Quality = 0;
	}

	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 2;
	scd.OutputWindow = rHWND;
	// Set to full screen or windowed mode.
	if (mFullScreen)
	{
		scd.Windowed = false;
	}
	else
	{
		scd.Windowed = true;
	}
	
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;


	IDXGIDevice* dxgiDevice = 0;
	pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);

	IDXGIAdapter* dxgiAdapter = 0;
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);

	IDXGIFactory* dxgiFactory = 0;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
	dxgiFactory->CreateSwapChain(pDevice, &scd, &pSwapChain);

	RELEASE_COM(dxgiDevice);
	RELEASE_COM(dxgiAdapter);
	RELEASE_COM(dxgiFactory);
}
