

/////////////
// GLOBALS //
/////////////
Texture2D inputTexure: register(t0);
RWTexture2D<float4> outputTexure: register(u0);
cbuffer GaussianWeights : register(b0)
{
	float4 weight[25];
	int w;
	int pad1;
	int pad2;
	int pad3;
};
groupshared float4 sharemem[128 + 101];

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
[numthreads(1, 128, 1)]
void main(uint3 dispatchThreadID : SV_DispatchThreadID, uint3 groupThreadID : SV_GroupThreadID)
{
	uint i = groupThreadID.y;
	sharemem[i] = inputTexure[dispatchThreadID.xy + int2(0, -w)];
	if (i < 2 * w)
	{
		sharemem[i + 128] = inputTexure[dispatchThreadID.xy + int2(0, 128 - w)];
	}
	GroupMemoryBarrierWithGroupSync();
	float4 sum = float4(0, 0, 0, 0);
	for (int j = 0; j < 2 * w + 1; j++)
	{

		if (j % 4 == 0)
		{
			sum += weight[j / 4].x * sharemem[i + j];
		}
		else if (j % 4 == 1)
		{
			sum += weight[j / 4].y * sharemem[i + j];
		}
		else if (j % 4 == 2)
		{
			sum += weight[j / 4].z * sharemem[i + j];
		}
		else
		{
			sum += weight[j / 4].w * sharemem[i + j];
		}
	}
	outputTexure[dispatchThreadID.xy] = sum;// Write to destination image
}