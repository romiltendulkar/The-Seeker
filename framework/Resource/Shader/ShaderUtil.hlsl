//shader util function
static const float pi = 3.14159265f;
static const float pi2 = 6.2831853f;
static const int GBUFFER_SIZE = 6;
static const int DEBUG_LINE = -1;
static const int NORMALOBJECT = 0;
static const int SKYDOME = 1;
static const int HASALPHA = 3;
static const int FONT = 4;
static const int OUTLINE = 5;
static const int PARTICLE = 6;
static const int BULLET = 7;
static const int BUTTON = 8;
static const int BUTTONB = 9;

SamplerState samLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct LightInfo
{
	float4 Attribute;
	float3 Position;
	float CutOff;
	float3 Direction;
	float OuterCutOff;
	float3 Ambient;
	float Constant;
	float3 Diffuse;
	float Linear;
	float3 Specular;
	float Quadratic;
	matrix shadowMartix;
};

float4 Phong(LightInfo light,float3 N,float3 L,float3 V,float3 kd,float3 ks,float alpha)
{
	float3 lightdir=L;
	if(light.Attribute.x==0||light.Attribute.x==2)
	{
		lightdir=-light.Direction;
	}
	float3 output=float3(0,0,0);
	float3 H = normalize(lightdir+V);
	float HN = saturate(dot(H,N));
	float NL = saturate(dot(N,lightdir));
	return float4(light.Diffuse*kd*NL+light.Specular*ks*pow(HN,alpha),1); 
}


float3 Fterm(float3 Ks,float LH)
{
	return Ks +((float3(1,1,1)-Ks))*pow((1-LH),5);
}
float Gterm(float LH)
{
	return 1.0f /  pow(LH,2);
}
float Dterm(float HN, float shininess)
{
	return ((shininess + 2.0f) / pi2) * pow(HN, shininess);
} 

float4 BRDF(LightInfo light,float3 N,float3 L,float3 V,float3 kd,float3 ks,float alpha)
{
	float3 I = float3(1,1,1);
	float3 lightdir=L;
	if(light.Attribute.x==0||light.Attribute.x==2)
	{
		lightdir=-light.Direction;
	}
	float3 H = normalize(lightdir+V);
	float LH = saturate(dot(lightdir,H));
	float NL = saturate(dot(N,lightdir));
	float HN = saturate(dot(H,N));
	float3 F = Fterm(ks/40,LH);
	float G = Gterm(LH);
	float D = Dterm(HN, alpha);//?
	//diffuse/pi is Diffuse
	//F*G*D/4 is Specular
	float3 BRDF = light.Diffuse*kd/pi + light.Specular*F*G*D/4.0f;
	return float4(I * NL * BRDF,1);
}

float3 ToneMapping(float3 C)
{
	return pow(float3(1,1,1)*C
		/ (float3(1,1,1)*C + float3(1, 1, 1)),
		float3(1 / 2.2, 1 / 2.2, 1 / 2.2));
}
float3 sRGB_to_linear(float3 C)
{
	return pow(C, float3(2.2, 2.2, 2.2));
}
float3 linear_to_sRGB(float3 C)
{
	return pow(C, float3(1 / 2.2, 1 / 2.2, 1 / 2.2));
}