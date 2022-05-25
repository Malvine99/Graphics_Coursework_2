#include "HeightShader.h"

HeightShader::HeightShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	initShader(L"height_vs.cso", L"shadow_ps.cso");
}

HeightShader::~HeightShader()
{
	// Release the sampler state.
	if (sampleState)
	{
		sampleState->Release();
		sampleState = 0;
	}

	// Release the matrix constant buffer.
	if (matrixBuffer)
	{
		matrixBuffer->Release();
		matrixBuffer = 0;
	}

	// Release the layout.
	if (layout)
	{
		layout->Release();
		layout = 0;
	}
	// Release the light constant buffer.
	if (lightBuffer)
	{
		lightBuffer->Release();
		lightBuffer = 0;
	}

	//Release base shader components
	BaseShader::~BaseShader();
}

void HeightShader::setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& worldMatrix, const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix, ID3D11ShaderResourceView* heightTexture, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* depthMap[], ID3D11ShaderResourceView* pointDepthMap[], LightA* light[])
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	XMMATRIX tworld, tview, tproj;


	// Transpose the matrices to prepare them for the shader.
	tworld = XMMatrixTranspose(worldMatrix);
	tview = XMMatrixTranspose(viewMatrix);
	tproj = XMMatrixTranspose(projectionMatrix);
	XMMATRIX tLightViewMatrix[LIGHTNUM];
	XMMATRIX tLightProjectionMatrix[LIGHTNUM];
	for (int i = 0; i < LIGHTNUM; i++) {
		tLightViewMatrix[i] = XMMatrixTranspose(light[i]->getViewMatrix());
		if (light[i]->getType() == 1) {
			tLightProjectionMatrix[i] = XMMatrixTranspose(light[i]->getOrthoMatrix());
		}
		else {
			tLightProjectionMatrix[i] = XMMatrixTranspose(light[i]->getProjectionMatrix());
		}
	}
	XMMATRIX tPointViewMatrix[6];
	for (int i = 0; i < 6; i++) {
		light[1]->generatePointViewMatrix(i);
		tPointViewMatrix[i] = XMMatrixTranspose(light[1]->getViewMatrix());
	}

	// Send matrix data
	result = deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	dataPtr = (MatrixBufferType*)mappedResource.pData;
	dataPtr->world = tworld;// worldMatrix;
	dataPtr->view = tview;
	dataPtr->projection = tproj;
	for (int i = 0; i < LIGHTNUM; i++) {
		dataPtr->lightView[i] = tLightViewMatrix[i];
		dataPtr->lightProjection[i] = tLightProjectionMatrix[i];
	}
	for (int i = 0; i < 6; i++) {
		dataPtr->pointView[i] = tPointViewMatrix[i];
	}

	deviceContext->Unmap(matrixBuffer, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &matrixBuffer);
	deviceContext->VSSetShaderResources(0, 1, &heightTexture);
	deviceContext->VSSetSamplers(0, 1, &sampleState);


	deviceContext->GSSetConstantBuffers(0, 1, &matrixBuffer);


	// Set shader texture and sampler resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);
	for (int i = 0; i < LIGHTNUM; i++) {
		deviceContext->PSSetShaderResources(i + 1, 1, &depthMap[i]);
	}
	for (int i = 0; i < 6; i++) {
		deviceContext->PSSetShaderResources(i + 5, 1, &pointDepthMap[i]);
	}
	deviceContext->PSSetSamplers(0, 1, &sampleState);
	deviceContext->PSSetSamplers(1, 1, &sampleStateShadow);

	// Send light data to pixel shader
	LightBufferType* lightPtr;
	deviceContext->Map(lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	lightPtr = (LightBufferType*)mappedResource.pData;
	lightPtr->ambient = light[0]->getAmbientColour();
	lightPtr->diffuse[0] = light[0]->getDiffuseColour();
	lightPtr->position[0].x = light[0]->getDirection().x;
	lightPtr->position[0].y = light[0]->getDirection().y;
	lightPtr->position[0].z = light[0]->getDirection().z;
	lightPtr->position[0].w = light[0]->getType();
	for (int i = 1; i < LIGHTNUM; i++) {
		lightPtr->diffuse[i] = light[i]->getDiffuseColour();
		lightPtr->position[i].x = light[i]->getPosition().x;
		lightPtr->position[i].y = light[i]->getPosition().y;
		lightPtr->position[i].z = light[i]->getPosition().z;
		lightPtr->position[i].w = light[i]->getType();
		lightPtr->attenuation[i] = light[i]->getAttenuation();;
	}
	lightPtr->spotDirection = light[2]->getDirection();
	lightPtr->padding = 0.0f;

	deviceContext->Unmap(lightBuffer, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &lightBuffer);

}

void HeightShader::initShader(const wchar_t* vsFilename, const wchar_t* psFilename)
{

	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC lightBufferDesc;

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

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	renderer->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);

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

	// Sampler for shadow map sampling.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.BorderColor[0] = 1.0f;
	samplerDesc.BorderColor[1] = 1.0f;
	samplerDesc.BorderColor[2] = 1.0f;
	samplerDesc.BorderColor[3] = 1.0f;
	renderer->CreateSamplerState(&samplerDesc, &sampleStateShadow);

	// Setup light buffer
	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;
	renderer->CreateBuffer(&lightBufferDesc, NULL, &lightBuffer);

}
