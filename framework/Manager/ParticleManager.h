
#pragma once
#include "../Source/pch.h"
#include "GraphicsManager.h"
class ParticleManager
{
public:
	ParticleManager();
	void Initialize(ID3D11Device* device, ID3D11DeviceContext* DeviceContext);
	~ParticleManager();
	void Emit(int numEmitters, const Particle& emitters);
	void SphereEmit(float angle,  Particle& emitters);
	void TimerEmit(int timer, Particle& emitters);
	void Test();
	void Update(float dt);
	void Draw();
	static ParticleManager* GetInstance();
	void Destroy();

private:
	int	ReadCounter();
	float mTimer = 0;
	static ParticleManager* mInstance;
	ID3D11DeviceContext* pDeviceContext;
	ID3D11Device* pDevice;
	DXTexture* pPaticleTexture1;
	DXTexture* pPaticleTexture2;
	DXTexture* pPaticleTexture3;
	DXTexture* pPaticleTexture4;

	ID3D11Buffer*				pEmitterCBuffer;
	ID3D11Buffer*				pTimerCBuffer;

	ID3D11Buffer *pStructuredBuffer;
	ID3D11UnorderedAccessView*	pParticleUAV;
	ID3D11ShaderResourceView*	pParticleSRV;

	ID3D11Buffer *pGPUCounterBuffer;
	ID3D11Buffer *pCPUCounterBuffer;
	ID3D11UnorderedAccessView*	pCounterUAV;
	ID3D11ShaderResourceView*	pCounterSRV;

	bool test=false;
};

