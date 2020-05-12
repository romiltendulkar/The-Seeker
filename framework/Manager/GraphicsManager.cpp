/**
 * @file GraphicsManager.h
 * @authors Ching-Yen Lin.
 * @date 9/1/2019
 * @brief This file has methods for rendering data on the screen
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#include "../Source/pch.h"
#include "GraphicsManager.h"
#include "ImGUIManager.h"
#include "../Source/Graphics/Shape.h"
#include "../Source/Components/Asset.h"
#include "../Source/Components/Transform.h"
#include "../Source/GameObject.h"
#include "../Manager/MemoryManager.h"
#include "../Manager/ImGUIManager.h"
#include "../Source/Components/Animation.h"
#include "../Source/Components/Body.h"
#include "../Manager/DXInputManager.h"
#include "../Source/Entity/DirectLight.h"
#include "../Source/Entity/PointLight.h"
#include "../Source/Entity/SpotLight.h"
#include "../Manager/UIManager.h"
#include "../Manager/ParticleManager.h"
#include "../Source/Entity/Player.h"



GraphicsManager::GraphicsManager()
{
	pDirectX = 0;
	pSimplePShader = 0;
	pSimpleVShader = 0;
	pAnimedVShader = 0;
	pCamera = 0;
	pObject = 0;
	mScreenWidth = 0;
	mScreenHeight = 0;
	mDebugMode = false;

}
GraphicsManager::~GraphicsManager()
{
	pWorldMatrixBuff->Release();
	pObjectBuff->Release();
	pModelBuff->Release();
}
void GraphicsManager::Fullscreen(bool b)
{
	FULL_SCREEN= b;
	if (pDirectX)
	{
		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);
		if (b)
			pDirectX->Fullscreen(b, screenWidth, screenHeight);
		else
			pDirectX->Fullscreen(b, mScreenWidth, mScreenHeight);
	}
}
bool GraphicsManager::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	mHWND = hwnd;
	bool result;
	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;
	// Create the Direct3D object.
	pDirectX = new DirectXHandler();
	if (!pDirectX)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = pDirectX->Initialize(screenWidth, screenHeight,
		VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_FAR, SCREEN_NEAR);
	if (!result)
	{
		//MessageBox(hwnd, "Could not initialize Direct3D", "Error", MB_OK);
		return false;
	}

	pBrush = new PrimitiveBatch<VertexPositionColor>(pDirectX->GetDeviceContext());

	InitializeShader();

	mDeferredRendering.Initialize(mScreenWidth, mScreenHeight);

	pCamera = new Camera();
	pCamera->GetViewMatrix(mDefaultView);
	pCamera->SetPosition(0, -10, 1);

	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		cout << "ERROR::FREETYPE: Could not init FreeType Library" << endl;
	}
	FT_Face face;
	auto error = FT_New_Face(ft, "./Resource/Font/""AnonymousPro-Bold.ttf", 0, &face);//swapgoats
	FT_Set_Pixel_Sizes(face, 0, 42);
	for (int i = 0; i < 128; i++)
	{
		if (FT_Load_Char(face, wchar_t(i), FT_LOAD_RENDER))
			continue;

		DXTexture* charImage = new DXTexture();
		charImage->Initialize(pDirectX->GetDevice(),
			pDirectX->GetDeviceContext(),
			&face->glyph->bitmap);

		Character character = {
		charImage,
		XMINT2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
		XMINT2(face->glyph->bitmap_left, face->glyph->bitmap_top),
		size_t(face->glyph->advance.x)
		};
		Characters[wchar_t(i)] = character;
	}
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
	/*OBJ* cube = new OBJ("./Resource/Model/Skeleton.obj");*/
	//OBJ* cube = new OBJ("./Resource/Model/cube.obj");
	//Box* q = new Box(VEC3(1,1,1), VEC3(-1,-1,-1));
	//Particles* particles = new Particles();
	//po = new DXObject(particles, mDirectX->GetDevice(), mDirectX->GetDeviceContext(),true);
	Quad * quad = new Quad();
	Disk * disk = new Disk(32);
	Sphere * sphere = new Sphere(32);
	FrontQ = new Quad();
	pQuad = new DXObject(quad, pDirectX->GetDevice(), pDirectX->GetDeviceContext());
	//pTestFront = new DXObject(FrontQ, pDirectX->GetDevice(), pDirectX->GetDeviceContext(), true);
	
	//pTestFront->SetTexture(DIFFUSE_TEXTURE, Characters[wchar_t('@')].Texture);
	//pTestFront->SetMaterialID(4);
	//pDisk = new DXObject(quad, pDirectX->GetDevice(), pDirectX->GetDeviceContext(), Matrix::CreateScale(100, 100, 1));
	//pDisk->SetMaterialID(1);
	pTexture = new DXTexture();
	pBlurTexture = new DXTexture();
	pBlurTexture->Initialize(pDirectX->GetDevice(), pDirectX->GetDeviceContext());
	pTexture->Initialize(pDirectX->GetDevice(), pDirectX->GetDeviceContext(), wstring(L"./Resource/Image/sky.jpg").c_str());
	//pQuad->SetTexture(DIFFUSE_TEXTURE, pTexture);

	

	InitializeBlur(8);
	SetBlurScreen();
	//mTexture->Use(NORMAL_TEXTURE_SLOT_BEGIN);
	pSkyDome = new DXObject(sphere, pDirectX->GetDevice(), pDirectX->GetDeviceContext(), Matrix::CreateScale(5000, 5000, 5000));
	pSkyDome->SetMaterialID(1);
	pSkyDome->SetTexture(DIFFUSE_TEXTURE, pTexture);
	pPoint = new DXObject(sphere, pDirectX->GetDevice(), pDirectX->GetDeviceContext(), Matrix::CreateScale(0.5, 0.5, 0.5));

	InitializeDefaultLight();
	return true;
}

