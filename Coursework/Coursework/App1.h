// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"
#include "TextureShader.h"
#include "LightShader.h"
#include "HeightShader.h"
#include "NormalShader.h"
#include "DepthShader.h"
#include "ShadowShader.h"
#include "HeightDepthShader.h"
#include "MouseMaskShader.h"
#include "HorizontalBlurShader.h"
#include "VerticalBlurShader.h"
#include "NormalDepthShader.h"

class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN);

	bool frame();

protected:
	bool render();
	void depthPass(int numPass);
	void firstRender();
	void maskPass();
	void horizontalBlurPass();
	void verticalBlurPass();
	void finalPass();
	void gui();

private:

	PlaneMesh* planeMesh;
	CubeMesh* cubeMesh;
	SphereMesh* sphereMesh;
	OrthoMesh* depthMapOrtho;
	OrthoMesh* blurOrtho;
	//main rendertexture for final pass
	RenderTexture* renderTexture;
	//intermediate rendertextures
	RenderTexture *mouseMask1, *mouseMask2, *verticalBlurTex, *horizontalBlurTex;
	bool flipMask, shouldBlur;

	TextureShader* textureShader;
	LightShader* lightShader;
	HeightShader* heightShader;
	NormalShader* normalShader;
	DepthShader* depthShader;
	ShadowShader* shadowShader;
	HeightDepthShader* heightDepthShader;
	MouseMaskShader* mouseShader;
	HorizontalBlurShader* horizontalShader;
	VerticalBlurShader* verticalShader;
	NormalDepthShader* normalDepthShader;

	//size is number of lights
	ShadowMap* shadowMap[LIGHTNUM];
	//array of shadowmaps for point lights. size is 6x number of spot lights
	ShadowMap* pointShadowMap[6];

	LightA* lights[LIGHTNUM];
	XMFLOAT4 lightColours[LIGHTNUM];
	XMFLOAT4 spotAtten;
	XMFLOAT3 pointPos, spotPos, spotDir;
	XMFLOAT3 cubePos[2], wallScale;

	bool lightsON[LIGHTNUM];
	bool normals;
	int chooseDepth;
	float mouseRadius;
};

#endif