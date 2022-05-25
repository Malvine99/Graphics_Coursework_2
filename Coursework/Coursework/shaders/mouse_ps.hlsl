
// Texture and sampler registers
Texture2D texture0 : register(t0);
SamplerState Sampler0 : register(s0);

cbuffer mouseBuffer : register(b0)
{
    float2 mousePosition;
    float blurRadius;
    float padding;
}

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

bool inRadius(float2 position)
{
    //checks if the pixel is within the radius of the mouse pointer
    float d = (blurRadius * blurRadius) - (((mousePosition.x - position.x) * (mousePosition.x - position.x)) + ((mousePosition.y - position.y) * (mousePosition.y - position.y)));
    if (d > 0)
    {
        return true;
    }
    else
    {
        return false;
    }

}

float4 main(InputType input) : SV_TARGET
{
    float4 textureColour = texture0.Sample(Sampler0, input.tex);

    if (inRadius(input.position.xy))
    {
        textureColour = float4(1.f, 1.f, 1.f, 1.f);
    }
    
    return textureColour;
}