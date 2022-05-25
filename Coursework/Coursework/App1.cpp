// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App1.h"

App1::App1()
{

}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in, bool VSYNC, bool FULL_SCREEN)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in, VSYNC, FULL_SCREEN);

	// Initalise scene variables.
	textureMgr->loadTexture(L"height", L"res/heightmap.png");
	textureMgr->loadTexture(L"wood", L"res/wood.png");
	textureMgr->loadTexture(L"rock", L"res/rock.jpg");
	textureMgr->loadTexture(L"paint", L"res/paint.jpg");
	//Geometry
	planeMesh = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext());
	cubeMesh = new CubeMesh(renderer->getDevice(), renderer->getDeviceContext());
	sphereMesh = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext());
	depthMapOrtho = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), screenWidth / 4, screenHeight / 4, -screenWidth / 2.7, screenHeight / 2.7);
	blurOrtho = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), screenWidth, screenHeight);
	//initialise RTTs
	renderTexture = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	mouseMask1 = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	mouseMask2 = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	verticalBlurTex = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	horizontalBlurTex = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	flipMask = false;
	shouldBlur = false;

	//initialise Shadowmaps
	int shadowmapWidth = 1024;
	int shadowmapHeight = 1024;

	for (int i = 0; i < LIGHTNUM; i++) {
		shadowMap[i] = new ShadowMap(renderer->getDevice(), shadowmapWidth, shadowmapHeight);
	}
	for (int i = 0; i < 6; i++) {
		pointShadowMap[i] = new ShadowMap(renderer->getDevice(), shadowmapWidth, shadowmapHeight);
	}

	//Shaders
	textureShader = new TextureShader(renderer->getDevice(), hwnd);
	lightShader = new LightShader(renderer->getDevice(), hwnd);
	heightShader = new HeightShader(renderer->getDevice(), hwnd);
	normalShader = new NormalShader(renderer->getDevice(), hwnd);
	depthShader = new DepthShader(renderer->getDevice(), hwnd);
	shadowShader = new ShadowShader(renderer->getDevice(), hwnd);
	heightDepthShader = new HeightDepthShader(renderer->getDevice(), hwnd);
	mouseShader = new MouseMaskShader(renderer->getDevice(), hwnd);
	horizontalShader = new HorizontalBlurShader(renderer->getDevice(), hwnd);
	verticalShader = new VerticalBlurShader(renderer->getDevice(), hwnd);
	normalDepthShader = new NormalDepthShader(renderer->getDevice(), hwnd);

	//initialise Lights
	#pragma region lightsInit
	int shadowSceneWidth = 100;
	int shadowSceneHeight = 100;
	lightColours[0] = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	lightColours[1] = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	lightColours[2] = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	lightColours[3] = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	lights[0] = new LightA();
	lights[0]->setType(1.f);
	lights[0]->setAmbientColour(0.f, 0.f, 0.f, 1.0f);
	lights[0]->setDiffuseColour(lightColours[0]);
	lights[0]->setPosition(0.f, 10.f, 50.f);
	lights[0]->setDirection(0.7f, -0.2f, 0.0f);
	lights[0]->setAttenuation(0.f, 0.f, 0.f, 0.f);
	lights[0]->generateOrthoMatrix((float)shadowSceneWidth, (float)shadowSceneHeight, 0.1f, 100.f);

	lights[1] = new LightA();
	lights[1]->setType(0.f);
	lights[1]->setAmbientColour(0.0f, 0.0f, 0.0f, 1.0f);
	lights[1]->setDiffuseColour(lightColours[1]);
	lights[1]->setPosition(45.0f, 17.0f, 30.0f);
	lights[1]->setAttenuation( 0.5f, 0.2f, 0.0f, 0.0f );
	lights[1]->generateProjectionMatrix(1.f, 100.f);

	lights[2] = new LightA();
	lights[2]->setType(2.f);
	lights[2]->setDirection(0.f, -1.f, 0.f);
	lights[2]->setAmbientColour(0.0f, 0.0f, 0.0f, 1.0f);
	lights[2]->setDiffuseColour(lightColours[2]);
	lights[2]->setPosition(45.0f, 10.0f, 20.0f);
	lights[2]->setAttenuation( 0.5f, 0.0f, 0.0f, 2.3f );
	lights[2]->generateProjectionMatrix(1.f, 100.f);

	lights[3] = new LightA();
	lights[3]->setType(2.f);
	lights[3]->setDirection(0.f, -1.f, 0.f);
	lights[3]->setAmbientColour(0.0f, 0.0f, 0.0f, 1.0f);
	lights[3]->setDiffuseColour(lightColours[3]);
	lights[3]->setPosition(60.0f, 10.0f, 40.0f);
	lights[3]->setAttenuation(0.001f, 0.0f, 0.0f, 2.8f);
	lights[3]->generateProjectionMatrix(1.f, 100.f);

	//initialise Gui handlers
	spotAtten.x = lights[1]->getAttenuation().x;
	spotAtten.y = lights[1]->getAttenuation().y;
	spotAtten.z = lights[1]->getAttenuation().z;
	spotAtten.w = lights[3]->getAttenuation().w;

	pointPos = lights[1]->getPosition();
	spotPos = lights[3]->getPosition();
	spotDir.x = 0;
	spotDir.y = -1;
	spotDir.z = 0;

	for (int i = 0; i < LIGHTNUM; i++) {
		lightsON[i] = true;
	}
	normals = false;
	chooseDepth = 0;
	#pragma endregion lightsInit

	mouseRadius = 10;
	//initialise object positions
	wallScale = XMFLOAT3(10, 10, 0.5);
	cubePos[0] = XMFLOAT3(50.0f, 5.0f, 35.0f);
	cubePos[1] = XMFLOAT3(50.f/wallScale.x, 10.f / wallScale.y, 25.f / wallScale.z);
}

