
struct Particle {
	float4 position;
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
cbuffer Timer: register(b0)
{
	float dt;
	float3 pad;
};
[numthreads(512, 1, 1)]
void main(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	if (dispatchThreadID.x + dispatchThreadID.y * 512 == 0)
	{
		outCounter[0].alive = 0;
	}
	
	if (outparticles[dispatchThreadID.x + dispatchThreadID.y * 512].alive == true)
	{
		outparticles[dispatchThreadID.x + dispatchThreadID.y * 512].age -= dt;
		if (outparticles[dispatchThreadID.x + dispatchThreadID.y * 512].age > 0)
		{
			float3 pos = outparticles[dispatchThreadID.x + dispatchThreadID.y * 512].position.xyz;
			float3 vel = outparticles[dispatchThreadID.x + dispatchThreadID.y * 512].velocity.xyz;
			pos = pos + dt * vel;
			outparticles[dispatchThreadID.x + dispatchThreadID.y * 512].position.x = pos.x;
			outparticles[dispatchThreadID.x + dispatchThreadID.y * 512].position.y = pos.y;
			outparticles[dispatchThreadID.x + dispatchThreadID.y * 512].position.z = pos.z;
			if (outparticles[dispatchThreadID.x + dispatchThreadID.y * 512].TextureIndex == 2)
			{
				outparticles[dispatchThreadID.x + dispatchThreadID.y * 512].position.w += dt / 3;
				outparticles[dispatchThreadID.x + dispatchThreadID.y * 512].velocity.w += dt / 3;
			}
			InterlockedAdd(outCounter[0].alive, 1);
		}
		else
		{
			outparticles[dispatchThreadID.x + dispatchThreadID.y * 512].alive = false;
		}
		//outparticles[dispatchThreadID.x + dispatchThreadID.y * 1024].position
		//	= outparticles[dispatchThreadID.x + dispatchThreadID.y * 1024].position*
		//	0.016*outparticles[dispatchThreadID.x + dispatchThreadID.y * 1024].velocity;
		

	}
}