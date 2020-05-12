#include "../Source/pch.h"
#include "ParticleManager.h"


ParticleManager::ParticleManager()
{
}


struct Counter
{
	int aliveCounter;
	Vector3 pad;
};

struct Timer
{
	float dt;
	Vector3 pad;
};
void ParticleManager::Initialize(ID3D11Device* device, ID3D11DeviceContext* DeviceContext)
{
	pDevice = device;
	pDeviceContext = DeviceContext;

	pPaticleTexture1 = new DXTexture();
	pPaticleTexture2 = new DXTexture();
	pPaticleTexture3 = new DXTexture();
	pPaticleTexture1->Initialize(pDevice, pDeviceContext, wstring(L"./Resource/Image/Shooting.png").c_str());
	pPaticleTexture2->Initialize(pDevice, pDeviceContext, wstring(L"./Resource/Image/Punch.png").c_str());
	pPaticleTexture3->Initialize(pDevice, pDeviceContext, wstring(L"./Resource/Image/sleeping.png").c_str());


	//Emitter buffer
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.ByteWidth = sizeof(ParticleEmitter);
	pDevice->CreateBuffer(&desc, nullptr, &pEmitterCBuffer);
	
	//Emitter buffer
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.ByteWidth = sizeof(Timer);
	pDevice->CreateBuffer(&desc, nullptr, &pTimerCBuffer);

	{
	// Create Structured Buffer 
	D3D11_BUFFER_DESC sbDesc; 
	sbDesc.BindFlags= D3D11_BIND_UNORDERED_ACCESS |
		D3D11_BIND_SHADER_RESOURCE;
	sbDesc.CPUAccessFlags = 0;
	sbDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	sbDesc.StructureByteStride = sizeof(Particle);
	sbDesc.ByteWidth = sizeof(Particle) * 512 * 512;
	sbDesc.Usage = D3D11_USAGE_DEFAULT;
	pDevice->CreateBuffer(&sbDesc, 0, &pStructuredBuffer);

	D3D11_SHADER_RESOURCE_VIEW_DESC srv;
	srv.Format = DXGI_FORMAT_UNKNOWN;
	srv.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srv.Buffer.ElementOffset = 0;
	srv.Buffer.ElementWidth = 512 * 512;

	pDevice->CreateShaderResourceView(pStructuredBuffer, &srv, &pParticleSRV);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uav;
	uav.Format = DXGI_FORMAT_UNKNOWN;
	uav.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uav.Buffer.FirstElement = 0;
	uav.Buffer.NumElements = 512 * 512;
	uav.Buffer.Flags = 0;
	pDevice->CreateUnorderedAccessView(pStructuredBuffer, &uav, &pParticleUAV);

	//ZeroMemory(&desc, sizeof(desc));
	//desc.Usage = D3D11_USAGE_STAGING;
	//desc.BindFlags = 0;
	//desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	//desc.ByteWidth = sizeof(Particle) * 1024 * 1024;
	//pDevice->CreateBuffer(&desc, nullptr, &m_pDebugCounterBuffer);
	}
	/*********************/
	{
		// Create Structured Buffer 
		D3D11_BUFFER_DESC sbDesc;
		sbDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS |
			D3D11_BIND_SHADER_RESOURCE;
		sbDesc.CPUAccessFlags = 0;
		sbDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		sbDesc.StructureByteStride = sizeof(Counter);
		sbDesc.ByteWidth = sizeof(Counter) * 1;
		sbDesc.Usage = D3D11_USAGE_DEFAULT;
		pDevice->CreateBuffer(&sbDesc, 0, &pGPUCounterBuffer);

		D3D11_SHADER_RESOURCE_VIEW_DESC srv;
		srv.Format = DXGI_FORMAT_UNKNOWN;
		srv.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srv.Buffer.ElementOffset = 0;
		srv.Buffer.ElementWidth = 1;

		pDevice->CreateShaderResourceView(pGPUCounterBuffer, &srv, &pCounterSRV);

		D3D11_UNORDERED_ACCESS_VIEW_DESC uav;
		uav.Format = DXGI_FORMAT_UNKNOWN;
		uav.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uav.Buffer.FirstElement = 0;
		uav.Buffer.NumElements = 1;
		uav.Buffer.Flags = 0;
		pDevice->CreateUnorderedAccessView(pGPUCounterBuffer, &uav, &pCounterUAV);

		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_STAGING;
		desc.BindFlags = 0;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		desc.ByteWidth = sizeof(Counter) * 1;
		pDevice->CreateBuffer(&desc, nullptr, &pCPUCounterBuffer);
	}



}

