#pragma once
#include "BaseShader.h"

class MouseMaskShader :
    public BaseShader
{
private:
    struct MouseBufferType {
        XMFLOAT2 mousePosition;
        float blurRadius;
        float padding;
    };

public: 
    MouseMaskShader(ID3D11Device* device, HWND hwnd);
    ~MouseMaskShader();

    void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView* mask, XMFLOAT2 mouse, float radius);

private:
    void initShader(const wchar_t* vs, const wchar_t* ps);

private:
    ID3D11Buffer* matrixBuffer;
    ID3D11Buffer* mouseBuffer;
    ID3D11SamplerState* sampleState;
};

