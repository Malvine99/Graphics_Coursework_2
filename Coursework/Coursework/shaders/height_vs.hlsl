#define LIGHTNUM 4

// Texture and sampler registers
Texture2D texture0 : register(t0);
SamplerState Sampler0 : register(s0);


cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
    matrix lightViewMatrix[LIGHTNUM];
    matrix pointViewMatrix[6];
    matrix lightProjectionMatrix[LIGHTNUM];
};

struct InputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct OutputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float4 lightViewPos[LIGHTNUM] : TEXCOORD1;
    float3 worldPosition : TEXCOORD5;
    float4 pointViewPos[6] : TEXCOORD6;
};

float3 calculateNormal(float scale, float2 tex)
{
    float offset = 0.01f;
    //calculate normals for manipulated surface
    //get neighbouring height values
    //left
    float leftH = texture0.SampleLevel(Sampler0, float2(tex.x - offset, tex.y), 0).r * scale;
    //right
    float rightH = texture0.SampleLevel(Sampler0, float2(tex.x + offset, tex.y), 0).r * scale;
    //top
    float topH = texture0.SampleLevel(Sampler0, float2(tex.x, tex.y + offset), 0).r * scale;
    //bottom
    float bottomH = texture0.SampleLevel(Sampler0, float2(tex.x, tex.y - offset), 0).r * scale;

    //vectors left-right and top-bottom
    float3 tangent, bitangent;
 
    tangent = normalize(float3(-2.0f * offset, leftH - rightH, 0.0f));
    bitangent = normalize(float3(0.0f, topH - bottomH, 2.0f * offset));

    float3 newNormal = cross(tangent, bitangent);
    newNormal = mul(newNormal, (float3x3) worldMatrix);

    return normalize(newNormal);
}

OutputType main(InputType input)
{
    OutputType output;
    //distance between main vertex and neighbouring values
    float scale = 20.f;
    //sample texture colour and move y-coordinate accordingly
    float4 textureColor = texture0.SampleLevel(Sampler0, input.tex, 0);
    input.position.y += (scale * textureColor.x);
    
	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    
	// Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

    output.normal = calculateNormal(scale, input.tex);
       
    // Calculate the position of the vertice as viewed by the light source.
    for (int i = 0; i < LIGHTNUM; i++)
    {
        if (i == 1)
        {
            for (int j = 0; j < 6; j++)
            {
                output.pointViewPos[j] = mul(input.position, worldMatrix);
                output.pointViewPos[j] = mul(output.pointViewPos[j], pointViewMatrix[j]);
                output.pointViewPos[j] = mul(output.pointViewPos[j], lightProjectionMatrix[i]);
            }
        }
        else
        {
	        // Calculate the position of the vertice as viewed by the light source.
            output.lightViewPos[i] = mul(input.position, worldMatrix);
            output.lightViewPos[i] = mul(output.lightViewPos[i], lightViewMatrix[i]);
            output.lightViewPos[i] = mul(output.lightViewPos[i], lightProjectionMatrix[i]);
        }
    }

    output.worldPosition = mul(input.position, worldMatrix).xyz;

    return output;
}