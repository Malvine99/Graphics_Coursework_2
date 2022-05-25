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
    float3 tangent : TEXCOORD2;
    float3 bitangent : TEXCOORD3;
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

float3 calculateNormal(float3 point1, float3 point2, float3 top)
{
    //get vector between the 2 bottom corners
    float3 betweenVector = normalize(point1 - point2);
    
    //get vector from intermediate point to top
    float3 betweenPoint = (point1 + point2) / 2;
    float3 height = normalize(top - betweenPoint);
    
    //cross intermediate vector and int-top vector
    float3 normal = cross(betweenVector, height);
    
    return normalize(normal);
}

[maxvertexcount(12)]
void main( point InputType input[1], inout TriangleStream<OutputType> triStream)
{
    OutputType output;
    float4 biTanOffset = float4(input[0].bitangent.x, input[0].bitangent.y, input[0].bitangent.z, 0.f);
    float4 tanOffset = float4(input[0].tangent.x, input[0].tangent.y, input[0].tangent.z, 0.f);
    float3 topOffset = input[0].position.xyz + (3 * input[0].normal);
    
    //TRIANGLE 1
    //FIRST point on line is slightly off the vertex position along the -bitangent
    output.position = input[0].position - biTanOffset;
    
    output.worldPosition = mul(output.position, worldMatrix).xyz;
    // Calculate the position of the vertice as viewed by the light source.
    for (int i = 0; i < LIGHTNUM; i++)
    {
            for (int j = 0; j < 6; j++)
            {
                output.pointViewPos[j] = mul(output.position, worldMatrix);
                output.pointViewPos[j] = mul(output.pointViewPos[j], pointViewMatrix[j]);
                output.pointViewPos[j] = mul(output.pointViewPos[j], lightProjectionMatrix[1]);
            }
	        // Calculate the position of the vertice as viewed by the light source.
            output.lightViewPos[i] = mul(output.position, worldMatrix);
            output.lightViewPos[i] = mul(output.lightViewPos[i], lightViewMatrix[i]);
            output.lightViewPos[i] = mul(output.lightViewPos[i], lightProjectionMatrix[i]);
    }
    
    output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.tex = float2(0.25f, 1.f);
    output.normal = calculateNormal(tanOffset.xyz, -biTanOffset.xyz, topOffset);
    triStream.Append(output);

    //SECOND point is position plus amplified normal
    output.position.xyz = topOffset;
    output.position.w = 1.f;
    
    output.worldPosition = mul(output.position, worldMatrix).xyz;
    // Calculate the position of the vertice as viewed by the light source.
    for (int i = 0; i < LIGHTNUM; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            output.pointViewPos[j] = mul(output.position, worldMatrix);
            output.pointViewPos[j] = mul(output.pointViewPos[j], pointViewMatrix[j]);
            output.pointViewPos[j] = mul(output.pointViewPos[j], lightProjectionMatrix[1]);
        }
	        // Calculate the position of the vertice as viewed by the light source.
        output.lightViewPos[i] = mul(output.position, worldMatrix);
        output.lightViewPos[i] = mul(output.lightViewPos[i], lightViewMatrix[i]);
        output.lightViewPos[i] = mul(output.lightViewPos[i], lightProjectionMatrix[i]);
    }
    
    output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.tex = float2(0.125f, 0.f);
    output.normal = calculateNormal(tanOffset.xyz, -biTanOffset.xyz, topOffset);
    triStream.Append(output);
    
    //THIRD point on line is slightly off the vertex position along the tangent
    output.position = input[0].position + tanOffset;
    
    output.worldPosition = mul(output.position, worldMatrix).xyz;
    // Calculate the position of the vertice as viewed by the light source.
    for (int i = 0; i < LIGHTNUM; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            output.pointViewPos[j] = mul(output.position, worldMatrix);
            output.pointViewPos[j] = mul(output.pointViewPos[j], pointViewMatrix[j]);
            output.pointViewPos[j] = mul(output.pointViewPos[j], lightProjectionMatrix[1]);
        }
	        // Calculate the position of the vertice as viewed by the light source.
        output.lightViewPos[i] = mul(output.position, worldMatrix);
        output.lightViewPos[i] = mul(output.lightViewPos[i], lightViewMatrix[i]);
        output.lightViewPos[i] = mul(output.lightViewPos[i], lightProjectionMatrix[i]);
    }
    
    output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.tex = float2(0.f, 1.f);
    output.normal = calculateNormal(tanOffset.xyz, -biTanOffset.xyz, topOffset);
    triStream.Append(output);
    

    
    
        //TRIANGLE 2
    //FIRST point on line is slightly off the vertex position along the -bitangent
    output.position = input[0].position - biTanOffset;
    
    output.worldPosition = mul(output.position, worldMatrix).xyz;
    // Calculate the position of the vertice as viewed by the light source.
    for (int i = 0; i < LIGHTNUM; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            output.pointViewPos[j] = mul(output.position, worldMatrix);
            output.pointViewPos[j] = mul(output.pointViewPos[j], pointViewMatrix[j]);
            output.pointViewPos[j] = mul(output.pointViewPos[j], lightProjectionMatrix[1]);
        }
	        // Calculate the position of the vertice as viewed by the light source.
        output.lightViewPos[i] = mul(output.position, worldMatrix);
        output.lightViewPos[i] = mul(output.lightViewPos[i], lightViewMatrix[i]);
        output.lightViewPos[i] = mul(output.lightViewPos[i], lightProjectionMatrix[i]);
    }
    
    output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.tex = float2(0.25f, 1.f);
    output.normal = calculateNormal(-biTanOffset.xyz, -tanOffset.xyz, topOffset);
    triStream.Append(output);

    //SECOND point is position plus amplified normal
    output.position.xyz = topOffset;
    output.position.w = 1.f;
    
    output.worldPosition = mul(output.position, worldMatrix).xyz;
    // Calculate the position of the vertice as viewed by the light source.
    for (int i = 0; i < LIGHTNUM; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            output.pointViewPos[j] = mul(output.position, worldMatrix);
            output.pointViewPos[j] = mul(output.pointViewPos[j], pointViewMatrix[j]);
            output.pointViewPos[j] = mul(output.pointViewPos[j], lightProjectionMatrix[1]);
        }
	        // Calculate the position of the vertice as viewed by the light source.
        output.lightViewPos[i] = mul(output.position, worldMatrix);
        output.lightViewPos[i] = mul(output.lightViewPos[i], lightViewMatrix[i]);
        output.lightViewPos[i] = mul(output.lightViewPos[i], lightProjectionMatrix[i]);
    }
    
    output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.tex = float2(0.375f, 0.f);
    output.normal = calculateNormal(-biTanOffset.xyz, -tanOffset.xyz, topOffset);
    triStream.Append(output);
    
    //THIRD point on line is slightly off the vertex position along the -tangent
    output.position = input[0].position - tanOffset;
    
    output.worldPosition = mul(output.position, worldMatrix).xyz;
    // Calculate the position of the vertice as viewed by the light source.
    for (int i = 0; i < LIGHTNUM; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            output.pointViewPos[j] = mul(output.position, worldMatrix);
            output.pointViewPos[j] = mul(output.pointViewPos[j], pointViewMatrix[j]);
            output.pointViewPos[j] = mul(output.pointViewPos[j], lightProjectionMatrix[1]);
        }
	        // Calculate the position of the vertice as viewed by the light source.
        output.lightViewPos[i] = mul(output.position, worldMatrix);
        output.lightViewPos[i] = mul(output.lightViewPos[i], lightViewMatrix[i]);
        output.lightViewPos[i] = mul(output.lightViewPos[i], lightProjectionMatrix[i]);
    }
    
    output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.tex = float2(0.5f, 1.f);
    output.normal = calculateNormal(-biTanOffset.xyz, -tanOffset.xyz, topOffset);
    triStream.Append(output);

    
    
    
        //TRIANGLE 3
    //FIRST point on line is slightly off the vertex position along the bitangent
    output.position = input[0].position + biTanOffset;
    
    output.worldPosition = mul(output.position, worldMatrix).xyz;
    // Calculate the position of the vertice as viewed by the light source.
    for (int i = 0; i < LIGHTNUM; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            output.pointViewPos[j] = mul(output.position, worldMatrix);
            output.pointViewPos[j] = mul(output.pointViewPos[j], pointViewMatrix[j]);
            output.pointViewPos[j] = mul(output.pointViewPos[j], lightProjectionMatrix[1]);
        }
	        // Calculate the position of the vertice as viewed by the light source.
        output.lightViewPos[i] = mul(output.position, worldMatrix);
        output.lightViewPos[i] = mul(output.lightViewPos[i], lightViewMatrix[i]);
        output.lightViewPos[i] = mul(output.lightViewPos[i], lightProjectionMatrix[i]);
    }
    
    output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.tex = float2(0.75f, 1.f);
    output.normal = calculateNormal(-tanOffset.xyz, biTanOffset.xyz, topOffset);
    triStream.Append(output);

    //SECOND point is position plus amplified normal
    output.position.xyz = topOffset;
    output.position.w = 1.f;
    
    output.worldPosition = mul(output.position, worldMatrix).xyz;
    // Calculate the position of the vertice as viewed by the light source.
    for (int i = 0; i < LIGHTNUM; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            output.pointViewPos[j] = mul(output.position, worldMatrix);
            output.pointViewPos[j] = mul(output.pointViewPos[j], pointViewMatrix[j]);
            output.pointViewPos[j] = mul(output.pointViewPos[j], lightProjectionMatrix[1]);
        }
	        // Calculate the position of the vertice as viewed by the light source.
        output.lightViewPos[i] = mul(output.position, worldMatrix);
        output.lightViewPos[i] = mul(output.lightViewPos[i], lightViewMatrix[i]);
        output.lightViewPos[i] = mul(output.lightViewPos[i], lightProjectionMatrix[i]);
    }
    
    output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.tex = float2(0.625f, 0.f);
    output.normal = calculateNormal(-tanOffset.xyz, biTanOffset.xyz, topOffset);
    triStream.Append(output);
    
    //THIRD point on line is slightly off the vertex position along the -tangent
    output.position = input[0].position - tanOffset;
    
    output.worldPosition = mul(output.position, worldMatrix).xyz;
    // Calculate the position of the vertice as viewed by the light source.
    for (int i = 0; i < LIGHTNUM; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            output.pointViewPos[j] = mul(output.position, worldMatrix);
            output.pointViewPos[j] = mul(output.pointViewPos[j], pointViewMatrix[j]);
            output.pointViewPos[j] = mul(output.pointViewPos[j], lightProjectionMatrix[1]);
        }
	        // Calculate the position of the vertice as viewed by the light source.
        output.lightViewPos[i] = mul(output.position, worldMatrix);
        output.lightViewPos[i] = mul(output.lightViewPos[i], lightViewMatrix[i]);
        output.lightViewPos[i] = mul(output.lightViewPos[i], lightProjectionMatrix[i]);
    }
    
    output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.tex = float2(0.5f, 1.f);
    output.normal = calculateNormal(-tanOffset.xyz, biTanOffset.xyz, topOffset);
    triStream.Append(output);
    

    
    
        //TRIANGLE 4
    //FIRST point on line is slightly off the vertex position along the bitangent
    output.position = input[0].position + biTanOffset;
    
    output.worldPosition = mul(output.position, worldMatrix).xyz;
    // Calculate the position of the vertice as viewed by the light source.
    for (int i = 0; i < LIGHTNUM; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            output.pointViewPos[j] = mul(output.position, worldMatrix);
            output.pointViewPos[j] = mul(output.pointViewPos[j], pointViewMatrix[j]);
            output.pointViewPos[j] = mul(output.pointViewPos[j], lightProjectionMatrix[1]);
        }
	        // Calculate the position of the vertice as viewed by the light source.
        output.lightViewPos[i] = mul(output.position, worldMatrix);
        output.lightViewPos[i] = mul(output.lightViewPos[i], lightViewMatrix[i]);
        output.lightViewPos[i] = mul(output.lightViewPos[i], lightProjectionMatrix[i]);
    }
    
    output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.tex = float2(0.75f, 1.f);
    output.normal = calculateNormal(biTanOffset.xyz, tanOffset.xyz, topOffset);
    triStream.Append(output);

    //SECOND point is position plus amplified normal
    output.position.xyz = topOffset;
    output.position.w = 1.f;
    
    output.worldPosition = mul(output.position, worldMatrix).xyz;
    // Calculate the position of the vertice as viewed by the light source.
    for (int i = 0; i < LIGHTNUM; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            output.pointViewPos[j] = mul(output.position, worldMatrix);
            output.pointViewPos[j] = mul(output.pointViewPos[j], pointViewMatrix[j]);
            output.pointViewPos[j] = mul(output.pointViewPos[j], lightProjectionMatrix[1]);
        }
	        // Calculate the position of the vertice as viewed by the light source.
        output.lightViewPos[i] = mul(output.position, worldMatrix);
        output.lightViewPos[i] = mul(output.lightViewPos[i], lightViewMatrix[i]);
        output.lightViewPos[i] = mul(output.lightViewPos[i], lightProjectionMatrix[i]);
    }
    
    output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.tex = float2(0.875f, 0.f);
    output.normal = calculateNormal(biTanOffset.xyz, tanOffset.xyz, topOffset);
    triStream.Append(output);
    
    //THIRD point on line is slightly off the vertex position along the tangent
    output.position = input[0].position + tanOffset;
    
    output.worldPosition = mul(output.position, worldMatrix).xyz;
    // Calculate the position of the vertice as viewed by the light source.
    for (int i = 0; i < LIGHTNUM; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            output.pointViewPos[j] = mul(output.position, worldMatrix);
            output.pointViewPos[j] = mul(output.pointViewPos[j], pointViewMatrix[j]);
            output.pointViewPos[j] = mul(output.pointViewPos[j], lightProjectionMatrix[1]);
        }
	        // Calculate the position of the vertice as viewed by the light source.
        output.lightViewPos[i] = mul(output.position, worldMatrix);
        output.lightViewPos[i] = mul(output.lightViewPos[i], lightViewMatrix[i]);
        output.lightViewPos[i] = mul(output.lightViewPos[i], lightProjectionMatrix[i]);
    }
    
    output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.tex = float2(1.f, 1.f);
    output.normal = calculateNormal(biTanOffset.xyz, tanOffset.xyz, topOffset);
    triStream.Append(output);


    triStream.RestartStrip();

}