#define LIGHTNUM 4

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


OutputType main(InputType input)
{
    OutputType output;

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    
	// Calculate the position of the vertice as viewed by the light source.
    for (int i = 0; i < LIGHTNUM; i++)
    {
        output.lightViewPos[i] = mul(input.position, worldMatrix);
        output.lightViewPos[i] = mul(output.lightViewPos[i], lightViewMatrix[i]);
        output.lightViewPos[i] = mul(output.lightViewPos[i], lightProjectionMatrix[i]);
    }
    for (int j = 0; j < 6; j++)
    {
        output.pointViewPos[j] = mul(input.position, worldMatrix);
        output.pointViewPos[j] = mul(output.pointViewPos[j], pointViewMatrix[j]);
        output.pointViewPos[j] = mul(output.pointViewPos[j], lightProjectionMatrix[1]);
    }
    
    output.tex = input.tex;
    output.normal = mul(input.normal, (float3x3) worldMatrix);
    output.normal = normalize(output.normal);

    output.worldPosition = mul(input.position, worldMatrix).xyz;

    return output;
}