#include "../pch.h"
#include "../../Manager/GraphicsManager.h"
#include "../../Manager/GraphicsManager.h"
#include "../../Manager/ImGUIManager.h"
#include "../../Source/Graphics/Shape.h"
#include "../../Source/Components/Asset.h"
#include "../../Source/Components/Transform.h"
#include "../../Source/GameObject.h"
#include "../../Manager/MemoryManager.h"
#include "../../Manager/ImGUIManager.h"
#include "../../Source/Components/Animation.h"
#include "../../Source/Components/Body.h"
#include "../../Manager/DXInputManager.h"

#include "../../Source/Entity/DirectLight.h"
#include "../../Source/Entity/PointLight.h"
#include "../../Source/Entity/SpotLight.h"
bool GraphicsManager::InitializeShader()
{
	bool result;
	pSimpleVShader = new DXShader(pDirectX->GetDevice(), pDirectX->GetDeviceContext());
	pSimplePShader = new DXShader(pDirectX->GetDevice(), pDirectX->GetDeviceContext());
	pAnimedVShader = new DXShader(pDirectX->GetDevice(), pDirectX->GetDeviceContext());
	pShadowPShader = new DXShader(pDirectX->GetDevice(), pDirectX->GetDeviceContext());

	pParticleVShader = new DXShader(pDirectX->GetDevice(), pDirectX->GetDeviceContext());
	pParticleGShader = new DXShader(pDirectX->GetDevice(), pDirectX->GetDeviceContext());
	pParticleCShader = new DXShader(pDirectX->GetDevice(), pDirectX->GetDeviceContext());
	pParticleEmitCShader = new DXShader(pDirectX->GetDevice(), pDirectX->GetDeviceContext());

	pBlurHCShader = new DXShader(pDirectX->GetDevice(), pDirectX->GetDeviceContext());
	pBlurVCShader = new DXShader(pDirectX->GetDevice(), pDirectX->GetDeviceContext());

	pLightGShader = new DXShader(pDirectX->GetDevice(), pDirectX->GetDeviceContext());

	pDeferredVShader = new DXShader(pDirectX->GetDevice(), pDirectX->GetDeviceContext());
	pGbufferPShader = new DXShader(pDirectX->GetDevice(), pDirectX->GetDeviceContext());
	pDSAPShader = new DXShader(pDirectX->GetDevice(), pDirectX->GetDeviceContext());
	pDSLPShader = new DXShader(pDirectX->GetDevice(), pDirectX->GetDeviceContext());

	pAnimedVShader->AddShader(StringtoWstring("AnimationVShader.cso").c_str(), VS, &mHWND);
	pSimpleVShader->AddShader(StringtoWstring("SimpleVShader.cso").c_str(), VS, &mHWND);
	pSimplePShader->AddShader(StringtoWstring("SimplePShader.cso").c_str(), PS, &mHWND);
	pShadowPShader->AddShader(StringtoWstring("ShadowPShader.cso").c_str(), PS, &mHWND);

	pParticleGShader->AddShader(StringtoWstring("ParticleGeometry.cso").c_str(), GS, &mHWND);
	pParticleCShader->AddShader(StringtoWstring("ParticleCompute.cso").c_str(), CS, &mHWND);
	pParticleEmitCShader->AddShader(StringtoWstring("ParticleComputeEmit.cso").c_str(), CS, &mHWND);
	pParticleVShader->AddShader(StringtoWstring("ParticleVShader.cso").c_str(), VS, &mHWND);

	pLightGShader->AddShader(StringtoWstring("LightGeometry.cso").c_str(), GS, &mHWND);

	pDeferredVShader->AddShader(StringtoWstring("DeferredVertexShader.cso").c_str(), VS, &mHWND);
	pGbufferPShader->AddShader(StringtoWstring("DefferedGbufferPShader.cso").c_str(), PS, &mHWND);
	pDSAPShader->AddShader(StringtoWstring("DeferredAmbientPShader.cso").c_str(), PS, &mHWND);
	pDSLPShader->AddShader(StringtoWstring("DefferedLightPShader.cso").c_str(), PS, &mHWND);

	pBlurHCShader->AddShader(StringtoWstring("BlurComputeH.cso").c_str(), CS, &mHWND);
	pBlurVCShader->AddShader(StringtoWstring("BlurComputeV.cso").c_str(), CS, &mHWND);

	vector<D3D11_INPUT_ELEMENT_DESC> AnimedSDLayout;
	AnimedSDLayout.push_back(
		D3D11_INPUT_ELEMENT_DESC{
			"POSITION" ,                //SemanticName
			0,                          //SemanticIndex
			DXGI_FORMAT_R32G32B32_FLOAT,//Format
			0,                          //InputSlot
			0,                          //AlignedByteOffset
			D3D11_INPUT_PER_VERTEX_DATA,//InputSlotClass
			0                           //InstanceDataStepRate
		});

	/* same as this
		D3D11_INPUT_ELEMENT_DESC temp;
		temp.SemanticName = "POSITION";
		temp.SemanticIndex = 0;
		temp.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		temp.InputSlot = 0;
		temp.AlignedByteOffset = 0;
		temp.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		temp.InstanceDataStepRate = 0;
	*/
	AnimedSDLayout.push_back(
		D3D11_INPUT_ELEMENT_DESC{
			"NORMAL" ,                      //SemanticName
			0,                             //SemanticIndex
			DXGI_FORMAT_R32G32B32_FLOAT,//Format
			0,                             //InputSlot
			D3D11_APPEND_ALIGNED_ELEMENT,  //AlignedByteOffset
			D3D11_INPUT_PER_VERTEX_DATA,   //InputSlotClass
			0                              //InstanceDataStepRate
		});
	AnimedSDLayout.push_back(
		D3D11_INPUT_ELEMENT_DESC{
			"TEXCOORD" ,                      //SemanticName
			0,                             //SemanticIndex
			DXGI_FORMAT_R32G32_FLOAT,//Format
			0,                             //InputSlot
			D3D11_APPEND_ALIGNED_ELEMENT,  //AlignedByteOffset
			D3D11_INPUT_PER_VERTEX_DATA,   //InputSlotClass
			0                              //InstanceDataStepRate
		});
	AnimedSDLayout.push_back(
		D3D11_INPUT_ELEMENT_DESC{
			"TANGENT" ,                      //SemanticName
			0,                             //SemanticIndex
			DXGI_FORMAT_R32G32B32_FLOAT,//Format
			0,                             //InputSlot
			D3D11_APPEND_ALIGNED_ELEMENT,  //AlignedByteOffset
			D3D11_INPUT_PER_VERTEX_DATA,   //InputSlotClass
			0                              //InstanceDataStepRate
		});
	AnimedSDLayout.push_back(
		D3D11_INPUT_ELEMENT_DESC{
			"WEIGHTS" ,                      //SemanticName
			0,                             //SemanticIndex
			DXGI_FORMAT_R32G32B32A32_FLOAT,//Format
			0,                             //InputSlot
			D3D11_APPEND_ALIGNED_ELEMENT,  //AlignedByteOffset
			D3D11_INPUT_PER_VERTEX_DATA,   //InputSlotClass
			0                              //InstanceDataStepRate
		});
	AnimedSDLayout.push_back(
		D3D11_INPUT_ELEMENT_DESC{
			"BONEINDICES" ,                      //SemanticName
			0,                             //SemanticIndex
			DXGI_FORMAT_R32G32B32A32_UINT,//Format
			0,                             //InputSlot
			D3D11_APPEND_ALIGNED_ELEMENT,  //AlignedByteOffset
			D3D11_INPUT_PER_VERTEX_DATA,   //InputSlotClass
			0                              //InstanceDataStepRate
		});
	pAnimedVShader->AddLayout(AnimedSDLayout);
	

	vector<D3D11_INPUT_ELEMENT_DESC> SimpleSDLayout;
	SimpleSDLayout.push_back(
		D3D11_INPUT_ELEMENT_DESC{
			"POSITION" ,                //SemanticName
			0,                          //SemanticIndex
			DXGI_FORMAT_R32G32B32_FLOAT,//Format
			0,                          //InputSlot
			0,                          //AlignedByteOffset
			D3D11_INPUT_PER_VERTEX_DATA,//InputSlotClass
			0                           //InstanceDataStepRate
		});
	SimpleSDLayout.push_back(
		D3D11_INPUT_ELEMENT_DESC{
			"NORMAL" ,                      //SemanticName
			0,                             //SemanticIndex
			DXGI_FORMAT_R32G32B32_FLOAT,//Format
			0,                             //InputSlot
			D3D11_APPEND_ALIGNED_ELEMENT,  //AlignedByteOffset
			D3D11_INPUT_PER_VERTEX_DATA,   //InputSlotClass
			0                              //InstanceDataStepRate
		});
	SimpleSDLayout.push_back(
		D3D11_INPUT_ELEMENT_DESC{
			"TEXCOORD" ,                      //SemanticName
			0,                             //SemanticIndex
			DXGI_FORMAT_R32G32_FLOAT,//Format
			0,                             //InputSlot
			D3D11_APPEND_ALIGNED_ELEMENT,  //AlignedByteOffset
			D3D11_INPUT_PER_VERTEX_DATA,   //InputSlotClass
			0                              //InstanceDataStepRate
		});
	SimpleSDLayout.push_back(
		D3D11_INPUT_ELEMENT_DESC{
			"TANGENT" ,                      //SemanticName
			0,                             //SemanticIndex
			DXGI_FORMAT_R32G32B32_FLOAT,//Format
			0,                             //InputSlot
			D3D11_APPEND_ALIGNED_ELEMENT,  //AlignedByteOffset
			D3D11_INPUT_PER_VERTEX_DATA,   //InputSlotClass
			0                              //InstanceDataStepRate
		});
	pSimpleVShader->AddLayout(SimpleSDLayout);
	pDeferredVShader->AddLayout(SimpleSDLayout);

	D3D11_BUFFER_DESC worldMatrixDesc;//register(b0)
	worldMatrixDesc.Usage = D3D11_USAGE_DYNAMIC;
	worldMatrixDesc.ByteWidth = sizeof(WorldMatrixBuffer);
	worldMatrixDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	worldMatrixDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	worldMatrixDesc.MiscFlags = 0;
	worldMatrixDesc.StructureByteStride = 0;
	pWorldMatrixBuff = MakeMatrixBuffer(worldMatrixDesc);

	D3D11_BUFFER_DESC ObjectDesc;//register(b1)
	ObjectDesc.Usage = D3D11_USAGE_DYNAMIC;
	ObjectDesc.ByteWidth = sizeof(ObjectInfo);
	ObjectDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	ObjectDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ObjectDesc.MiscFlags = 0;
	ObjectDesc.StructureByteStride = 0;
	pObjectBuff = MakeMatrixBuffer(ObjectDesc);

	D3D11_BUFFER_DESC ModelDesc;//register(b2)
	ModelDesc.Usage = D3D11_USAGE_DYNAMIC;
	ModelDesc.ByteWidth = sizeof(ModelTransformation);
	ModelDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	ModelDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ModelDesc.MiscFlags = 0;
	ModelDesc.StructureByteStride = 0;
	pModelBuff = MakeMatrixBuffer(ModelDesc);

	D3D11_BUFFER_DESC lightDesc;//register(b3)
	lightDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightDesc.ByteWidth = sizeof(LightInfo);
	lightDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightDesc.MiscFlags = 0;
	lightDesc.StructureByteStride = 0;
	pLightBuff = MakeMatrixBuffer(lightDesc);

	D3D11_BUFFER_DESC boneDesc;//register(b4)
	boneDesc.Usage = D3D11_USAGE_DYNAMIC;
	boneDesc.ByteWidth = sizeof(BoneTransform);
	boneDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	boneDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	boneDesc.MiscFlags = 0;
	boneDesc.StructureByteStride = 0;
	pBoneBuff = MakeMatrixBuffer(boneDesc);

	D3D11_BUFFER_DESC LightPVDesc;//register(b5)
	LightPVDesc.Usage = D3D11_USAGE_DYNAMIC;
	LightPVDesc.ByteWidth = sizeof(LightInform);
	LightPVDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	LightPVDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	LightPVDesc.MiscFlags = 0;
	LightPVDesc.StructureByteStride = 0;
	pLightPVBuff = MakeMatrixBuffer(LightPVDesc);

	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	result = pDirectX->GetDevice()->CreateSamplerState(&samplerDesc, &pDefaultSamplerState);
	pDirectX->GetDeviceContext()->PSSetSamplers(0, 1, &pDefaultSamplerState);

	return result;
}
bool GraphicsManager::InitializeDefaultLight()
{
	pLight = new Light();
	//direct light example
	pLight->SetPosition(0, 0.0f, 300.0f);
	pLight->SetDirection(0.0f, 0.0f, -1.0f);
	pLight->SetDiffuse(0.5f, 0.5f, 0.5f);
	pLight->SetAmbient(0.1f, 0.1f, 0.1f);
	pLight->SetSpecular(0.6f, 0.6f, 0.6f);
	pLight->SetType(DIRECT_LIGHT);
	pLight->HasShadow(true);
	pLight->ShadowMap().Initialize(1024, 1024);

	//InitLocalLights();
	//point light example
	//mLight->setPosition(0.0f, -3.0f, 0.0f);
	//mLight->setDiffuse(1.0f, 1.0f, 1.0f);
	//mLight->setAmbient(0.2f, 0.2f, 0.2f);
	//mLight->setSpecular(1.0f, 1.0f, 1.0f);
	//mLight->setType(POINT_LIGHT);
	//mLight->setAttenuation(1.0f,0.09f,0.032f);

	//spot light
	//mLight->setPosition(0.0f, -3.0f, 0.0f);
	//mLight->setDirection(0.0f, 1.0f, 0.0f);
	//mLight->setDiffuse(1.0f, 1.0f, 1.0f);
	//mLight->setAmbient(0.2f, 0.2f, 0.2f);
	//mLight->setSpecular(1.0f, 1.0f, 1.0f);
	//mLight->setAttenuation(1.0f, 0.09f, 0.032f);
	//mLight->setCutOff(12.5f, 17.5f);
	//mLight->setType(SPOT_LIGHT);
	return false;
}
void GraphicsManager::CreateGaussianWeights(int wr)
{
	int weightRange = wr;
	float sum = 0.0f;
	float s = weightRange / 2.0f;
	for (int i = 0; i < 2 * weightRange + 1; i++)
	{
		float temp= powf(exp(1.0), -0.5f*powf((-(float)weightRange + i) / s, 2.0f));
		if (i % 4 == 0)
		{
			GW.weight[i / 4].x = temp;
		}
		else if (i % 4 == 1)
		{
			GW.weight[i / 4].y = temp;
		}
		else if (i % 4 == 2)
		{
			GW.weight[i / 4].z = temp;
		}
		else /*if (i % 4 == 3)*/
		{
			GW.weight[i / 4].w = temp;
		}
		sum += temp;
	}
	for (int i = 0; i < 2 * weightRange + 1; i++)
	{
		if (i % 4 == 0)
		{
			GW.weight[i / 4].x = GW.weight[i / 4].x / sum;
		}
		else if (i % 4 == 1)
		{
			GW.weight[i / 4].y = GW.weight[i / 4].y / sum;
		}
		else if (i % 4 == 2)
		{
			GW.weight[i / 4].z = GW.weight[i / 4].z / sum;
		}
		else /*if (i % 4 == 3)*/
		{
			GW.weight[i / 4].w = GW.weight[i / 4].w / sum;
		}
	}
	GW.w= wr;
}