App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.
	//Geometry
	if (planeMesh) {
		delete planeMesh;
		planeMesh = NULL;
	}
	if (cubeMesh) {
		delete cubeMesh;
		cubeMesh = NULL;
	}	
	if (sphereMesh) {
		delete sphereMesh;
		sphereMesh = NULL;
	}
	//Shaders
	if (textureShader) {
		delete textureShader;
		textureShader = NULL;
	}
	if (lightShader) {
		delete lightShader;
		lightShader = NULL;
	}
	if (heightShader) {
		delete heightShader;
		heightShader = NULL;
	}
	if (normalShader) {
		delete normalShader;
		normalShader = NULL;
	}
	if (depthShader) {
		delete depthShader;
		depthShader = NULL;
	}
	if (shadowShader) {
		delete shadowShader;
		shadowShader = NULL;
	}	
	if (heightDepthShader) {
		delete heightDepthShader;
		heightDepthShader = NULL;
	}
	if (mouseShader) {
		delete mouseShader;
		mouseShader = NULL;
	}
	if (horizontalShader) {
		delete horizontalShader;
		horizontalShader = NULL;
	}
	if (verticalShader) {
		delete verticalShader;
		verticalShader = NULL;
	}
	if (normalDepthShader) {
		delete normalDepthShader;
		normalDepthShader = NULL;
	}
	//Lights
	for (int i = 0; i < LIGHTNUM; i++) {
		if (lights[i]) {
			delete lights[i];
			lights[i] = NULL;
		}
	}
}

