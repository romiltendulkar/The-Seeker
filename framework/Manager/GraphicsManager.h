/**
 * @file GraphicsManager.h
 * @authors Ching-Yen Lin.
 * @date 9/1/2019
 * @brief This file has methods for rendering data on the screen
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */

#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H
//#define _HAS_STD_BYTE 0
#include "../Source/pch.h"
#include "../Source/Graphics/DirectXHandler.h"
#include "../Source/Graphics/DXShader.h"
#include "../Source/Graphics/Camera.h"
#include "../Source/Graphics/DXObject.h"
#include "../Source/Graphics/DXTexture.h"
#include "../Source/Graphics/Light.h"
#include "../Manager/GameObjectManager.h"
#include "../Source/Graphics/DeferredRendering/DeferredRendering.h"


using namespace std;

struct WorldMatrixBuffer
{
	Matrix WorldView;
	Matrix WorldProj;
	Matrix WorldInverse;
};
struct Particle
{
	Vector4 position;//pos+size
	Vector4 velocity;
	unsigned int TextureIndex;
	float age;
	bool alive;
	int age2;
};
struct ParticleEmitter
{
	Vector4 position;//pos+size
	Vector4 velocity;
	unsigned int TextureIndex;
	unsigned int MaxParticlesNum;
	float age;
	float age1;

};
struct Character {
	DXTexture *Texture;  // ID handle of the glyph texture
	XMINT2 Size;       // Size of glyph
	XMINT2 Bearing;    // Offset from baseline to left/top of glyph
	size_t Advance;    // Offset to advance to next glyph
};
struct GaussianWeights
{
	Vector4 weight[25];
	int w;
	int pad1;
	int pad2;
	int pad3;

};
class Asset;
class Body;
class GraphicsManager
{
public:
	/**
	all pointer point to null and set defualt value
	*/
	GraphicsManager();
	std::unordered_map<wchar_t, Character> Characters;

	/**
	release pointers
	*/
	~GraphicsManager();

	/**
	Initialize directX, shader, buffer and anything need
	*/
	bool Initialize(int screenWidth, int screenHeight, HWND hwnd);
	void Fullscreen(bool b);
	/**
	Initialize directX, shader, buffer and anything need
	*/
	bool InitializeShader();

	/**
	Initialize directX, shader, buffer and anything need
	*/
	bool InitializeDefaultLight();

	/**
Initialize directX, shader, buffer and anything need
*/
	bool InitializeBlur(int wr);
	void Blurring(int w, int h, ID3D11ShaderResourceView*, ID3D11ShaderResourceView**);
	/**
	release all pointer
	*/
	void Shutdown();

	/**
	Render all the things
	*/
	bool Update(float rdeltaTime);

	/**
	Resize screen
	*/
	void Resize(int screenWidth, int screenHeight);

	void Getsize(int& screenWidth, int& screenHeight);
	/**
	set debug mode
	*/
	void SetDebug(bool);

	/**
	get video card's name
	*/
	string GetDeviceName();

	/**
	get DirectXHandler
	*/
	DirectXHandler*GetDirectXHandler();

	/**
	get Manager itself
	*/
	static GraphicsManager* GetInstance();

	/**
	delete Manager itself
	*/
	void Destroy();

	/**
	Set Camera Rotation
	*/
	void SetCameraRotation(float, float, float, float);

	/**
	Set Camera Movement
	*/
	void SetCameraMovement(float, float, float, float);

	/**
	Set Add line for draw
	*/
	void AddDrawingLine(Vector4, Vector4, Vector3 color = Vector3(1,1,1),bool = false);

	/**
	Set Add line for draw
	*/
	void AddDrawingPoint(Vector4,Vector3 color = Vector3(1, 1, 1), bool = true);

	/**
	Set the distance between target and camera position
	*/
	void SetCameraDistance(float);
	void SetCameraShack(float,float);
	/**
	Get camera position
	*/
	void GetCameraPosition(Vector3&);

