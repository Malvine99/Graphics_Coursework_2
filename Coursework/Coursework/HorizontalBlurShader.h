#pragma once
#include "BaseShader.h"
class HorizontalBlurShader :
    public BaseShader
{
private:
    struct ScreenSizeBufferType
    {
        float screenWidth;
        XMFLOAT3 padding;
    };

public:
    HorizontalBlurShader(ID3D11Device* device, HWND hwnd);
    ~HorizontalBlurShader();

    void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView* mask, ID3D11ShaderResourceView* scene, float width);

private:
    void initShader(const wchar_t* vs, const wchar_t* ps);

private:
    ID3D11Buffer* matrixBuffer;
    ID3D11SamplerState* sampleState;
    ID3D11Buffer* screenSizeBuffer;

};