void GraphicsManager::Shutdown()
{
	if (pDirectX)
	{
		pDirectX->Shutdown();
		delete pDirectX;
		pDirectX = 0;
	}
	if (pBrush)
	{
		delete pBrush;
		pBrush = 0;
	}
	if (pObject)
	{
		pObject->~DXObject();
	}
	if (pCamera)
	{
		pCamera->~Camera();
	}
	if (pSimplePShader)
	{
		pSimplePShader->~DXShader();
	}
	if (pSimpleVShader)
	{
		pSimpleVShader->~DXShader();
	}
	if (pAnimedVShader)
	{
		pAnimedVShader->~DXShader();
	}
	if (pShadowPShader)
	{
		pShadowPShader->~DXShader();
	}
	RELEASE_COM(pDefaultSamplerState);
	return;
}

bool GraphicsManager::Update(float rdeltaTime)
{
	dt = rdeltaTime;
	Render();
	return true;
}

void GraphicsManager::Resize(int screenWidth, int screenHeight)
{
	if (!FULL_SCREEN)
	{
		mScreenWidth = screenWidth;
		mScreenHeight = screenHeight;
		mDeferredRendering.Resize(screenWidth, screenHeight);
		//pDirectX->ResizeScreen(screenWidth, screenHeight);
		SetBlurScreen();
	}
}
void GraphicsManager::Getsize(int& screenWidth, int& screenHeight)
{
	screenWidth = mScreenWidth;
	screenHeight = mScreenHeight;
}

void GraphicsManager::SetDebug(bool b)
{
	mDebugMode = b;
}

ID3D11Buffer* GraphicsManager::MakeMatrixBuffer(D3D11_BUFFER_DESC matrixBuffer)
{
	HRESULT result;
	ID3D11Buffer* buff;

	result = pDirectX->GetDevice()->CreateBuffer(&matrixBuffer, NULL, &buff);

	if (FAILED(result))
	{
		return nullptr;
	}
	return buff;
}

//bind matrix buffer by using map
void GraphicsManager::BindWorldMatrixBuffer(Matrix view, Matrix proj)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	WorldMatrixBuffer* dataPtr;
	result = pDirectX->GetDeviceContext()->Map(pWorldMatrixBuff
		, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))return;

	dataPtr = (WorldMatrixBuffer*)mappedResource.pData;
	dataPtr->WorldView = view.Transpose();
	dataPtr->WorldProj = proj.Transpose();
	dataPtr->WorldInverse = view.Transpose().Invert();
	pDirectX->GetDeviceContext()->Unmap(pWorldMatrixBuff, 0);
	pDirectX->GetDeviceContext()
		->VSSetConstantBuffers(0, 1, &(pWorldMatrixBuff));
	pDirectX->GetDeviceContext()
		->GSSetConstantBuffers(0, 1, &(pWorldMatrixBuff));
}

