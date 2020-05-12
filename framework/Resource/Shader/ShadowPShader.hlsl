////////////////////////////////////////////////////////////////////////////////
// Filename: Gbuffer Pixel Shader
////////////////////////////////////////////////////////////////////////////////
#include "ShaderUtil.hlsl"
cbuffer ObjectInfo : register(b1)
{
	float3 Diffuse;
	float Shininess;
	float3 Specular;
	int MaterialID;
};
cbuffer LightList : register(b3)
{
	LightInfo light;
};
struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 worldpos : WORLDPOS;
	float3 eyespos : EYESPOS;
};
struct PixelOutputType
{
	float4 mDepth : SV_Target0;
};
PixelOutputType main(PixelInputType input) : SV_TARGET
{
	PixelOutputType output;
	output.mDepth = float4(input.position.w,
		pow(input.position.w, 2),
		pow(input.position.w, 3), 
		pow(input.position.w, 4));
	output.mDepth = input.position;
	return output;
}
