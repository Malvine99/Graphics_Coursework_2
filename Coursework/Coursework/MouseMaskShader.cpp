#include "MouseMaskShader.h"

MouseMaskShader::MouseMaskShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	initShader(L"mouse_vs.cso", L"mouse_ps.cso");
}

MouseMaskShader::~MouseMaskShader()
{
	// Release the matrix constant buffer.
	if (matrixBuffer)
	{
		matrixBuffer->Release();
		matrixBuffer = 0;
	}

	// Release the sampler state.
	if (sampleState)
	{
		sampleState->Release();
		sampleState = 0;
	}

	BaseShader::~BaseShader();
}

void MouseMaskShader::setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& worldMatrix, const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix, ID3D11ShaderResourceView* mask, XMFLOAT2 mouse, float radius)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	MouseBufferType* mousePtr;

	// Transpose the matrices to prepare them for the shader.
	XMMATRIX tworld = XMMatrixTranspose(worldMatrix);
	XMMATRIX tview = XMMatrixTranspose(viewMatrix);
	XMMATRIX tproj = XMMatrixTranspose(projectionMatrix);

	// Lock the constant buffer so it can be written to.
	deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	dataPtr = (MatrixBufferType*)mappedResource.pData;
	dataPtr->world = tworld;// worldMatrix;
	dataPtr->view = tview;
	dataPtr->projection = tproj;
	deviceContext->Unmap(matrixBuffer, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &matrixBuffer);

	deviceContext->Map(mouseBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	mousePtr = (MouseBufferType*)mappedResource.pData;
	mousePtr->mousePosition = mouse;
	mousePtr->blurRadius = radius;
	mousePtr->padding = 0;
	deviceContext->Unmap(mouseBuffer, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &mouseBuffer);

	// Set shader texture and sampler resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &mask);
	deviceContext->PSSetSamplers(0, 1, &sampleState);

}

void MouseMaskShader::initShader(const wchar_t* vsFilename, const wchar_t* psFilename)
{
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC mouseBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;

	// Load (+ compile) shader files
	loadVertexShader(vsFilename);
	loadPixelShader(psFilename);

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;
	renderer->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);

	mouseBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	mouseBufferDesc.ByteWidth = sizeof(MouseBufferType);
	mouseBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	mouseBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	mouseBufferDesc.MiscFlags = 0;
	mouseBufferDesc.StructureByteStride = 0;
	renderer->CreateBuffer(&mouseBufferDesc, NULL, &mouseBuffer);

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	renderer->CreateSamplerState(&samplerDesc, &sampleState);

}