bool App1::frame()
{
	bool result;

	result = BaseApplication::frame();
	if (!result)
	{
		return false;
	}

	//turn lights on or off
	for (int i = 0; i < LIGHTNUM; i++) {
		if (!lightsON[i]) {
			lights[i]->setDiffuseColour(0.f, 0.f, 0.f, 1.f);
		}
		else {
			lights[i]->setDiffuseColour(lightColours[i]);
		}
	}
	// Render the graphics.
	result = render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool App1::render()
{
	for (int i = 0; i < LIGHTNUM; i++) {
		depthPass(i);
	}
	firstRender();
	if (shouldBlur) {
		maskPass();
		horizontalBlurPass();
		verticalBlurPass();
	}
	finalPass();
	return true;
}

void App1::depthPass(int numPass)
{
	if (lights[numPass]->getType() == 0) {
		//if light is point light, loop for each matrix.
		for (int i = 0; i < 6; i++) {
			pointShadowMap[i]->BindDsvAndSetNullRenderTarget(renderer->getDeviceContext());

			lights[numPass]->generatePointViewMatrix(i);
			XMMATRIX lightViewMatrix = lights[numPass]->getViewMatrix();
			XMMATRIX lightProjectionMatrix = lights[numPass]->getProjectionMatrix();
			XMMATRIX worldMatrix = renderer->getWorldMatrix();

			planeMesh->sendData(renderer->getDeviceContext());
			heightDepthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix, textureMgr->getTexture(L"height"));
			heightDepthShader->render(renderer->getDeviceContext(), planeMesh->getIndexCount());

			if (normals) {
				planeMesh->sendData(renderer->getDeviceContext());
				normalDepthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix, textureMgr->getTexture(L"height"));
				normalDepthShader->render(renderer->getDeviceContext(), planeMesh->getIndexCount());
			}

			XMMATRIX cubeWorld = worldMatrix;
			cubeWorld = XMMatrixTranslation(cubePos[0].x, cubePos[0].y, cubePos[0].z);
			cubeMesh->sendData(renderer->getDeviceContext());
			depthShader->setShaderParameters(renderer->getDeviceContext(), cubeWorld, lightViewMatrix, lightProjectionMatrix);
			depthShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());

			cubeWorld = worldMatrix;
			cubeWorld = XMMatrixTranslation(cubePos[1].x, cubePos[1].y, cubePos[1].z);
			XMMATRIX scaleMatrix = XMMatrixScaling(wallScale.x, wallScale.y, wallScale.z);
			cubeWorld = XMMatrixMultiply(cubeWorld, scaleMatrix);
			cubeMesh->sendData(renderer->getDeviceContext());
			depthShader->setShaderParameters(renderer->getDeviceContext(), cubeWorld, lightViewMatrix, lightProjectionMatrix);
			depthShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());


			// Set back buffer as render target and reset view port.
			renderer->setBackBufferRenderTarget();
			renderer->resetViewport();
		}
	}
	else if (lights[numPass]->getType() == 1) {
		//directional light
		shadowMap[numPass]->BindDsvAndSetNullRenderTarget(renderer->getDeviceContext());

		lights[numPass]->generateViewMatrix();
		XMMATRIX lightViewMatrix = lights[numPass]->getViewMatrix();
		XMMATRIX lightProjectionMatrix = lights[numPass]->getOrthoMatrix();
		XMMATRIX worldMatrix = renderer->getWorldMatrix();

		planeMesh->sendData(renderer->getDeviceContext());
		heightDepthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix, textureMgr->getTexture(L"height"));
		heightDepthShader->render(renderer->getDeviceContext(), planeMesh->getIndexCount());

		if (normals) {
			planeMesh->sendData(renderer->getDeviceContext());
			normalDepthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix, textureMgr->getTexture(L"height"));
			normalDepthShader->render(renderer->getDeviceContext(), planeMesh->getIndexCount());
		}

		XMMATRIX cubeWorld = worldMatrix;
		cubeWorld = XMMatrixTranslation(cubePos[0].x, cubePos[0].y, cubePos[0].z);
		cubeMesh->sendData(renderer->getDeviceContext());
		depthShader->setShaderParameters(renderer->getDeviceContext(), cubeWorld, lightViewMatrix, lightProjectionMatrix);
		depthShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());

		cubeWorld = worldMatrix;
		cubeWorld = XMMatrixTranslation(cubePos[1].x, cubePos[1].y, cubePos[1].z);
		XMMATRIX scaleMatrix = XMMatrixScaling(wallScale.x, wallScale.y, wallScale.z);
		cubeWorld = XMMatrixMultiply(cubeWorld, scaleMatrix);
		cubeMesh->sendData(renderer->getDeviceContext());
		depthShader->setShaderParameters(renderer->getDeviceContext(), cubeWorld, lightViewMatrix, lightProjectionMatrix);
		depthShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());

		// Set back buffer as render target and reset view port.
		renderer->setBackBufferRenderTarget();
		renderer->resetViewport();
	}
	else {
		//spot light
		shadowMap[numPass]->BindDsvAndSetNullRenderTarget(renderer->getDeviceContext());

		lights[numPass]->setDirection(spotDir.x, spotDir.y, spotDir.z);
		lights[numPass]->generateViewMatrix();
		XMMATRIX lightViewMatrix = lights[numPass]->getViewMatrix();
		XMMATRIX lightProjectionMatrix = lights[numPass]->getProjectionMatrix();
		XMMATRIX worldMatrix = renderer->getWorldMatrix();

		planeMesh->sendData(renderer->getDeviceContext());
		heightDepthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix, textureMgr->getTexture(L"height"));
		heightDepthShader->render(renderer->getDeviceContext(), planeMesh->getIndexCount());

		if (normals) {
			planeMesh->sendData(renderer->getDeviceContext());
			normalDepthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix, textureMgr->getTexture(L"height"));
			normalDepthShader->render(renderer->getDeviceContext(), planeMesh->getIndexCount());
		}

		XMMATRIX cubeWorld = worldMatrix;
		cubeWorld = XMMatrixTranslation(cubePos[0].x, cubePos[0].y, cubePos[0].z);
		cubeMesh->sendData(renderer->getDeviceContext());
		depthShader->setShaderParameters(renderer->getDeviceContext(), cubeWorld, lightViewMatrix, lightProjectionMatrix);
		depthShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());

		cubeWorld = worldMatrix;
		cubeWorld = XMMatrixTranslation(cubePos[1].x, cubePos[1].y, cubePos[1].z);
		XMMATRIX scaleMatrix = XMMatrixScaling(wallScale.x, wallScale.y, wallScale.z);
		cubeWorld = XMMatrixMultiply(cubeWorld, scaleMatrix);
		cubeMesh->sendData(renderer->getDeviceContext());
		depthShader->setShaderParameters(renderer->getDeviceContext(), cubeWorld, lightViewMatrix, lightProjectionMatrix);
		depthShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());

		// Set back buffer as render target and reset view port.
		renderer->setBackBufferRenderTarget();
		renderer->resetViewport();

	}
}

