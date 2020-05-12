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
	uint4 TextureIO;
	float4 TextureSize;

};
Texture2D DiffuseTexture: register(t7);
Texture2D SpecularTexture: register(t8);
Texture2D ShininessTexture: register(t9);
Texture2D NormalTexture: register(t10);

Texture2D ParticlTexture1: register(t11);
Texture2D ParticlTexture2: register(t12);
Texture2D ParticlTexture3: register(t13);
Texture2D ParticlTexture4: register(t14);

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
	float3 tanVec : TANVEC;
	int TextureID : TEXTUREID;

};
struct PixelOutputType
{
	float4 worldPosition : SV_Target0;
	float4 normal : SV_Target1;
	float4 diffuse : SV_Target2;
	float4 specular : SV_Target3;
	float4 object1 : SV_Target4;
	float4 object2 : SV_Target5;
};

PixelOutputType main(PixelInputType input) : SV_TARGET
{

	PixelOutputType output;
	output.object2 = float4(0, 0, 0, 0);
	output.worldPosition = input.worldpos;
	float4 normal = float4(input.normal, 0);
	if (TextureIO.w == 1)
	{
		float3 delta = NormalTexture.Sample(samLinear, float2(input.tex.x*TextureSize.x, input.tex.y*TextureSize.y)).xyz;
		delta = delta * 2.0 - float3(1, 1, 1);
		float3 T = normalize(input.tanVec);
		float3 B = normalize(cross(T, input.normal));
		float3 N  = delta.x*T + delta.y*B + delta.z*input.normal;
		normal = float4(N, 1);
		//normal=NormalTexture.Sample(samLinear, float2(input.tex.x*TextureSize.x, input.tex.y*TextureSize.y));
		/*vec3 delta = texture(utexture_norm, texCoord*vec2(20)).xyz;
		delta = delta * 2.0 - vec3(1, 1, 1);
		vec3 T = normalize(tanVec);
		vec3 B = normalize(cross(T, normalVec));
		N = delta.x*T + delta.y*B + delta.z*normalVec;
		Kd = texture(utexture, texCoord*vec2(20)).rgb;*/
	}
	output.normal = normal;

	float4 diffuse= float4(Diffuse, 1);
	if (TextureIO.x == 1)
	{
		diffuse = DiffuseTexture.Sample(samLinear, float2(input.tex.x*TextureSize.x, input.tex.y*TextureSize.y));
		if (SKYDOME == MaterialID)
		{
			float3 V = input.eyespos.xyz - input.worldpos.xyz;
			float3 N = normalize(V);
			float2 UV = float2(1.0f / 2.0f - atan2(N.y , N.x) / (2.0f * pi),1- acos(N.z) / pi);
			diffuse = DiffuseTexture.Sample(samLinear, UV);
		}
	}
	output.object2 = float4(0, 0, 0, 0);
	if (MaterialID == PARTICLE)
	{
		int id = input.TextureID;
		if (id == 0)
			diffuse = ParticlTexture1.Sample(samLinear, input.tex);
		else if (id == 1)
			diffuse = ParticlTexture2.Sample(samLinear, input.tex);
		else if (id == 2)
			diffuse = ParticlTexture3.Sample(samLinear, input.tex);
		else if (id == 3)
			diffuse = ParticlTexture4.Sample(samLinear, input.tex);
		output.object2 = diffuse;
	}
	if (MaterialID == BUTTONB)
	{
		output.object2 = diffuse;
	}
	if (BULLET == MaterialID)
	{
		output.object2 = normalize(diffuse)/12;
	}
	output.diffuse = diffuse;
	
	if (diffuse.w < 1 && (MaterialID == 2|| MaterialID == 3|| MaterialID == PARTICLE|| MaterialID == BUTTONB|| MaterialID == BUTTON))
	{
		discard;
	}

	float4 specular= float4(Specular.xyz, Shininess);
	if (TextureIO.y == 1)
	{
		specular.xyz = SpecularTexture.Sample(samLinear, float2(input.tex.x*TextureSize.x, input.tex.y*TextureSize.y)).xyz;
	}
	if (TextureIO.z == 1)
	{
		specular.w = ShininessTexture.Sample(samLinear, float2(input.tex.x*TextureSize.x, input.tex.y*TextureSize.y)).x;
	}

	output.specular = specular;
	output. object1 = float4(MaterialID, 0, 0, 0);
	/*output. object2 = float4(input.position.w, input.position.w, input.position.w, 1);*/

	if (MaterialID == FONT)
	{
		float4 color= DiffuseTexture.Sample(samLinear, input.tex);
		output.diffuse = float4(color.x+0.3, color.x + 0.3, color.x + 0.3, 1);
		
		if (color.x < 0.55)
		{
			discard;
		}

	}
	if (OUTLINE == MaterialID)
	{
		output.diffuse = float4(Diffuse,1);
		float r = 0.1f;
		float2 coor = input.tex;
		if (coor.x < r && coor.y < r)
		{
			if (distance(coor, float2(r,r)) > r)
			{
				discard;
			}
		}
		else if (coor.x > 1 - r && coor.y > 1 - r)
		{
			if (distance(coor, float2(1 - r, 1 - r)) > r)
			{
				discard;
			}
		}
		else if (coor.x < r && coor.y >1-  r)
		{
			if (distance(coor, float2(r, 1-r)) > r)
			{
				discard;
			}
		}
		else if (coor.x > 1 - r && coor.y < r)
		{
			if (distance(coor, float2(1 - r, r)) > r)
			{
				discard;
			}
		}
	}
	
	return output;
}