void GraphicsManager::BindLightBuffer(Light* light)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	LightInfo* dataPtr;
	result = pDirectX->GetDeviceContext()->Map(pLightBuff
		, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))return;

	dataPtr = (LightInfo*)mappedResource.pData;
	*dataPtr = light->GetInfo();
	pDirectX->GetDeviceContext()->Unmap(pLightBuff, 0);
	pDirectX->GetDeviceContext()
		->PSSetConstantBuffers(3, 1, &(pLightBuff));
}
void GraphicsManager::BindLightPVBuffer(LightInform lightinform)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	LightInform* dataPtr;
	result = pDirectX->GetDeviceContext()->Map(pLightPVBuff
		, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))return;

	dataPtr = (LightInform*)mappedResource.pData;
	*dataPtr = lightinform;
	pDirectX->GetDeviceContext()->Unmap(pLightPVBuff, 0);
	pDirectX->GetDeviceContext()
		->PSSetConstantBuffers(5, 1, &(pLightPVBuff));
	pDirectX->GetDeviceContext()
		->GSSetConstantBuffers(5, 1, &(pLightPVBuff));
}

string GraphicsManager::GetDeviceName()
{

	char temp[128];
	int mem;
	pDirectX->GetVideoCardInfo(temp, mem);
	return	string(temp);
}

DirectXHandler * GraphicsManager::GetDirectXHandler()
{
	return pDirectX;
}

GraphicsManager * GraphicsManager::GetInstance()
{
	if (!mInstance)
	{
		mInstance = new GraphicsManager();
	}
	return mInstance;
}

void GraphicsManager::Destroy()
{
	delete mInstance;
}

void GraphicsManager::SetCameraRotation(float x, float y, float z, float sensitivity)
{
	if (pCamera)
	{
		float pan, tile, roll;
		pCamera->GetPanTiltRoll(pan, tile, roll);
		pCamera->SetPan(pan - x / sensitivity);
		pCamera->SetTilt(tile - y / sensitivity);
		pCamera->SetRoll(roll - z / sensitivity);
	}
}

//NEED TODO: make it better like actual camera movemont
void GraphicsManager::SetCameraMovement(float x, float y, float z, float sensitivity)
{
	if (pCamera)
	{
		pCamera->SetMovement(x / sensitivity, y / sensitivity, z / sensitivity);
	}
}

void GraphicsManager::AddDrawingLine(Vector4 a, Vector4 b, Vector3 color, bool ZBuffer)
{
	LLineQueue.push({ Vector4(a.x,a.y,a.z,1),
		Vector4(b.x,b.y,b.z,1),
		Vector3(color.x,color.y,color.z),ZBuffer });

}

void GraphicsManager::AddDrawingPoint(Vector4 a, Vector3 color, bool ZBuffer)
{
	LPointQueue.push({ Vector4(a.x,a.y,a.z,1),
		Vector3(color.x,color.y,color.z),ZBuffer });
}

void GraphicsManager::SetCameraDistance(float rdis)
{
	if (pCamera)
	{
		pCamera->mZoom += rdis;
		//std::cout << "Z " << mCamera->mZoom << std::endl;
		pCamera->SetMovement(0, 0, 0);
	}
}

void GraphicsManager::SetCameraShack(float num, float Magnitude)
{
	if(pCamera)
		pCamera->CameraShackFrame(num, Magnitude);
}

void GraphicsManager::GetCameraPosition(Vector3 & rcamerapos)
{
	float x, y, z;

	pCamera->GetPosition(x, y, z);
	rcamerapos = Vector3(x, y, z);
}

Camera* GraphicsManager::GetCamera()
{
	return pCamera;
}

void GraphicsManager::SetBlurScreen()
{
	D3D11_TEXTURE2D_DESC textDESC;
	ZeroMemory(&textDESC, sizeof(textDESC));
	textDESC.Width = mScreenWidth;
	textDESC.Height = mScreenHeight;
	textDESC.MipLevels = 1;
	textDESC.ArraySize = 1;
	textDESC.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textDESC.SampleDesc.Count = 1;
	textDESC.SampleDesc.Quality = 0;
	textDESC.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textDESC.CPUAccessFlags = 0;
	textDESC.MiscFlags = 0;

	ID3D11Texture2D* texture = nullptr;
	pDirectX->GetDevice()->CreateTexture2D(&textDESC, 0, &texture);

	ID3D11ShaderResourceView*	pSRV;
	D3D11_SHADER_RESOURCE_VIEW_DESC resDESC;
	ZeroMemory(&resDESC, sizeof(resDESC));
	resDESC.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	resDESC.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	resDESC.Texture2D.MostDetailedMip = 0;
	resDESC.Texture2D.MipLevels = 1;
	pDirectX->GetDevice()->CreateShaderResourceView(texture, &resDESC, &pSRV);

	if(pBlurTexture&&pSRV)
		std::swap(pBlurTexture->pTexture, pSRV);


	RELEASE_COM(pSRV);
	RELEASE_COM(texture);

}

