#define LIGHTNUM 4

cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

struct InputType
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TEXCOORD0;
    float3 bitangent : TEXCOORD1;
};

struct OutputType
{
    float4 position : SV_POSITION;
    float4 depthPosition : TEXCOORD0;
};

[maxvertexcount(12)]
void main(point InputType input[1], inout TriangleStream<OutputType> triStream)
{
    OutputType output;
    float4 biTanOffset = float4(input[0].bitangent.x, input[0].bitangent.y, input[0].bitangent.z, 0.f);
    float4 tanOffset = float4(input[0].tangent.x, input[0].tangent.y, input[0].tangent.z, 0.f);
    float3 topOffset = input[0].position.xyz + (3 * input[0].normal);
    
    //TRIANGLE 1
    //first point on line is slightly off the vertex position along the -bitangent
    output.position = input[0].position - biTanOffset;
    output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.depthPosition = output.position;
    triStream.Append(output);

    //second point is position plus amplified normal
    output.position.xyz = topOffset;
    output.position.w = 1.f;
    output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.depthPosition = output.position;
    triStream.Append(output);
    
    //third point on line is slightly off the vertex position along the tangent
    output.position = input[0].position + tanOffset;
    output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.depthPosition = output.position;
    triStream.Append(output);
    

        //TRIANGLE 2
    //first point on line is slightly off the vertex position along the -bitangent
    output.position = input[0].position - biTanOffset;
    output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.depthPosition = output.position;
    triStream.Append(output);

    //second point is position plus amplified normal
    output.position.xyz = topOffset;
    output.position.w = 1.f;
    output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.depthPosition = output.position;
    triStream.Append(output);
    
    //third point on line is slightly off the vertex position along the -tangent
    output.position = input[0].position - tanOffset;
    output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.depthPosition = output.position;
    triStream.Append(output);

    
        //TRIANGLE 3
    //first point on line is slightly off the vertex position along the bitangent
    output.position = input[0].position + biTanOffset;
    output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.depthPosition = output.position;
    triStream.Append(output);

    //second point is position plus amplified normal
    output.position.xyz = topOffset;
    output.position.w = 1.f;
    output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.depthPosition = output.position;
    triStream.Append(output);
    
    //third point on line is slightly off the vertex position along the -tangent
    output.position = input[0].position - tanOffset;
    output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.depthPosition = output.position;
    triStream.Append(output);
    

        //TRIANGLE 4
    //first point on line is slightly off the vertex position along the bitangent
    output.position = input[0].position + biTanOffset;
    output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.depthPosition = output.position;
    triStream.Append(output);

    //second point is position plus amplified normal
    output.position.xyz = topOffset;
    output.position.w = 1.f;
    output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.depthPosition = output.position;
    triStream.Append(output);
    
    //third point on line is slightly off the vertex position along the tangent
    output.position = input[0].position + tanOffset;
    output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.depthPosition = output.position;
    triStream.Append(output);


    triStream.RestartStrip();

}