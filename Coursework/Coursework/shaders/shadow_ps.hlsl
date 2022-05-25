#define LIGHTNUM 4

Texture2D shaderTexture : register(t0);
Texture2D<float4> depthMapTexture[LIGHTNUM] : register(t1);
Texture2D<float4> pointDepthMapTexture[6] : register(t5);

SamplerState diffuseSampler : register(s0);
SamplerState shadowSampler : register(s1);

cbuffer LightBuffer : register(b0)
{
    float4 ambient;
    float4 diffuse[LIGHTNUM];
    float4 position[LIGHTNUM];
    float4 atten[LIGHTNUM];
    float3 spotDir;
    float padding;
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float4 lightViewPos[LIGHTNUM] : TEXCOORD1;
    float3 worldPosition : TEXCOORD5;
    float4 pointViewPos[6] : TEXCOORD6;
};

// Calculate lighting intensity based on direction and normal. Combine with light colour.
float4 calculateLighting(float3 lightDirection, float3 normal, float4 diffuse)
{
    float intensity = saturate(dot(normal, lightDirection));
    float4 colour = saturate(diffuse * intensity);
    return colour;
}

// Is the gemoetry in our shadow map
bool hasDepthData(float2 uv)
{
    if (uv.x < 0.f || uv.x > 1.f || uv.y < 0.f || uv.y > 1.f)
    {
        return false;
    }
    return true;
}

bool isInShadow(Texture2D sMap, float2 uv, float4 lightViewPosition, float bias)
{
    // Sample the shadow map (get depth of geometry)
    float depthValue = sMap.Sample(shadowSampler, uv).r;
	// Calculate the depth from the light.
    float lightDepthValue = lightViewPosition.z / lightViewPosition.w;
    lightDepthValue -= bias;

	// Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
    if (lightDepthValue < depthValue)
    {
        return false;
    }
    return true;
}

float2 getProjectiveCoords(float4 lightViewPosition)
{
    // Calculate the projected texture coordinates.
    float2 projTex = lightViewPosition.xy / lightViewPosition.w;
    projTex *= float2(0.5, -0.5);
    projTex += float2(0.5f, 0.5f);
    return projTex;
}

float4 main(InputType input) : SV_TARGET
{
    float shadowMapBias = 0.005f;
    float4 lightColour = float4(0.f, 0.f, 0.f, 1.f);
    float4 textureColour = shaderTexture.Sample(diffuseSampler, input.tex);

    for (int i = 0; i < LIGHTNUM; i++) {
        if (position[i].w == 0)
        { //point light	
                for (int j = 0; j < 6; j++)
                {
                // Calculate the projected texture coordinates.
                float2 pTexCoord = getProjectiveCoords(input.pointViewPos[j]);
                
                    // Shadow test. Is or isn't in shadow
                if (hasDepthData(pTexCoord))
                { // Has depth map data

                    if (!isInShadow(pointDepthMapTexture[j], pTexCoord, input.pointViewPos[j], shadowMapBias))
                    {
                    // is NOT in shadow, therefore light
                        //cut off to avoid light overlap
                        float3 direction;// = float3(1.f, 0.f, 0.f);
                        
                        switch (j)
                        {
                            case 0:{
		                            //along X
                                    direction = float3(1.f, 0.f, 0.f);
                                    break;
                                }
                            case 1:{
		                            //along -X
                                    direction = float3(-1.f, 0.f, 0.f);
                                    break;
                                }
                            case 2:{
		                            //along Z
                                    direction = float3(0.f, 0.f, 1.f);
                                    break;
                                }
                            case 3:{
		                            //along -Z
                                    direction = float3(0.f, 0.f, -1.f);
                                    break;
                                }
                            case 4:{
		                            //along Y
                                    direction = float3(0.f, 1.f, 0.f);
                                    break;
                                }
                            case 5:{
		                            //along -Y
                                    direction = float3(0.f, -1.f, 0.f);
                                    break;
                                }
                            default:
                                direction = float3(0.f, -1.f, 0.f);
                                break;
                        }
                        
                        float3 lightVector = normalize(position[i].xyz - input.worldPosition);
                            float distance = length(position[i].xyz - input.worldPosition);
                            float attenuation = 1 / (atten[i].x + (atten[i].y * distance) + (atten[i].z * pow(distance, 2)));
                            lightColour += calculateLighting(lightVector * attenuation, input.normal, diffuse[i]);
                    }
                }
            }

        }
        else if (position[i].w == 1)
        { //directional light
            // Calculate the projected texture coordinates.
            float2 pTexCoord = getProjectiveCoords(input.lightViewPos[i]);
	
            // Shadow test. Is or isn't in shadow
            if (hasDepthData(pTexCoord))
            {
                // Has depth map data
                if (!isInShadow(depthMapTexture[i], pTexCoord, input.lightViewPos[i], shadowMapBias))
                {
                    // is NOT in shadow, therefore light
                    float3 lightVector = normalize(position[i].xyz);
                    lightColour += calculateLighting(-lightVector, input.normal, diffuse[i]);
                }
            }
        }
        else
        { // spotlight
            // Calculate the projected texture coordinates.
            float2 pTexCoord = getProjectiveCoords(input.lightViewPos[i]);
	
            // Shadow test. Is or isn't in shadow
            if (hasDepthData(pTexCoord))
            {
                // Has depth map data
                if (!isInShadow(depthMapTexture[i], pTexCoord, input.lightViewPos[i], shadowMapBias))
                {
                    // is NOT in shadow, therefore light
                    float3 direction = spotDir; //{ 0.f, -1.f, 0.f};
                    float3 lightVector = normalize(position[i].xyz - input.worldPosition);
                    float dotCheck = (dot(lightVector, direction)) / (length(lightVector) * length(direction));
                    if (dotCheck <= cos(atten[i].w))
                    {
                        float distance = length(position[i].xyz - input.worldPosition);
                        float attenuation = 1 / (atten[i].x + (atten[i].y * distance) + (atten[i].z * pow(distance, 2)));
                        lightColour += calculateLighting(lightVector * attenuation, input.normal, diffuse[i]);
                    }
                }
            }

        }
    }
    
    
    lightColour = saturate(lightColour + ambient);
    return saturate(lightColour) * textureColour;
}