bool GraphicsManager::InitializeBlur(int wr)
{
	CreateGaussianWeights(wr);

	D3D11_BUFFER_DESC GWDesc;
	ZeroMemory(&GWDesc, sizeof(GWDesc));
	GWDesc.Usage = D3D11_USAGE_DYNAMIC;
	GWDesc.ByteWidth = sizeof(GaussianWeights);
	GWDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	GWDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pDirectX->GetDevice()->CreateBuffer(&GWDesc, nullptr, &pGWBuff);

	D3D11_MAPPED_SUBRESOURCE MappedResource;
	pDirectX->GetDeviceContext()->Map(pGWBuff, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	GaussianWeights* constants = (GaussianWeights*)MappedResource.pData;
	memcpy(MappedResource.pData,&GW,sizeof(GW));

	pDirectX->GetDeviceContext()->Unmap(pGWBuff, 0);

	return true;
}
void GraphicsManager::Blurring(int w, int h, ID3D11ShaderResourceView* input, ID3D11ShaderResourceView**output)
{

	ID3D11UnorderedAccessView*	pUAV;
	ID3D11ShaderResourceView*	pSRV;
	D3D11_TEXTURE2D_DESC textDESC;
	ZeroMemory(&textDESC, sizeof(textDESC));
	textDESC.Width = w;
	textDESC.Height = h;
	textDESC.MipLevels = 1;
	textDESC.ArraySize = 1;
	textDESC.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textDESC.SampleDesc.Count = 1;
	textDESC.SampleDesc.Quality = 0;
	textDESC.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	textDESC.CPUAccessFlags = 0;
	textDESC.MiscFlags = 0;

	ID3D11Texture2D* texture = nullptr;
	pDirectX->GetDevice()->CreateTexture2D(&textDESC, 0, &texture);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	ZeroMemory(&uavDesc, sizeof(uavDesc));
	uavDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	uavDesc.Texture2D.MipSlice = 0;
	auto hr = pDirectX->GetDevice()->CreateUnorderedAccessView(texture, &uavDesc, &pUAV);

	D3D11_SHADER_RESOURCE_VIEW_DESC resDESC;
	ZeroMemory(&resDESC, sizeof(resDESC));
	resDESC.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	resDESC.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	resDESC.Texture2D.MostDetailedMip = 0;
	resDESC.Texture2D.MipLevels = 1;

	pDirectX->GetDevice()->CreateShaderResourceView(texture, &resDESC, &pSRV);
	RELEASE_COM(texture);
	ID3D11UnorderedAccessView* emptyuav = 0;
	ID3D11ShaderResourceView* emptySRV = 0;
	ID3D11Buffer* emptyBuff = 0;

	ID3D11UnorderedAccessView*	pUAV1;
	ID3D11ShaderResourceView*	pSRV1;
	{
		D3D11_TEXTURE2D_DESC textDESC;
		ZeroMemory(&textDESC, sizeof(textDESC));
		textDESC.Width = w;
		textDESC.Height = h;
		textDESC.MipLevels = 1;
		textDESC.ArraySize = 1;
		textDESC.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textDESC.SampleDesc.Count = 1;
		textDESC.SampleDesc.Quality = 0;
		textDESC.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		textDESC.CPUAccessFlags = 0;
		textDESC.MiscFlags = 0;

		ID3D11Texture2D* texture = nullptr;
		pDirectX->GetDevice()->CreateTexture2D(&textDESC, 0, &texture);

		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
		ZeroMemory(&uavDesc, sizeof(uavDesc));
		uavDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		uavDesc.Texture2D.MipSlice = 0;
		auto hr = pDirectX->GetDevice()->CreateUnorderedAccessView(texture, &uavDesc, &pUAV1);

		D3D11_SHADER_RESOURCE_VIEW_DESC resDESC;
		ZeroMemory(&resDESC, sizeof(resDESC));
		resDESC.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		resDESC.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		resDESC.Texture2D.MostDetailedMip = 0;
		resDESC.Texture2D.MipLevels = 1;

		pDirectX->GetDevice()->CreateShaderResourceView(texture, &resDESC, &pSRV1);

		RELEASE_COM(texture);
	}

	pBlurHCShader->Use();
	pDirectX->GetDeviceContext()->CSSetConstantBuffers(0, 1, &pGWBuff);
	pDirectX->GetDeviceContext()->CSSetShaderResources(0, 1, &input);
	pDirectX->GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, &pUAV, NULL);

	if (w % 128 > 0)
	{
		pDirectX->GetDeviceContext()->Dispatch(w / 128 + 1, h, 1);
	}
	else
	{
		pDirectX->GetDeviceContext()->Dispatch(w / 128, h, 1);
	}

	pDirectX->GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, &emptyuav, 0);
	pDirectX->GetDeviceContext()->CSSetShaderResources(0, 1, &emptySRV);
	pDirectX->GetDeviceContext()->CSSetConstantBuffers(0, 1, &emptyBuff);

	pBlurHCShader->Unuse();

	//std::swap(*input, pSRV);

	pBlurVCShader->Use();
	pDirectX->GetDeviceContext()->CSSetConstantBuffers(0, 1, &pGWBuff);
	pDirectX->GetDeviceContext()->CSSetShaderResources(0, 1, &pSRV);
	pDirectX->GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, &pUAV1, NULL);
	if (h % 128 > 0)
	{
		pDirectX->GetDeviceContext()->Dispatch(w, h / 128 + 1, 1);
	}
	else
	{
		pDirectX->GetDeviceContext()->Dispatch(w, h / 128 , 1);
	}
	
	pDirectX->GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, &emptyuav, 0);
	pDirectX->GetDeviceContext()->CSSetShaderResources(0, 1, &emptySRV);
	pDirectX->GetDeviceContext()->CSSetConstantBuffers(0, 1, &emptyBuff);

	pBlurVCShader->Unuse();
	std::swap(*output, pSRV1);
	RELEASE_COM(pSRV);
	RELEASE_COM(pUAV);
	RELEASE_COM(pSRV1);
	RELEASE_COM(pUAV1);
	
}
void GraphicsManager::DrawDebug()
{
	float divisions = 20.0f;
	float halfdiv = divisions / 2.0f;
	Vector4 xaxis(1.f, 0.f, 0.01f, 1.0);
	Vector4 yaxis(0.f, 1.f, 0.01f, 1.0);
	Vector4 scale = xaxis * (-divisions / 2.0f);

	for (long i = 0; i <= divisions; ++i)
	{
		Vector4 v1(scale - yaxis * halfdiv);
		Vector4 v2(scale + yaxis * halfdiv);
		AddDrawingLine(v1, v2, Vector3(Colors::White));
		scale = scale + xaxis;
	}
	scale = yaxis * (-divisions / 2);
	for (long i = 0; i <= divisions; i++)
	{
		Vector4 v1(scale - xaxis * halfdiv);
		Vector4 v2(scale + xaxis * halfdiv);
		scale = scale + yaxis;
		AddDrawingLine(v1, v2, Vector3(Colors::White));
	}
}
void GraphicsManager::DrawLine()
{
	BindObjectInfo(pDirectX->GetDeviceContext(), pObjectBuff,
		{ Vector3(1),120,Vector3(1),-1 });
	BindModelTransformation(pDirectX->GetDeviceContext(), pModelBuff,
		Matrix());
	pSimpleVShader->Use();
	BindBoneTransformation(pDirectX->GetDeviceContext(), pBoneBuff, vector<Matrix>());
	while (!LLineQueue.empty())
	{
		pBrush->Begin();
		VertexPositionColor v1(LLineQueue.front().LineA, Vector4(LLineQueue.front().Color.x, LLineQueue.front().Color.y, LLineQueue.front().Color.z, 1));
		VertexPositionColor v2(LLineQueue.front().LineB, Vector4(LLineQueue.front().Color.x, LLineQueue.front().Color.y, LLineQueue.front().Color.z, 1));
		AddDrawingPoint(LLineQueue.front().LineA,Vector3(Colors::White), false);
		AddDrawingPoint(LLineQueue.front().LineB, Vector3(Colors::White), false);
		BindObjectInfo(pDirectX->GetDeviceContext(), pObjectBuff,
			{ LLineQueue.front().Color,120,Vector3(1),-1 });
		if (LLineQueue.front().ZBuffer) pDirectX->EnableZBuffer(true);
		pBrush->DrawLine(v1, v2);
		pDirectX->EnableZBuffer(false);
		LLineQueue.pop();
		pBrush->End();

	}
}
void GraphicsManager::DrawPoint()
{
	BindObjectInfo(pDirectX->GetDeviceContext(), pObjectBuff,
		{ Vector3(1),120,Vector3(1),-1 });
	BindModelTransformation(pDirectX->GetDeviceContext(), pModelBuff,
		Matrix());
	pSimpleVShader->Use();
	BindBoneTransformation(pDirectX->GetDeviceContext(), pBoneBuff, vector<Matrix>());
	while (!LPointQueue.empty())
	{
		BindObjectInfo(pDirectX->GetDeviceContext(), pObjectBuff,
			{ LPointQueue.front().Color,120,Vector3(1),-1 });
		if (LPointQueue.front().ZBuffer) pDirectX->EnableZBuffer(true);
		Vector4 temppoint = LPointQueue.front().PointA;
		Matrix temp = Matrix::CreateTranslation(temppoint.x, temppoint.y, temppoint.z).Transpose()
			*Matrix::CreateScale(0.2).Transpose();
		pPoint->SetObjectTr(temp);
		pPoint->SetDiffuse(LPointQueue.front().Color.x, LPointQueue.front().Color.y, LPointQueue.front().Color.z);
		pPoint->SetMaterialID(-1);
		pPoint->Draw(pObjectBuff, pModelBuff, pBoneBuff);
		pDirectX->EnableZBuffer(false);
		LPointQueue.pop();
	}
}
void GraphicsManager::DrawBones(Asset* rpasset)
{
	FBX* tempFBX = dynamic_cast<FBX*>(rpasset->obj->pShape);
	rpasset->obj->DrawBoneHelper(tempFBX->mRootBone, MyMul(tempFBX->mModelTr,rpasset->obj->GetObjectTr()));
}

