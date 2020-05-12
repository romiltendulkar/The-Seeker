//RWTexture2D<float4> outpos: register(u0);

struct Particle {
	float4 position;//pos+size
	float4 velocity;
	unsigned int TextureIndex;
	float age;
	bool alive;
	int age2;
};
struct Counter {
	int alive;
	float3 pad;
};
RWStructuredBuffer<Particle> outparticles: register(u0);
RWStructuredBuffer<Counter> outCounter: register(u1);
cbuffer ParticleEmitter: register(b0)
{
	float4 position;//pos+size
	float4 velocity;
	unsigned int TextureIndex;
	unsigned int MaxParticlesNum;
	float age;
	float age1;
};
[numthreads(512, 1, 1)]
void main(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	if (dispatchThreadID.x + dispatchThreadID.y * 512 == 0)
	{
		outCounter[0].alive = 0;
	}
	if (dispatchThreadID.x + dispatchThreadID.y * 512 < MaxParticlesNum&&dispatchThreadID.x + dispatchThreadID.y * 512 <(512 * 512))
	{
		if (outparticles[dispatchThreadID.x + dispatchThreadID.y * 512].alive == false)
		{
			outparticles[dispatchThreadID.x + dispatchThreadID.y * 512].position = position;
			outparticles[dispatchThreadID.x + dispatchThreadID.y * 512].velocity = velocity;
			outparticles[dispatchThreadID.x + dispatchThreadID.y * 512].TextureIndex = TextureIndex;
			outparticles[dispatchThreadID.x + dispatchThreadID.y * 512].age = age;
			outparticles[dispatchThreadID.x + dispatchThreadID.y * 512].alive = true;
			if (TextureIndex == 2)
			{
				outparticles[dispatchThreadID.x + dispatchThreadID.y * 512].position.w=0;
				outparticles[dispatchThreadID.x + dispatchThreadID.y * 512].velocity.w=0;
			}
			
		}

	}
	if (outparticles[dispatchThreadID.x + dispatchThreadID.y * 512].alive == true)
	{
		InterlockedAdd(outCounter[0].alive, 1);
	}
}