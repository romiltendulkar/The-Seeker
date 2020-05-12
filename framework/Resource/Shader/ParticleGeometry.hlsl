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
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 worldpos : WORLDPOS;
	float3 eyespos : EYESPOS;
	float3 tanVec : TANVEC;
	int TextureID : TEXTUREID;
};
struct VoutputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD;
	float3 normal : NORMAL;
	float4 worldpos : WORLDPOS;
	float3 eyespos : EYESPOS;
	bool draw : DRAW;
	int TextureID : TEXTUREID;
	float2 size:SIZE;
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
[maxvertexcount(4)]
void main(point VoutputType input[1], inout TriangleStream<PixelInputType> OutputStream)
{
/*	1*****3
	*    **
	*  *  *
	* *   *
	**    *
	2*****4
	*/
	if(input[0].draw!=true)
	{
		return;
	}
	float3 planeNormal = input[0].worldpos.xyz - input[0].eyespos;
	planeNormal.z = 0.0f;
	planeNormal = normalize(planeNormal);

	float3 CameraRight_worldspace = float3( WorldView[0][0], WorldView[1][0], WorldView[2][0] );
	float3 CameraUp_worldspace = float3( WorldView[0][1], WorldView[1][1], WorldView[2][1] );

	float4 vert[4];
	vert[0] = input[0].worldpos
		- float4(CameraRight_worldspace  * 0.5, 0)*input[0].size.x
		+ float4(CameraUp_worldspace * 0.5, 0)*input[0].size.y;
	vert[1] = input[0].worldpos 
		- float4(CameraRight_worldspace * 0.5, 0)*input[0].size.x
		- float4(CameraUp_worldspace  * 0.5, 0)*input[0].size.y;
	vert[2] = input[0].worldpos
		+ float4(CameraRight_worldspace * 0.5, 0)*input[0].size.x
		+ float4(CameraUp_worldspace  * 0.5, 0)*input[0].size.y;
	vert[3] = input[0].worldpos
		+ float4(CameraRight_worldspace  * 0.5, 0)*input[0].size.x
		- float4(CameraUp_worldspace  * 0.5,0)*input[0].size.y;

	float2 texCoord[4];
	texCoord[0] = float2(0, 0);
	texCoord[1] = float2(0, 1);
	texCoord[2] = float2(1, 0);
	texCoord[3] = float2(1, 1);
	for (int i = 0; i < 4; i++)
	{
		PixelInputType temp1;
		temp1.position = mul(mul(float4(vert[i].xyz,1), WorldView), WorldProj);
		temp1.tex =  texCoord[i];
		temp1.normal =  float3(0, 0, 0);
		temp1.worldpos= mul(float4(vert[i].xyz, 1), ModelTr);
		temp1.eyespos = float3(0, 0, 0);
		temp1.TextureID = input[0].TextureID;
		temp1.tanVec = float3(0, 0, 0);
		OutputStream.Append(temp1);
	}

}