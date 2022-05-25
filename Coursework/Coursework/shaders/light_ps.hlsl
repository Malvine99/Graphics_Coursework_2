// Light pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)
#define LIGHTNUM 4

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

cbuffer LightBuffer : register(b0)
{
    float4 ambient;
    float4 diffuse[LIGHTNUM];
    //fourth value is flag for light type
    float4 position[LIGHTNUM];
    float4 atten[LIGHTNUM];
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 worldPosition : TEXCOORD1;
};

// Calculate lighting intensity based on direction and normal. Combine with light colour.
float4 calculateLighting(float3 lightDirection, float3 normal, float4 ldiffuse)
{
    float intensity = saturate(dot(normal, lightDirection));
    float4 colour = saturate(ldiffuse * intensity);
    return colour;
}

float4 main(InputType input) : SV_TARGET
{
 	// Sample the texture. Calculate light intensity and colour, return light*texture for final pixel colour.
    float4 textureColour = texture0.Sample(sampler0, input.tex);
	//textureColour = float4(input.normal.x, input.normal.y, input.normal.z, 1.0f);
    
	//return textureColour;
	float4 lightColour = ambient;
    
    for (int i = 0; i < LIGHTNUM; i++)
    {
        //check flag for point or directional
        if (position[i].w == 0) //point
        {
            float3 lightVector = normalize(position[i].xyz - input.worldPosition);
            float distance = length(position[i].xyz - input.worldPosition);
            float attenuation = 1 / (atten[i].x + (atten[i].y * distance) + (atten[i].z * pow(distance, 2)));
            lightColour += calculateLighting(lightVector*attenuation, input.normal, diffuse[i]);
        }
        else if (position[i].w == 1)//directional
        {
            float3 lightVector = normalize(position[i].xyz);
            lightColour += calculateLighting(-lightVector, input.normal, diffuse[i]);
        } 
        else //spotlight
        {
            float3 direction = { 0.f, -1.f, 0.f };
            float3 lightVector = normalize(position[i].xyz - input.worldPosition);
            float dotCheck = (dot(lightVector, direction)) / (length(lightVector) * length(direction));
            if (dotCheck <= cos(atten[i].w))
            {
              float distance = length(position[i].xyz - input.worldPosition);
              float attenuation = 1 / (atten[i].x + (atten[i].y * distance) + (atten[i].z * pow(distance, 2)));
              lightColour += calculateLighting(lightVector*attenuation, input.normal, diffuse[i]);
            }
        }
    }
    return (lightColour* textureColour);
}

