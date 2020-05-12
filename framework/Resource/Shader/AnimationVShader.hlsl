////////////////////////////////////////////////////////////////////////////////
// Filename: AnimationShader.vs
////////////////////////////////////////////////////////////////////////////////
static matrix Identity =
{
	{ 1, 0, 0, 0 },
	{ 0, 1, 0, 0 },
	{ 0, 0, 1, 0 },
	{ 0, 0, 0, 1 }
};


/////////////
// GLOBALS //
/////////////
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



//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
	float3 vertex : POSITION;
	float3 vertexNormal : NORMAL;
	float2 vertexTexture : TEXCOORD;
	float3 vertexTangent: TANGENT;
	float4 boneWeight: WEIGHTS;
	uint4 boneId: BONEINDICES;
};
cbuffer BoneTransform : register(b4)
{
	matrix boneTr[50];
}
struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD;
	float3 normal : NORMAL;
	float4 worldpos : WORLDPOS;
	float3 eyespos : EYESPOS;
	float3 tanVec : TANVEC;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType main(VertexInputType input)
{

	PixelInputType output;

	// Change the position vector to be 4 units for proper matrix calculations.
	//input.vertex.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	float4 in_vertex = float4(input.vertex,1.0f);
	float4 in_normal = float4(input.vertexNormal, 0.0f);
	//float4 in_vertex = input.vertex;
	float3 posL = float3(0.0f, 0.0f, 0.0f);
	float3 norL = float3(0.0f, 0.0f, 0.0f);
	float weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	weights[0] = input.boneWeight.x;
	weights[1] = input.boneWeight.y;
	weights[2] = input.boneWeight.z;
	weights[3] = 1.0f - weights[0] - weights[1] - weights[2];
	for (int i = 0; i < 4; ++i)
	{
		posL += weights[i] * mul(in_vertex, boneTr[input.boneId[i]]).xyz;
		norL += weights[i] * mul(in_normal, boneTr[input.boneId[i]]).xyz;
	}


	output.position = mul(float4(posL, 1.0f), ModelTr);
	output.worldpos = output.position;
	output.position = mul(output.position, WorldView);
	output.position = mul(output.position, WorldProj);

	output.normal = mul(float4(norL, 0.0f), NormalTr).xyz;

	output.eyespos = mul(float4(0, 0, 0, 1), WorldInverse).xyz;

	output.tex = float2(input.vertexTexture.x, 1.0f - input.vertexTexture.y);

	output.tanVec = mul(input.vertexTangent, ModelTr).xyz;

	return output;
}
