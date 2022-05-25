#pragma once
#include "BaseShader.h"
class HeightDepthShader : public BaseShader
{
public:
	HeightDepthShader(ID3D11Device* device, HWND hwnd);
	~HeightDepthShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView* heightTexture);

private:
	void initShader(const wchar_t* vs, const wchar_t* ps);

	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
};