void GraphicsManager::Render()
{
	//Initial graphics stuff, removed from loop because why put it in a loop to slow down stuff

	// Clear the buffers to begin the scene.
	pDirectX->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);
	unsigned int key1 = MemoryManager::GetInstance()->current_position;

	auto maincharacher=MemoryManager::GetInstance()->maincharacher;
	if (maincharacher > -1)
	{
		auto temp = (*MemoryManager::GetInstance()->ObjectPool)[maincharacher];
		if (temp.pEn->mEntityName == "Player")
		{
			auto x = temp.pTr->mPositionX;
			auto y = temp.pTr->mPositionY;
			auto z = temp.pTr->mPositionZ;

			if (MemoryManager::GetInstance()->mpOwner->mCurrentLevel == "MenuLevel")
			{
				pCamera->SetPosition(x, y, z);
				pCamera->mTx = -x+5;
				pCamera->mTy = -y+10;
			}
			else
			{
				pCamera->SetPosition(x, y, z);
				pCamera->mTx = -x;
				pCamera->mTy = -y;
				if (pCamera->isShack)
				{
					//auto shack=pCamera->GetCameraShackValue(dt);
					auto shack = pCamera->GetCameraShackFrameValue(1);
					pCamera->mTx += shack.x;
					pCamera->mTy += shack.y;
				}
			}
		}
	}

	//set the camera
	Matrix WorldProj, light_view, light_pro, ThPersonView, FiPersonView, OrthProj;
	Matrix Identity;
	OrthProj = Matrix::CreateOrthographic(mScreenWidth, mScreenHeight, SCREEN_NEAR, SCREEN_FAR).Transpose();
	pCamera->GetViewMatrix(FiPersonView);
	pCamera->Get3rdPersonView(ThPersonView);
	//pDirectX->GetOrthoMatrix(OrthProj);
	WorldView = ThPersonView;

	light_view = pLight->GetLightViewMatrix();
	light_pro = pLight->GetLightProjectionMatrix();
	WorldProj = MyPerspective((0.4f*mScreenWidth) / mScreenHeight, 0.4f, SCREEN_NEAR, SCREEN_FAR);//
	BindWorldMatrixBuffer(WorldView, WorldProj);
	//BindWorldMatrixBuffer(light_view, light_pro);
	//use Render target for Deferred Render
	mDeferredRendering.Use(0.5f, 0.5f, 0.5f, 1.0f);
	//remember open mDepth testing
	pDirectX->EnableZBuffer(true);
	//remember close Alpha Blending until there is Transparent object
	pDirectX->SetAlphaBlending(NO_BLEND);
	//use GBuffer pixel shader for drawing in Render target
	pGbufferPShader->Use();
	pSimpleVShader->Use();

	unsigned int key = MemoryManager::GetInstance()->current_position;

	for (unsigned int i = 0; i < key; i++)
	{
		Asset* pAs = (*MemoryManager::GetInstance()->ObjectPool)[i].pAs;
		Transform* pTr = (*MemoryManager::GetInstance()->ObjectPool)[i].pTr;
		Animation* pAn = (*MemoryManager::GetInstance()->ObjectPool)[i].pAn;
		Body* pBd = (*MemoryManager::GetInstance()->ObjectPool)[i].pBd;
		if (pAs && pTr)
		{
			//use the shader
			pSimpleVShader->Use();
			if (pAn)
			{
				pAnimedVShader->Use();
				FBX* tempFBX = dynamic_cast<FBX*>(pAs->obj->pShape);
				/*tempFBX->Update(pAn->GetTotalTime(), pAn->GetCurrentStatus(), pAn->IsLoop(), pAn->GetSpeed());*/

				if (pAn->GetAnimationQueue().size() > 1)
				{
					tempFBX->Blending(pAn->GetAnimationQueue().front().AnimationName, pAn->GetAnimationQueue().back().AnimationName,
						pAn->GetBlendingValue(), pAn->GetTotalTime(), pAn->GetSpeed());
				}
				else
				{
					tempFBX->Update(pAn->GetTotalTime(), pAn->GetCurrentStatus(), pAn->IsLoop(), pAn->GetSpeed());
				}

				if (mDebugMode)
				{
					DrawBones(pAs);
				}
			}
			if (pBd&&mDebugMode)
			{
				DrawBoundingBox(pBd,Vector3(Colors::Red));
			}
			pAs->obj->SetObjectTr(pTr);//set position
			pAs->obj->Draw(pObjectBuff, pModelBuff, pBoneBuff);
			mDeferredRendering.Unuse();

			pShadowPShader->Use();
			auto& lightL = MemoryManager::GetInstance()->lightList;
			for (int i = 0; i < lightL.size(); i++)
			{
				auto index = lightL[i];
				auto lightE = (*MemoryManager::GetInstance()->ObjectPool)[index].pEn;
				Light* pLt=nullptr;
				if (lightE->GetEntityType() == ENTITY_TYPE::SPOT_LIGHT)
					pLt = static_cast<SpotLight*>((*MemoryManager::GetInstance()->ObjectPool)[index].pEn)->pLd->pLt;
				if (lightE->GetEntityType() == ENTITY_TYPE::POINT_LIGHT)
					pLt = static_cast<PointLight*>((*MemoryManager::GetInstance()->ObjectPool)[index].pEn)->pLd->pLt;
				if (lightE->GetEntityType() == ENTITY_TYPE::DIRECT_LIGHT)
				{
					pLt = static_cast<DirectLight*>((*MemoryManager::GetInstance()->ObjectPool)[index].pEn)->pLd->pLt;
					if (maincharacher > -1)
					{
						pLt->SetPosition((*MemoryManager::GetInstance()->ObjectPool)[maincharacher].pTr->mPositionX,
							(*MemoryManager::GetInstance()->ObjectPool)[maincharacher].pTr->mPositionY,
							(*MemoryManager::GetInstance()->ObjectPool)[maincharacher].pTr->mPositionZ);
					}
					pLt->ChangeSize(GetCamera()->mZoom*0.23f);
				}
				if (!pLt->HasShadow()) continue;
				pLt->ShadowMap().ContinousDrawShadow();
				light_view = pLt->GetLightViewMatrix();
				light_pro = pLt->GetLightProjectionMatrix();
				BindWorldMatrixBuffer(light_view, light_pro);
				BindLightBuffer(pLt);
				pDirectX->SetRasterizer(FRONT_CULL);
				pAs->obj->Draw(pObjectBuff, pModelBuff, pBoneBuff);
				pDirectX->SetRasterizer(NO_CULL);
				pLt->ShadowMap().FinishDrawShadow();
			}
			/*pLightGShader->Unuse();*/
			mDeferredRendering.Reuse();
			pGbufferPShader->Use();
			BindWorldMatrixBuffer(WorldView, WorldProj);
			//BindWorldMatrixBuffer(light_view, light_pro);

		}
	}
	mDeferredRendering.Reuse();
	//mDeferredRendering.Unuse();
	pGbufferPShader->Use();
	ParticleManager::GetInstance()->Draw();

	pSimpleVShader->Use();

	mDeferredRendering.Reuse();
	//pDisk->Draw(pObjectBuff, pModelBuff, pBoneBuff);
	pSkyDome->Draw(pObjectBuff, pModelBuff, pBoneBuff);

	//pTestTexture->pTexture = ParticleManager::GetInstance()->pParticleBufferSRV;
	//pDisk->SetTexture(DIFFUSE_TEXTURE, pTestTexture);
	//pDisk->Draw(pObjectBuff, pModelBuff, pBoneBuff);

	pDirectX->EnableZBuffer(false);
	BindWorldMatrixBuffer(mDefaultView, OrthProj);
	UIManager::GetInstance()->Draw();


	pDirectX->SetAlphaBlending(NO_BLEND);
	BindWorldMatrixBuffer(WorldView, WorldProj);
	//BindWorldMatrixBuffer(light_view, light_pro);
	if (mDebugMode)
	{
		DrawDebug();
		DrawMouse();

		DrawPoint();
		DrawLine();
	}
	if (PhysicsManager::GetInstance()->rayresult != -1)
	{
		Body* pBd = (*MemoryManager::GetInstance()->ObjectPool)[PhysicsManager::GetInstance()->rayresult].pBd;
		ImGUIManager::GetInstance()->selectedkey = pBd->key;
		/*DrawBoundingBox(pBd,Vector3(Colors::Green));*/
		//cout << PhysicsManager::GetInstance()->ppoint.x << " " << PhysicsManager::GetInstance()->ppoint.y << " "
		//	<< PhysicsManager::GetInstance()->ppoint.z << "\n";
	}

	//unuse Deferred Rendering
	mDeferredRendering.Unuse();
	
	Blurring(mScreenWidth,mScreenHeight, *(mDeferredRendering.getParticleLevel()),&(pBlurTexture->pTexture));

	//use GBuffer data
	mDeferredRendering.BindGBuffer(GBUFFER_TEXTURE_SLOT_BEGIN);

	//use vertex for Deferred Rendering
	//only draw quad
	BindWorldMatrixBuffer(WorldView, WorldProj);
	//BindWorldMatrixBuffer(light_view, light_pro);
	pDeferredVShader->Use();

	//draw Ambient pass for Deferred Rendering
	pDSAPShader->Use();
	pDirectX->SetAlphaBlending(NO_BLEND);
	//pDirectX->SetAlphaBlending(ALPHA_BLEND);
	//pSimplePShader->Use();

	pQuad->Draw(pObjectBuff, pModelBuff, pBoneBuff);

	//use the light
	//should loop through light list
	auto& lightL = MemoryManager::GetInstance()->lightList;
	for (int i = 0; i < lightL.size(); ++i)
	{
		auto index = lightL[i];
		auto lightE = static_cast<SpotLight*>((*MemoryManager::GetInstance()->ObjectPool)[index].pEn);
		Light* pLt = nullptr;
		if (lightE->GetEntityType() == ENTITY_TYPE::SPOT_LIGHT)
			pLt = static_cast<SpotLight*>((*MemoryManager::GetInstance()->ObjectPool)[index].pEn)->pLd->pLt;
		if (lightE->GetEntityType() == ENTITY_TYPE::POINT_LIGHT)
			pLt = static_cast<PointLight*>((*MemoryManager::GetInstance()->ObjectPool)[index].pEn)->pLd->pLt;
		if (lightE->GetEntityType() == ENTITY_TYPE::DIRECT_LIGHT)
			pLt = static_cast<DirectLight*>((*MemoryManager::GetInstance()->ObjectPool)[index].pEn)->pLd->pLt;
		BindLightBuffer(pLt);

		if (pLt->HasShadow())
		{
			pLt->ShadowMap().BindShadowMap(SHADOW_TEXTURE_SLOT_BEGIN);
		}
		////draw Light pass for Deferred Rendering
		pDSLPShader->Use();
		pDirectX->EnableZBuffer(false);
		pDirectX->SetAlphaBlending(ONE_ONE_BLEND);
		pDirectX->SetRasterizer(BACK_CULL);
		pQuad->Draw(pObjectBuff, pModelBuff, pBoneBuff);
		pBlurTexture->Bind(9);
		pDirectX->SetRasterizer(NO_CULL);
		if (pLt->HasShadow())
		{
			pLt->ShadowMap().UnBindShadowMap(SHADOW_TEXTURE_SLOT_BEGIN);
			pLt->ShadowMap().StartDrawShadow();
			pLt->ShadowMap().FinishDrawShadow();
		}

	}
	mDeferredRendering.UnBindGBuffer(GBUFFER_TEXTURE_SLOT_BEGIN);

	if (mDebugMode)
	{
		ImGUIManager::GetInstance()->DrawWindows();
	}
	if (mPaused)
	{
		ImGUIManager::GetInstance()->DrawCharStats();
	}
	if (mCompletion)
	{
		ImGUIManager::GetInstance()->DrawCompletion();
	}
	if (mNeedCompletion)
	{
		ImGUIManager::GetInstance()->NeedCompletion();
	}
	mNeedCompletion = false;
	//render on the screen
	//should not draw anything after this
	ImGUIManager::GetInstance()->DrawWindows();
	queue<Point> empty;
	swap(LPointQueue, empty);

	queue<Line> empty1;
	swap(LLineQueue, empty1);
	pDirectX->EndScene();

}
