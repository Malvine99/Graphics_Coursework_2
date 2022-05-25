#include "LightA.h"

void LightA::setAttenuation(float constant, float linear, float quadratic, float cutoff)
{
	attenuation.x = constant;
	attenuation.y = linear;
	attenuation.z = quadratic;
	attenuation.w = cutoff;
}

void LightA::setAttenuation(XMFLOAT4 atten)
{
	attenuation = atten;
}

void LightA::setType(float t)
{
	//0 = point light, 1 = directional light, 2 = spot light
	type = t;
}

void LightA::setPosition(float x, float y, float z)
{
	position = XMVectorSet(x, y, z, 1.f);
}

void LightA::setPosition(XMFLOAT3 pos)
{
	position = XMVectorSet(pos.x, pos.y, pos.z, 1.f);
}

void LightA::setDiffuseColour(float red, float green, float blue, float alpha)
{
	diffuseColour = XMFLOAT4(red, green, blue, alpha);
}

void LightA::setDiffuseColour(XMFLOAT4 col)
{
	diffuseColour = col;
}

XMFLOAT4 LightA::getAttenuation()
{
	return attenuation;
}

float LightA::getType()
{
	//0 = point light, 1 = directional light, 2 = spot light
	return type;
}

void LightA::generatePointViewMatrix(int direction)
{
	//creates a viewmatrix along the given axis
	//0 = X, 1 = -X, 2 = Z, 3 = -Z, 4 = Y, 5 = -Y

	// default up vector
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
	//init direction vector
	XMVECTOR dir = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	switch (direction) {
	case 0: {
		//along X
		dir = XMVectorSet(1.f, 0.f, 0.f, 1.f);
		break;
	}
	case 1: {
		//along -X
		dir = XMVectorSet(-1.f, 0.f, 0.f, 1.f);
		break;
	}
	case 2: {
		//along Z
		dir = XMVectorSet(0.f, 0.f, 1.f, 1.f);
		break;
	}
	case 3: {
		//along -Z
		dir = XMVectorSet(0.f, 0.f, -1.f, 1.f);
		break;
	}
	case 4: {
		//along Y
		up = XMVectorSet(0.f, 0.f, 1.f, 1.f);
		dir = XMVectorSet(0.f, 1.f, 0.f, 1.f);
		break;
	}
	case 5: {
		//along -Y
		up = XMVectorSet(0.f, 0.f, -1.f, 1.f);
		dir = XMVectorSet(0.f, -1.f, 0.f, 1.f);
		break;
	}
	default:
		break;
	}

	XMVECTOR right = XMVector3Cross(dir, up);
	up = XMVector3Cross(right, dir);
	// Create the view matrix from the three vectors.
	viewMatrix = XMMatrixLookAtLH(position, position + dir, up);
}
