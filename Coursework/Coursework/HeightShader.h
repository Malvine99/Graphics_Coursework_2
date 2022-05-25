#pragma once
#include "BaseShader.h"
#include "LightA.h"

#define LIGHTNUM 4


class HeightShader : public BaseShader
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
		XMMATRIX lightView[LIGHTNUM];
		XMMATRIX pointView[6];
		XMMATRIX lightProjection[LIGHTNUM];
	};

	struct LightBufferType
	{
		XMFLOAT4 ambient;
		XMFLOAT4 diffuse[LIGHTNUM];
		XMFLOAT4 position[LIGHTNUM];
		XMFLOAT4 attenuation[LIGHTNUM];
		XMFLOAT3 spotDirection;
		float padding;
	};

public:
	HeightShader(ID3D11Device* device, HWND hwnd);
	~HeightShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView* heightTexture, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* depthMap[], ID3D11ShaderResourceView* pointDepthMap[], LightA* light[]);

private:
	void initShader(const wchar_t* vs, const wchar_t* ps);


	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11SamplerState* sampleStateShadow;
	ID3D11Buffer* lightBuffer;
};

