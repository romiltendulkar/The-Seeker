//pixel shader

#include "ShaderUtil.hlsl"
cbuffer ObjectInfo: register(b1)
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
Texture2D utexture[GBUFFER_SIZE]: register(t0);
Texture2D shadow_texture: register(t6);
Texture2D normal_texture: register(t7);

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 worldpos : WORLDPOS;
	float3 eyespos : EYESPOS;
};

float4 main(PixelInputType input) : SV_TARGET
{
	float4 world_pos = utexture[0].Sample(samLinear, input.tex);
	float3 normal_vec = utexture[1].Sample(samLinear, input.tex).xyz;
	float3 diffuse = utexture[2].Sample(samLinear, input.tex).xyz;
	float4 specular_and_shininess = utexture[3].Sample(samLinear, input.tex);
	float3 specular = specular_and_shininess.xyz;
	float shininess = specular_and_shininess.w;
	float material_id = utexture[4].Sample(samLinear, input.tex).x;
	float4 t6 = utexture[5].Sample(samLinear, input.tex);
	float4 shadow = shadow_texture.Sample(samLinear, input.tex);


	if (material_id == -1||material_id == 1) return float4(diffuse.xyz, 1);
	if (material_id == BUTTON) return float4(linear_to_sRGB((diffuse.xyz)), 1);
	if (material_id == BUTTONB) return float4(linear_to_sRGB((diffuse.xyz)), 1);
	float3 lightVec = light.Position - world_pos.xyz;
	float attenuation = 1.0;
	if (light.Attribute.x == 1)
	{
		float distance = length(lightVec);
		attenuation = 1.0 / (light.Constant + light.Linear * distance +
					light.Quadratic * (distance * distance));
	}
	float4 output = attenuation * (float4(light.Ambient*diffuse, 1.0f));
	return   output;
}