void GraphicsManager::DrawMouse()
{
	int x, y;
	DXInputManager::GetInstance()->GetMouseLocation(x, y);
	float pointX = ((2.0f * (float)x) / (float)mScreenWidth) - 1.0f;
	float pointY = (((2.0f * (float)y) / (float)mScreenHeight) - 1.0f) * -1.0f;

	auto m = MyPerspective((0.4f*mScreenWidth) / mScreenHeight, 0.4f, SCREEN_NEAR, SCREEN_FAR);
	pointX = pointX / m._11;
	pointY = pointY / m._22;

	//Matrix WorldView;
	////pCamera->GetViewMatrix(WorldView);
	//pCamera->Get3rdPersonView(WorldView);

	Vector4 direction = MyVectorTransform(WorldView, Vector4(pointX, pointY, -1, 1));
	direction.Normalize();
	//float cx, cy, cz;
	//pCamera->GetPosition(cx, cy, cz);
	auto Origin = MyVectorTransform(WorldView.Invert().Transpose(), Vector4(0, 0, 0, 1));

	auto d = Origin + 100000 * direction;
	d.w = 1;
	RayOrigin = Vector3(Origin);
	RayDir = Vector3(direction);
	AddDrawingLine(d, Vector4(Origin), Vector3(Colors::Green));

}
void GraphicsManager::DrawBoundingBox(Body * rbody,Vector3 Color)
{

	auto center = Vector4(rbody->mPos_x, rbody->mPos_y, rbody->mPos_z, 1);
	auto shapex = static_cast<ShapeAABB*>(rbody->mShape);
	Vector4 p1(center + Vector3(shapex->mLeft, shapex->mIn, shapex->mTop));
	Vector4 p2(center + Vector3(shapex->mLeft, shapex->mIn, shapex->mBottom));
	Vector4 p3(center + Vector3(shapex->mLeft, shapex->mOut, shapex->mTop));
	Vector4 p4(center + Vector3(shapex->mLeft, shapex->mOut, shapex->mBottom));
	Vector4 p5(center + Vector3(shapex->mRight, shapex->mIn, shapex->mTop));
	Vector4 p6(center + Vector3(shapex->mRight, shapex->mIn, shapex->mBottom));
	Vector4 p7(center + Vector3(shapex->mRight, shapex->mOut, shapex->mTop));
	Vector4 p8(center + Vector3(shapex->mRight, shapex->mOut, shapex->mBottom));
	AddDrawingLine(p1, p2, Color);
	AddDrawingLine(p3, p4, Color);
	AddDrawingLine(p1, p3, Color);
	AddDrawingLine(p2, p4, Color);

	AddDrawingLine(p1, p5, Color);
	AddDrawingLine(p2, p6, Color);
	AddDrawingLine(p3, p7, Color);
	AddDrawingLine(p4, p8, Color);

	AddDrawingLine(p5, p6, Color);
	AddDrawingLine(p7, p8, Color);
	AddDrawingLine(p5, p7, Color);
	AddDrawingLine(p6, p8, Color);
}