void App1::firstRender()
{
	if (!shouldBlur) {
		mouseMask1->clearRenderTarget(renderer->getDeviceContext(), 0.f, 0.f, 0.f, 1.f);
		mouseMask2->clearRenderTarget(renderer->getDeviceContext(), 0.f, 0.f, 0.f, 1.f);
	}
	renderTexture->setRenderTarget(renderer->getDeviceContext());
	renderTexture->clearRenderTarget(renderer->getDeviceContext(), 0.f, 0.5f, 0.4f, 1.f);

	// Generate the view matrix based on the camera's position.
	camera->update();
	lights[1]->setAttenuation(spotAtten.x, spotAtten.y, spotAtten.z, 0.f);
	lights[1]->setPosition(pointPos);
	lights[2]->setAttenuation(spotAtten.x, spotAtten.y, spotAtten.z, spotAtten.w);
	lights[3]->setAttenuation(lights[3]->getAttenuation().x, 0.f, 0.f, spotAtten.w);
	lights[3]->setPosition(spotPos);

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX viewMatrix = camera->getViewMatrix();
	XMMATRIX projectionMatrix = renderer->getProjectionMatrix();
	//put depthmaps into array
	ID3D11ShaderResourceView* depthMaps[LIGHTNUM];
	for (int i = 0; i < LIGHTNUM; i++) {
		depthMaps[i] = shadowMap[i]->getDepthMapSRV();
	}
	ID3D11ShaderResourceView* pointDepthMaps[6];
	for (int i = 0; i < 6; i++) {
		pointDepthMaps[i] = pointShadowMap[i]->getDepthMapSRV();
	}

	planeMesh->sendData(renderer->getDeviceContext());
	heightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"height"), textureMgr->getTexture(L"rock"), depthMaps, pointDepthMaps, lights);
	heightShader->render(renderer->getDeviceContext(), planeMesh->getIndexCount());

	if (normals) {
		planeMesh->sendData(renderer->getDeviceContext());
		normalShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"height"), textureMgr->getTexture(L"paint"), depthMaps, pointDepthMaps, lights);
		normalShader->render(renderer->getDeviceContext(), planeMesh->getIndexCount());
	}

	XMMATRIX cubeWorld = worldMatrix;
	cubeWorld = XMMatrixTranslation(cubePos[0].x, cubePos[0].y, cubePos[0].z);
	cubeMesh->sendData(renderer->getDeviceContext());
	shadowShader->setShaderParameters(renderer->getDeviceContext(), cubeWorld, viewMatrix, projectionMatrix, textureMgr->getTexture(L"wood"), depthMaps, pointDepthMaps, lights);
	shadowShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());

	cubeWorld = worldMatrix;
	cubeWorld = XMMatrixTranslation(cubePos[1].x, cubePos[1].y, cubePos[1].z);
	XMMATRIX scaleMatrix = XMMatrixScaling(wallScale.x, wallScale.y, wallScale.z);
	cubeWorld = XMMatrixMultiply(cubeWorld, scaleMatrix);
	cubeMesh->sendData(renderer->getDeviceContext());
	shadowShader->setShaderParameters(renderer->getDeviceContext(), cubeWorld, viewMatrix, projectionMatrix, textureMgr->getTexture(L"wood"), depthMaps, pointDepthMaps, lights);
	shadowShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());

	XMMATRIX sphereWorld = worldMatrix;
	sphereWorld = XMMatrixTranslation(lights[1]->getPosition().x, lights[1]->getPosition().y, lights[1]->getPosition().z);
	sphereMesh->sendData(renderer->getDeviceContext());
	lightShader->setShaderParameters(renderer->getDeviceContext(), sphereWorld, viewMatrix, projectionMatrix, textureMgr->getTexture(L"wood"), lights);
	lightShader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount());

	sphereWorld = worldMatrix;
	sphereWorld = XMMatrixTranslation(lights[2]->getPosition().x, lights[2]->getPosition().y, lights[2]->getPosition().z);
	sphereMesh->sendData(renderer->getDeviceContext());
	lightShader->setShaderParameters(renderer->getDeviceContext(), sphereWorld, viewMatrix, projectionMatrix, textureMgr->getTexture(L"wood"), lights);
	lightShader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount());

	sphereWorld = worldMatrix;
	sphereWorld = XMMatrixTranslation(lights[3]->getPosition().x, lights[3]->getPosition().y, lights[3]->getPosition().z);
	sphereMesh->sendData(renderer->getDeviceContext());
	lightShader->setShaderParameters(renderer->getDeviceContext(), sphereWorld, viewMatrix, projectionMatrix, textureMgr->getTexture(L"wood"), lights);
	lightShader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount());
	
	// Reset the render target back to the original back buffer and not the render to texture anymore.
	renderer->setBackBufferRenderTarget();
}

