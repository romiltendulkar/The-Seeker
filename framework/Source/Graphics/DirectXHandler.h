/**
 * @file DirectXHandler.h
 * @author Ching-Yen Lin
 * @date 9/01/19
 * @brief This file content DirectXhandler member setting
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#ifndef DIRECTXHANDLER_H
#define DIRECTXHANDLER_H
enum CULLTYPE
{
	WIREFRAME = 0,
	BACK_CULL,
	FRONT_CULL,
	NO_CULL,
	OTHER
};
enum BLENDTYPE
{
	ONE_ONE_BLEND = 0,
	ALPHA_BLEND,
	NO_BLEND
};
#include "../pch.h"
class DirectXHandler
{
public:

	DirectXHandler();
	DirectXHandler(const DirectXHandler& rDirectXHandler) {}
	~DirectXHandler();

	/**
	 Initialize DirectX 11 with window
	*/
	bool Initialize(int, int, bool, HWND, bool, float, float);

	/**
	clean the pointer
	*/
	void Shutdown();

	/**
	 clean the main render target and depth for preparing draw
	*/
	void BeginScene(float, float, float, float);

	/**
	 call swap chain to present scence
	*/
	void EndScene();

	/**
	set Rasterizer like wire draw, cull
	*/
	void SetRasterizer(CULLTYPE);

	/**
	Resize Screen
	*/
	void ResizeScreen(int, int);

	/**
	to interactive the MSAA like  x0 x4 x8
	*/
	void MsaaCount(UINT);

	/**
	get the MSAA like  x0 x4 x8
	*/
	UINT MsaaCount();

	/**
	 get ViewPort
	*/
	void ViewPort(int&, int&);

	/**
	 set ViewPort
	*/
	void ViewPort(float, float);

	/**
	 get Device pointer
	*/
	ID3D11Device* GetDevice();

	/**
	 get Device Context pointer
	*/
	ID3D11DeviceContext* GetDeviceContext();

	/**
	Reset Render Target and ViewPort
	*/
	void ResetRenderTarget();

	/**
	interactive ZBuffer
	*/
	void EnableZBuffer(bool);

	/**
	set different Alpha Blending
	*/
	void SetAlphaBlending(BLENDTYPE);

	/**
	Set Depth Stencil View
	*/
	void SetDepthStencilView(ID3D11DepthStencilView*);

	/**
	Get Aspect Ratio
	*/
	float GetAspectRatio() const;

	/**
	Get Projection Matrix
	*/
	void GetProjectionMatrix(Matrix&);

	/**
	Get World Matrix
	*/
	void GetWorldMatrix(Matrix&);

	/**
	Get Ortho Matrix
	*/
	void GetOrthoMatrix(Matrix&);

	/**
	Get VideoCard Info like name and memory size
	*/
	void GetVideoCardInfo(char*, int&);
	void Fullscreen(bool b,int rScreenWidth, int rScreenHeight);
private:

	/**
	Create Swap Chain
	*/
	void CreateSwapChain(HWND rHWND);
	bool mFullScreen;
	int mScreenWidth;
	int mScreenHeight;
	float mFieldOfView;
	float mScreenDepth;
	float mScreenNear;
	UINT mMSAACount;
	UINT mMSAAQuality;
	bool mEnableMSAA;
	bool mVsyncEnabled;
	int mVideoCardMemory;
	char LVideoCardDescription[128];
	unsigned int mNumerator;
	unsigned int mDenominator;
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pDeviceContext;
	IDXGISwapChain* pSwapChain;

	//main render target
	ID3D11RenderTargetView* pRenderTargetView;

	//Zbuffer mode
	ID3D11DepthStencilState* pDepthStencilState;
	ID3D11DepthStencilState* pDepthDisabledStencilState;
	//depth buff
	ID3D11DepthStencilView* pDepthStencilView;
	ID3D11Texture2D* pDepthStencilBuffer;

	//Cull Back
	ID3D11RasterizerState* pWireframe;
	ID3D11RasterizerState* pCullBack;
	ID3D11RasterizerState* pCullFront;
	ID3D11RasterizerState* pNoCull;
	//alpha setting
	ID3D11BlendState* pOneOneBlendingState;
	ID3D11BlendState* pTransparentBlendingState;

	//view port
	D3D11_VIEWPORT mViewPort;
	Matrix mProjectionMatrix;
	Matrix mWorldMatrix;
	Matrix mOrthoMatrix;
};

#endif

