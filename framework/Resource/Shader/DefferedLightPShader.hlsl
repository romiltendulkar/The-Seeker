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

cbuffer LightList : register(b3)
{
	LightInfo light;
};
Texture2D utexture[6]: register(t0);
Texture2D shadow_texture[3]: register(t6);
Texture2D P_texture: register(t9);
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
	float4 diffuse = float4(sRGB_to_linear(utexture[2].Sample(samLinear, input.tex).xyz),1);
	float4 specular_and_shininess = utexture[3].Sample(samLinear, input.tex);
	float3 specular = sRGB_to_linear(specular_and_shininess.xyz);
	float shininess = specular_and_shininess.w;
	float material_id = utexture[4].Sample(samLinear, input.tex).x;
	float4 t6 = utexture[5].Sample(samLinear, input.tex);
	float4 t7= P_texture.Sample(samLinear, input.tex);

	float4 shadow = shadow_texture[0].Sample(samLinear, input.tex);
	clip(diffuse.w);

	if (material_id == DEBUG_LINE) return float4(diffuse.xyz, 1);
	if (material_id == SKYDOME) return float4(0,0,0, 1);
	if (material_id == 3) return float4(diffuse.xyz, 1);
	if (material_id == 4) return float4(diffuse.xyz, 1);
	if (material_id == 5) return float4(diffuse.xyz, 1);
	if (material_id == BUTTON) return float4(0,0,0, 1);
	if (material_id == BUTTONB) return float4(0, 0, 0, 1);
	if (material_id == PARTICLE) return float4(diffuse.xyz, 1);
	float3 lightVec = light.Position - world_pos.xyz;
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
	float3 N=normalize(normal_vec);
	float3 V=normalize(input.eyespos - world_pos.xyz);

	float4 output = attenuation * (intensity*BRDF(light, N, L, V, diffuse.xyz, specular, shininess));
	if(t7.x>0 || t7.y > 0 || t7.z > 0 )
	{
		output = float4(ToneMapping(output.xyz), 1);
		output.xyz = output.xyz+ t7.xyz * 2;
		output = float4(ToneMapping(output.xyz), 1);

		return output;
	}
	output = float4(ToneMapping(output.xyz),1);
	//output.w = 1.0f;

	if (light.shadowMartix[3][3] >0)
	{
		//return output;
		float4 shadowCoord = mul(world_pos, light.shadowMartix);
		if (shadowCoord.w > 0)
		{
			float2 shadowIndex = shadowCoord.xy / shadowCoord.w;
			shadowIndex.y = 1.0f - shadowIndex.y;
			if (shadowIndex.x >= 0 && shadowIndex.x <= 1 && shadowIndex.y >= 0 && shadowIndex.y <= 1)
			{
				float4 shadow2 = shadow_texture[0].Sample(samLinear, shadowIndex);
				float Lightdepth = shadow2.w;
				float Pixeldepth = shadowCoord.w;
				if (Pixeldepth > Lightdepth + 0.001f)
				{
					output = float4(0.0, 0.0, 0.0, 0);
				}
			}
			//else
			//{
			//	//output = float4(0.0, 0.0, 0.0, 0);
			//}
		}
		else
		{
			output = float4(0.0, 0.0, 0.0, 0);
		}
	}

	return output;
}
