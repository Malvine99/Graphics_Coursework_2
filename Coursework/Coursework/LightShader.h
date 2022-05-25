#pragma once

#include "DXF.h"
#include "LightA.h"

using namespace std;
using namespace DirectX;

#define LIGHTNUM 4

class LightShader : public BaseShader
{
private:
	struct LightBufferType
	{
		XMFLOAT4 ambient;
		XMFLOAT4 diffuse[LIGHTNUM];
		XMFLOAT4 position[LIGHTNUM];
		XMFLOAT4 attenuation[LIGHTNUM];
	};

public:
	LightShader(ID3D11Device* device, HWND hwnd);
	~LightShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView* texture, LightA* light[]);

private:
	void initShader(const wchar_t* vs, const wchar_t* ps);

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* lightBuffer;
};

