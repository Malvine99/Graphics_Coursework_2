#define LIGHTNUM 4

// Texture and sampler registers
Texture2D texture0 : register(t0);
SamplerState Sampler0 : register(s0);


cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

struct InputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct OutputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TEXCOORD2;
    float3 bitangent : TEXCOORD3;
};

OutputType main(InputType input)
{
    OutputType output;
    //distance between main vertex and neighbouring values
    float offset = 0.01f;
    float scale = 20.f;
    //sample texture colour and move y-coordinate accordingly
    float4 textureColor = texture0.SampleLevel(Sampler0, input.tex, 0);
    input.position.y += (scale * textureColor.x);
    
    output.position = input.position;
    
	// Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

    //calculate normals for manipulated surface
    //get neighbouring height values
    //left
    float leftH = texture0.SampleLevel(Sampler0, float2(input.tex.x - offset, input.tex.y), 0).r * scale;
    //right
    float rightH = texture0.SampleLevel(Sampler0, float2(input.tex.x + offset, input.tex.y), 0).r * scale;
    //top
    float topH = texture0.SampleLevel(Sampler0, float2(input.tex.x, input.tex.y + offset), 0).r * scale;
    //bottom
    float bottomH = texture0.SampleLevel(Sampler0, float2(input.tex.x, input.tex.y - offset), 0).r * scale;

    //vectors left-right and top-bottom
    float3 tangent, bitangent;
 
    tangent = normalize(float3(-2.0f * offset, leftH - rightH, 0.0f));
    bitangent = normalize(float3(0.0f, topH - bottomH, 2.0f * offset));
    
    float3 newNormal = cross(tangent, bitangent);
    newNormal = mul(newNormal, (float3x3) worldMatrix);
    output.normal = normalize(newNormal);
    output.tangent = tangent / 3;
    output.bitangent = bitangent / 3;
          
    return output;
}