void App1::maskPass()
{
	if (flipMask) {
		mouseMask2->setRenderTarget(renderer->getDeviceContext());

		renderer->setZBuffer(false);
		XMMATRIX worldMatrix = renderer->getWorldMatrix();
		XMMATRIX orthoMatrix = renderer->getOrthoMatrix();			// ortho matrix for 2D rendering
		XMMATRIX orthoViewMatrix = camera->getOrthoViewMatrix();	// Default camera position for orthographic rendering

		blurOrtho->sendData(renderer->getDeviceContext());
		mouseShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix, mouseMask1->getShaderResourceView(), XMFLOAT2(input->getMouseX(), input->getMouseY()), mouseRadius);
		mouseShader->render(renderer->getDeviceContext(), blurOrtho->getIndexCount());

		renderer->setZBuffer(true);

		renderer->setBackBufferRenderTarget();
	}
	else {
		mouseMask1->setRenderTarget(renderer->getDeviceContext());

		renderer->setZBuffer(false);
		XMMATRIX worldMatrix = renderer->getWorldMatrix();
		XMMATRIX orthoMatrix = renderer->getOrthoMatrix();			// ortho matrix for 2D rendering
		XMMATRIX orthoViewMatrix = camera->getOrthoViewMatrix();	// Default camera position for orthographic rendering

		blurOrtho->sendData(renderer->getDeviceContext());
		mouseShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix, mouseMask2->getShaderResourceView(), XMFLOAT2(input->getMouseX(), input->getMouseY()), mouseRadius);
		mouseShader->render(renderer->getDeviceContext(), blurOrtho->getIndexCount());

		renderer->setZBuffer(true);

		renderer->setBackBufferRenderTarget();
	}
}

