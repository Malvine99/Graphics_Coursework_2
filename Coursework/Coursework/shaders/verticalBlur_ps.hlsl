Texture2D maskTexture : register(t0);
Texture2D sceneTexture : register(t1);
SamplerState Sampler0 : register(s0);

cbuffer ScreenSizeBuffer : register(b0)
{
    float screenHeight;
    float3 padding;
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};


float4 main(InputType input) : SV_TARGET
{
    float4 colour = sceneTexture.Sample(Sampler0, input.tex);
    float4 maskColour = maskTexture.Sample(Sampler0, input.tex);
    
    if (maskColour.x == 1.f)
    {
        float weight0, weight1, weight2, weight3, weight4;
 	    // Create the weights that each neighbor pixel will contribute to the blur.
        weight4 = 0.382928f;
        weight3 = 0.241732f;
        weight2 = 0.060598f;
        weight1 = 0.005977f;
        weight0 = 0.000229f;
      
        float texelSize = 1.0f / screenHeight;
        // Add the horizontal pixels to the colour by the specific weight of each.
        colour += sceneTexture.Sample(Sampler0, input.tex + float2(0.f, texelSize * -4.0f)) * weight4;
        colour += sceneTexture.Sample(Sampler0, input.tex + float2(0.f, texelSize * -3.0f)) * weight3;
        colour += sceneTexture.Sample(Sampler0, input.tex + float2(0.f, texelSize * -2.0f)) * weight2;
        colour += sceneTexture.Sample(Sampler0, input.tex + float2(0.f, texelSize * -1.0f)) * weight1;
        colour += sceneTexture.Sample(Sampler0, input.tex) * weight0;
        colour += sceneTexture.Sample(Sampler0, input.tex + float2(0.f, texelSize * 1.0f)) * weight1;
        colour += sceneTexture.Sample(Sampler0, input.tex + float2(0.f, texelSize * 2.0f)) * weight2;
        colour += sceneTexture.Sample(Sampler0, input.tex + float2(0.f, texelSize * 3.0f)) * weight3;
        colour += sceneTexture.Sample(Sampler0, input.tex + float2(0.f, texelSize * 4.0f)) * weight4;
    }
    
    
    return saturate(colour);
}