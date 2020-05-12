////////////////////////////////////////////////////////////////////////////////
// Filename: color.vs
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
};
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
	float4 in_vertex = float4(input.vertex.xyz, 1.0f) ;
    output.position = mul(in_vertex, ModelTr);
	output.worldpos=output.position;
    output.position = mul(output.position, WorldView);
	output.position = mul(output.position, WorldProj);
	
	output.normal=mul(input.vertexNormal, (float3x3)NormalTr);
	
	output.eyespos=mul(float4(0,0,0,1),WorldInverse).xyz;
	
	output.tex = float2(input.vertexTexture.x, 1.0f - input.vertexTexture.y);
	output.tanVec = mul(input.vertexTangent, ModelTr).xyz;
    return output;
}
