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
	matrix PathTr;
};
cbuffer LightInform : register(b5)
{
	matrix LightProjView1;
	matrix LightProjView2;
	matrix LightProjView3;
	matrix LightProjView4;
};


//////////////
// TYPEDEFS //
//////////////

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
[maxvertexcount(12)]
void main(triangle PixelInputType input[3], inout TriangleStream<PixelInputType> OutputStream)
{
	for (int i = 0; i < 3; i++)
	{
		PixelInputType temp = input[i];
		temp.worldpos = mul(LightProjView1, temp.worldpos);
		OutputStream.Append(temp);
	}

}