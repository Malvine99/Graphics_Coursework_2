#pragma once
#include "Light.h"
class LightA :
    public Light
{
public:
    void setAttenuation(float constant, float linear, float quadratic, float cutoff);	
    void setAttenuation(XMFLOAT4 attenuation);
    void setType(float t);
    void setPosition(float x, float y, float z);
    void setPosition(XMFLOAT3 pos);
    void setDiffuseColour(float red, float green, float blue, float alpha);
    void setDiffuseColour(XMFLOAT4 col);
    XMFLOAT4 getAttenuation();			
    float getType();

    void generatePointViewMatrix(int direction);

private:
    XMFLOAT4 attenuation;
    float type;
};

