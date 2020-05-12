//pixel shader

#include "ShaderUtil.hlsl"
cbuffer ObjectInfo: register(b1)
{
	float3 Diffuse;
	float Shininess;
	float3 Specular;
	int MaterialID;
	uint4 TextureIO;
};
Texture2D DiffuseTexture: register(t7);
Texture2D SpecularTexture: register(t8);
Texture2D ShininessTexture: register(t9);
Texture2D NormalTexture: register(t10);
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

float4 main(PixelInputType input) : SV_TARGET
{

	if (MaterialID == -1) return float4(Diffuse, 1);
	float4 normal = float4(input.normal, 0);
	if (TextureIO.w == 1)
	{
		normal = NormalTexture.Sample(samLinear, input.tex);
	}

	float4 diffuse = float4(Diffuse, 1);
	if (TextureIO.x == 1)
	{
		diffuse = DiffuseTexture.Sample(samLinear, input.tex);
		if (SKYDOME == MaterialID)
		{
			float3 V = input.eyespos.xyz - input.worldpos.xyz;
			float3 N = normalize(V);
			float2 UV = float2(1.0f / 2.0f - atan2(N.y, N.x) / (2.0f * pi), 1 - acos(N.z) / pi);
			diffuse = DiffuseTexture.Sample(samLinear, UV);
		}
	}
	//if (diffuse.w == 0)
	//{
	//	discard;
	//}
	//clip(diffuse.w);
	float4 specular = float4(Specular.xyz, Shininess);
	if (TextureIO.y == 1)
	{
		specular.xyz = SpecularTexture.Sample(samLinear, input.tex).xyz;
	}
	if (TextureIO.z == 1)
	{
		specular.w = ShininessTexture.Sample(samLinear, input.tex).x;
	}



	
	float3 lightVec = light.Position - input.worldpos.xyz;
	float attenuation=1.0;
	if(light.Attribute.x==1)
	{
		float distance =length(lightVec);
		attenuation = 1.0 / (light.Constant + light.Linear * distance + 
					light.Quadratic * (distance * distance));
	}
	float3 L = normalize(lightVec);
	float intensity=1.0;
	if(light.Attribute.x==2)
	{
		float theta = dot(L, normalize(-light.Direction)); 
		float epsilon = light.CutOff - light.OuterCutOff;
		intensity = saturate((theta - light.OuterCutOff) / epsilon);
	}
	float3 N=normalize(normal);
	float3 V=normalize(input.eyespos - input.worldpos);
	return diffuse;
	return float4(diffuse.x, diffuse.y, diffuse.z, 1);
    return attenuation*(float4(light.Ambient*diffuse,1)+intensity*BRDF(light,N,L,V, diffuse, specular.xyz, specular.w));
}
