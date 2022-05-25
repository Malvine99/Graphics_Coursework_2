#pragma once
#include "BaseShader.h"

class NormalDepthShader : public BaseShader
{
public:
	NormalDepthShader(ID3D11Device* device, HWND hwnd);
	~NormalDepthShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView* heightTexture);

private:
	void initShader(const wchar_t* vs, const wchar_t* gs, const wchar_t* ps);
	void initShader(const wchar_t* vs, const wchar_t* ps);

	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
};