void App1::horizontalBlurPass()
{
	XMMATRIX worldMatrix, baseViewMatrix, orthoMatrix;

	horizontalBlurTex->setRenderTarget(renderer->getDeviceContext());
	horizontalBlurTex->clearRenderTarget(renderer->getDeviceContext(), 0.5f, 0.f, 0.5f, 1.f);

	worldMatrix = renderer->getWorldMatrix();
	orthoMatrix = horizontalBlurTex->getOrthoMatrix();
	baseViewMatrix = camera->getOrthoViewMatrix();

	// Render for Horizontal Blur
	renderer->setZBuffer(false);

	if (flipMask) {
		blurOrtho->sendData(renderer->getDeviceContext());
		horizontalShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, mouseMask2->getShaderResourceView(), renderTexture->getShaderResourceView(), horizontalBlurTex->getTextureWidth());
		horizontalShader->render(renderer->getDeviceContext(), blurOrtho->getIndexCount());
		renderer->setZBuffer(true);
	}
	else {
		blurOrtho->sendData(renderer->getDeviceContext());
		horizontalShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, mouseMask1->getShaderResourceView(), renderTexture->getShaderResourceView(), horizontalBlurTex->getTextureWidth());
		horizontalShader->render(renderer->getDeviceContext(), blurOrtho->getIndexCount());
		renderer->setZBuffer(true);
	}

	renderer->setBackBufferRenderTarget();
}

void App1::verticalBlurPass()
{
	XMMATRIX worldMatrix, baseViewMatrix, orthoMatrix;

	verticalBlurTex->setRenderTarget(renderer->getDeviceContext());
	verticalBlurTex->clearRenderTarget(renderer->getDeviceContext(), 0.5f, 0.f, 0.5f, 1.f);

	worldMatrix = renderer->getWorldMatrix();
	orthoMatrix = verticalBlurTex->getOrthoMatrix();
	baseViewMatrix = camera->getOrthoViewMatrix();

	// Render for Horizontal Blur
	renderer->setZBuffer(false);

	if (flipMask) {
		blurOrtho->sendData(renderer->getDeviceContext());
		verticalShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, mouseMask2->getShaderResourceView(), renderTexture->getShaderResourceView(), verticalBlurTex->getTextureHeight());
		verticalShader->render(renderer->getDeviceContext(), blurOrtho->getIndexCount());
		renderer->setZBuffer(true);
		flipMask = false;
	}
	else {
		blurOrtho->sendData(renderer->getDeviceContext());
		verticalShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, mouseMask1->getShaderResourceView(), renderTexture->getShaderResourceView(), verticalBlurTex->getTextureHeight());
		verticalShader->render(renderer->getDeviceContext(), blurOrtho->getIndexCount());
		renderer->setZBuffer(true);
		flipMask = true;
	}

	renderer->setBackBufferRenderTarget();

	//set blurred scene to rendertexture for render to screen.
	renderTexture->setRenderTarget(renderer->getDeviceContext());
	renderTexture->clearRenderTarget(renderer->getDeviceContext(), 0.f, 0.f, 0.f, 1.f);
	renderer->setZBuffer(false);
		blurOrtho->sendData(renderer->getDeviceContext());
		textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, verticalBlurTex->getShaderResourceView());
		textureShader->render(renderer->getDeviceContext(), blurOrtho->getIndexCount());
	renderer->setZBuffer(true);
	renderer->setBackBufferRenderTarget();

}

