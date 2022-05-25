#pragma once
#include "BaseShader.h"
class VerticalBlurShader :
    public BaseShader
{
private:
    struct ScreenSizeBufferType
    {
        float screenHeight;
        XMFLOAT3 padding;
    };

public:
    VerticalBlurShader(ID3D11Device* device, HWND hwnd);
    ~VerticalBlurShader();

    void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView* mask, ID3D11ShaderResourceView* scene, float width);

private:
    void initShader(const wchar_t* vs, const wchar_t* ps);

private:
    ID3D11Buffer* matrixBuffer;
    ID3D11SamplerState* sampleState;
    ID3D11Buffer* screenSizeBuffer;
};