ParticleManager::~ParticleManager()
{
}
void ParticleManager::Emit(int num, const Particle& emitters)
{
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	pDeviceContext->Map(pEmitterCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	ParticleEmitter* constants = (ParticleEmitter*)MappedResource.pData;
	constants->age = emitters.age;
	constants->position = emitters.position;
	constants->TextureIndex = emitters.TextureIndex;
	constants->velocity = emitters.velocity;
	constants->MaxParticlesNum = num+ ReadCounter();
	pDeviceContext->Unmap(pEmitterCBuffer, 0);

	pDeviceContext->CSSetConstantBuffers(0, 1, &pEmitterCBuffer);

	ID3D11UnorderedAccessView* emptyuav = 0;
	ID3D11ShaderResourceView* emptySRV = 0;
	GraphicsManager::GetInstance()->pParticleEmitCShader->Use();

	pDeviceContext->CSSetShaderResources(0, 1, &pParticleSRV);
	pDeviceContext->CSSetUnorderedAccessViews(0, 1, &pParticleUAV, NULL);
	
	pDeviceContext->CSSetShaderResources(1, 1, &pCounterSRV);
	pDeviceContext->CSSetUnorderedAccessViews(1, 1, &pCounterUAV, NULL);
	pDeviceContext->Dispatch(1, 512, 1);
	
	pDeviceContext->CSSetUnorderedAccessViews(0, 1, &emptyuav, 0);
	pDeviceContext->CSSetShaderResources(0, 1, &emptySRV);
	pDeviceContext->CSSetUnorderedAccessViews(1, 1, &emptyuav, 0);
	pDeviceContext->CSSetShaderResources(1, 1, &emptySRV);
	GraphicsManager::GetInstance()->pParticleEmitCShader->Unuse();
}
void ParticleManager::SphereEmit(float angle, Particle& emitters)
{
	Vector4 tempV = emitters.velocity;
	for (float i = 0; i <= 80; i+= angle) {

		for (float j = 0; j <= 360; j += angle) {
			float x = cos(j*DEGREE_TO_RADIAN)*sin(i*DEGREE_TO_RADIAN);
			float y = sin(j*DEGREE_TO_RADIAN)*sin(i*DEGREE_TO_RADIAN);
			float z = cos(i*DEGREE_TO_RADIAN);
			emitters.velocity = tempV *Vector4(x, y, z, 1);
			emitters.velocity.w = tempV.w;
			ParticleManager::GetInstance()->Emit(1, emitters);
		}
	}
}
void ParticleManager::TimerEmit(int timer, Particle & emitters)
{
	if (int(mTimer) % timer == 0)
	{
		ParticleManager::GetInstance()->Emit(1, emitters);
	}
}
void ParticleManager::Test()
{
	cout << "t";
}
void ParticleManager::Update(float dt)
{
	mTimer += 1;
	int timer = int(mTimer);
	if (timer > 360)
		mTimer = 0;
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	pDeviceContext->Map(pTimerCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	Timer* constants = (Timer*)MappedResource.pData;
	constants->dt = dt;
	pDeviceContext->Unmap(pTimerCBuffer, 0);

	pDeviceContext->CSSetConstantBuffers(0, 1, &pTimerCBuffer);
	ID3D11UnorderedAccessView* emptyuav = 0;
	ID3D11ShaderResourceView* emptySRV = 0;
	GraphicsManager::GetInstance()->pParticleCShader->Use();
	pDeviceContext->CSSetShaderResources(0, 1, &pParticleSRV);
	pDeviceContext->CSSetUnorderedAccessViews(0, 1, &pParticleUAV, NULL);

	pDeviceContext->CSSetShaderResources(1, 1, &pCounterSRV);
	pDeviceContext->CSSetUnorderedAccessViews(1, 1, &pCounterUAV, NULL);
	pDeviceContext->Dispatch(1, 512, 1);

	pDeviceContext->CSSetUnorderedAccessViews(0, 1, &emptyuav, 0);
	pDeviceContext->CSSetShaderResources(0, 1, &emptySRV);
	pDeviceContext->CSSetUnorderedAccessViews(1, 1, &emptyuav, 0);

	GraphicsManager::GetInstance()->pParticleCShader->Unuse();

}
void ParticleManager::Draw()
{

	BindObjectInfo(pDeviceContext, GraphicsManager::GetInstance()->pObjectBuff,
		{ Vector3(1),120,Vector3(1),6 });
	BindModelTransformation(pDeviceContext, GraphicsManager::GetInstance()->pModelBuff,
		Matrix());

	GraphicsManager::GetInstance()->pParticleVShader->Use();
	GraphicsManager::GetInstance()->pParticleGShader->Use();
	pPaticleTexture1->Bind(11);
	pPaticleTexture2->Bind(12);
	pPaticleTexture3->Bind(13);
	ID3D11Buffer* vb = nullptr;
	UINT stride = 0;
	UINT offset = 0;

	pDeviceContext->VSSetShaderResources(0, 1, &pParticleSRV);
	pDeviceContext->IASetVertexBuffers(0, 1, &vb, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	pDeviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	pDeviceContext->Draw(512 * 512, 0);
	
	GraphicsManager::GetInstance()->pParticleVShader->Unuse();
	GraphicsManager::GetInstance()->pParticleGShader->Unuse();
}
ParticleManager* ParticleManager::GetInstance()
{
	if (mInstance == nullptr)
	{
		mInstance = new ParticleManager();
	}
	return mInstance;
}
void ParticleManager::Destroy()
{
	delete mInstance;
}

int ParticleManager::ReadCounter()
{
	int count = 0;
	pDeviceContext->CopyResource(pCPUCounterBuffer, pGPUCounterBuffer);
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	pDeviceContext->Map(pCPUCounterBuffer, 0, D3D11_MAP_READ, 0, &MappedResource);
	auto pcount = (Counter*)MappedResource.pData;
	count= pcount[0].aliveCounter;
	pDeviceContext->Unmap(pCPUCounterBuffer, 0);

	return count;
}