void App1::finalPass()
{
	// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	renderer->setZBuffer(false);
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX orthoMatrix = renderer->getOrthoMatrix();  // ortho matrix for 2D rendering
	XMMATRIX orthoViewMatrix = camera->getOrthoViewMatrix();	// Default camera position for orthographic rendering

	blurOrtho->sendData(renderer->getDeviceContext());
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix, renderTexture->getShaderResourceView());
	textureShader->render(renderer->getDeviceContext(), blurOrtho->getIndexCount());


	//render depthmap RTT. Done here so it doesn't blur
	ID3D11ShaderResourceView* depthMaps[LIGHTNUM];
	for (int i = 0; i < LIGHTNUM; i++) {
		depthMaps[i] = shadowMap[i]->getDepthMapSRV();
	}
	ID3D11ShaderResourceView* pointDepthMaps[6];
	for (int i = 0; i < 6; i++) {
		pointDepthMaps[i] = pointShadowMap[i]->getDepthMapSRV();
	}

	ID3D11ShaderResourceView* RTTMap;

	if (chooseDepth < 4) {
		RTTMap = depthMaps[chooseDepth];
	}
	else {
		RTTMap = pointDepthMaps[chooseDepth - 4];
	}
	depthMapOrtho->sendData(renderer->getDeviceContext());
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix, RTTMap);
	textureShader->render(renderer->getDeviceContext(), depthMapOrtho->getIndexCount());

	renderer->setZBuffer(true);


	// Render GUI
	gui();

	// Present the rendered scene to the screen.
	renderer->endScene();
}

void App1::gui()
{
	// Force turn off unnecessary shader stages.
	renderer->getDeviceContext()->GSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->HSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->DSSetShader(NULL, NULL, 0);

	// Build UI
	ImGui::Text("FPS: %.2f", timer->getFPS());
	ImGui::Checkbox("Wireframe mode", &wireframeToggle);
	ImGui::Checkbox("blur mouse", &shouldBlur);
	ImGui::Checkbox("AmbientLight", &lightsON[0]);
	ImGui::Checkbox("PointLight", &lightsON[1]);
	ImGui::Checkbox("Spot1", &lightsON[2]);
	ImGui::Checkbox("Spot2", &lightsON[3]);
	ImGui::SliderFloat("point pos x", &pointPos.x, 0, 100);
	ImGui::SliderFloat("point pos y", &pointPos.y, 0, 50);
	ImGui::SliderFloat("point pos z", &pointPos.z, 0, 100);
	ImGui::SliderFloat("spot pos x", &spotPos.x, 0, 100);
	ImGui::SliderFloat("spot pos y", &spotPos.y, 0, 50);
	ImGui::SliderFloat("spot pos z", &spotPos.z, 0, 100);
	ImGui::SliderFloat("spot dir x", &spotDir.x, -1, 1);
	ImGui::SliderFloat("spot dir y", &spotDir.y, -1, 1);
	ImGui::SliderFloat("spot dir z", &spotDir.z, -1, 1);
	ImGui::SliderFloat("constant attenuation", &spotAtten.x, 0.001f, 3.f);
	ImGui::SliderFloat("linear attenuation", &spotAtten.y, 0.f, 1.f);
	ImGui::SliderFloat("quadratic attenuation", &spotAtten.z, 0.f, 0.1f);
	ImGui::SliderFloat("cutoff angle", &spotAtten.w, 2.36f, 3.1f);
	ImGui::SliderFloat("blurRadius", &mouseRadius, 1.f, 50.f);
	ImGui::SliderInt("display depthmap", &chooseDepth, 0, 9);
	ImGui::Checkbox("show normals", &normals);
	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

