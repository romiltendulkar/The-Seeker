////////////////////////////////////////////////////////////////////////////////
// Filename: Deferred Vectex Shader
////////////////////////////////////////////////////////////////////////////////

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
};
struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD;
	float3 normal : NORMAL;
	float4 worldpos : WORLDPOS;
	float3 eyespos : EYESPOS;
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType main(VertexInputType input)
{
	PixelInputType output;
	output.position = float4(input.vertex.xy, 0, 1.0f);
	output.tex = float2(input.vertexTexture.x,1.0f- input.vertexTexture.y);
	output.eyespos = mul(float4(0, 0, 0, 1), WorldInverse).xyz;
	return output;
}