	/**
	Get camera pointer
	*/
	Camera* GetCamera();

	void SetBlurScreen();

	//https://stackoverflow.com/questions/2573834/c-convert-string-or-char-to-wstring-or-wchar-t
	//for string to wstring

	bool FULL_SCREEN = false;
	const bool VSYNC_ENABLED = true;
	const float SCREEN_FAR = 5000.0f;
	const float SCREEN_NEAR = 0.5f;
	ID3D11Buffer* pWorldMatrixBuff;
	Vector3 RayOrigin;
	Vector3 RayDir;
public:
	ID3D11Buffer* pObjectBuff;
	ID3D11Buffer* pModelBuff;
	ID3D11Buffer* pLightBuff;
	ID3D11Buffer* pBoneBuff;
	ID3D11Buffer* pLightPVBuff;
	ID3D11Buffer* pGWBuff;
private:
	Quad * FrontQ;
	Matrix mDefaultView;
	DXObject* pQuad;
	DXObject* pTestFront;
	DXObject* pDisk;
	DXObject* pSkyDome;
	DXObject* pPoint;
	float dt;
	static GraphicsManager* mInstance;
	struct Line
	{
		Vector4 LineA;
		Vector4 LineB;
		Vector3 Color;
		bool ZBuffer = false;
	};
	queue<Line> LLineQueue;
	struct Point
	{
		Vector4 PointA;
		Vector3 Color;
		bool ZBuffer = true;
	};
	queue<Point> LPointQueue;
	/**
	Render sence
	*/
	void Render();

	/**
	Make Matrix Buffer
	*/
	ID3D11Buffer* MakeMatrixBuffer(D3D11_BUFFER_DESC matrixBuffer);

	/**
	Draw Debug
	*/
	void DrawDebug();

	/**
	Draw line
	*/
	void DrawLine();

	/**
	Draw line
	*/
	void DrawPoint();

	/**
	Draw Bones
	*/
	void DrawBones(Asset*);

	/**
	Draw Mouse
	*/
	void DrawMouse();

	/**
	Draw Bounding Box
	*/
	void DrawBoundingBox(Body*,Vector3);

	/**
	Bind World Matrix to buffer
	*/
	void BindWorldMatrixBuffer(Matrix, Matrix);

	/**
	Bind Light to buffer
	*/
	void BindLightBuffer(Light*);
	void BindLightPVBuffer(LightInform);
	void CreateGaussianWeights(int wr);
	HWND mHWND;
	int mScreenWidth, mScreenHeight;

	//get this pointer
	DirectXHandler* pDirectX;
	//Deferred Rander
	DeferredRendering mDeferredRendering;

	//shaders
	DXShader* pSimplePShader;
	DXShader* pSimpleVShader;
	DXShader* pAnimedVShader;
	DXShader* pShadowPShader;

public:
	DXShader* pParticleVShader;
	DXShader* pParticleGShader;
	DXShader* pParticleCShader;
	DXShader* pParticleEmitCShader;
private:
	DXShader* pBlurHCShader;
	DXShader* pBlurVCShader;

	DXShader* pLightGShader;

	//derred Vertex shader
	DXShader* pDeferredVShader;
	//derred pixel shader Gbuffer pass
	DXShader* pGbufferPShader;
	//derred pixel shader ambient pass
	DXShader* pDSAPShader;
	//derred pixel shader Light pass
	DXShader* pDSLPShader;




	Camera* pCamera;
	DXObject* pObject;

	DXTexture* pTexture;
	DXTexture* pBlurTexture;

	ID3D11SamplerState* pDefaultSamplerState;
	Light* pLight;

	DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* pBrush;
	bool mDebugMode;
	GaussianWeights GW;
public:
	bool mPaused = false;
	bool mCompletion = false;
	bool mNeedCompletion = false;
	Matrix WorldView;
};
#endif // !GRAPHICSMANAGER_H
