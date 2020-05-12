cbuffer WorldInform : register(b0)
{
	matrix WorldView;
	matrix WorldProj;
	matrix WorldInverse;
};
cbuffer ModelInform : register(b2)
{
	matrix ModelTr;
	matrix NormalTr;
};

struct VoutputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD;
	float3 normal : NORMAL;
	float4 worldpos : WORLDPOS;
	float3 eyespos : EYESPOS;
	bool draw : DRAW;
	int TextureID : TEXTUREID;
	float2 size:SIZE;
};
struct Particle {
	float4 position;//pos+size
	float4 velocity;
	unsigned int TextureIndex;
	float age;
	bool alive;
	int age2;
};
StructuredBuffer<Particle>	ParticleBuffer		: register(t0);

VoutputType main(uint VertexId : SV_VertexID)
{
	VoutputType output = (VoutputType)0;

	Particle pa = ParticleBuffer[VertexId];

	float4 in_vertex = float4(pa.position.xyz, 1.0f);
	output.position = mul(in_vertex, ModelTr);
	output.worldpos = output.position;
	output.position = mul(output.position, WorldView);
	output.position = mul(output.position, WorldProj);
	output.normal = float3(0,0,0);
	output.eyespos = mul(float4(0, 0, 0, 1), WorldInverse).xyz;
	output.tex = float2(0,0);
	output.draw = ParticleBuffer[VertexId].alive;
	output.TextureID= pa.TextureIndex;
	output.size.x = pa.position.w;
	output.size.y = pa.velocity.w;
	return output;
}