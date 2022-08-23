struct PointLight
{
    float3 Position;
    float3 Color;
    float Intensity;
    float Radius;
};

struct DirectionalLight
{
    float3 Direction;
    float3 Color;
    float Intensity;
};
cbuffer LightCBuffer : register(b1)
{
    int PointLightCount;
    int DirectionalLightCount;
    float2 padding1;


    PointLight PointLights[50];
    DirectionalLight DirectionalLights[50];
    row_major float4x4 LightViewProjectionCascade[3]; // TODO BETTER HANDLING OF CASCADE NUM
    float FarPlane[3]; // attention à l'alignement :D 
    int cascadeIndice;

};


cbuffer WorldCBuffer : register(b4)
{
    float time;
}

struct PSinput
{
    float4 position : SV_Position;
    float2 textCoord : TEXCOORD0;
    float3 normal    : NORMAL;
    float3 Tangent   : TANGENT;
    float3 Binormal  : BINORMAL;
	float3 WorldPos : POSITION;

};

Texture2D AlbedoMap : register(t10);
SamplerState Sampler : register(s0);

float4 main(PSinput input) : SV_TARGET
{
    float4 AlbedoTex            = AlbedoMap.Sample(Sampler, input.textCoord);
    float  DiffuseIntensity = saturate(dot(input.normal, float3(0.5f,0.5f,0.5f)));
    float3 DiffuseColor = float3(1.0f, 1.0f, 1.0f);
    float4 Diffuse = float4((DiffuseIntensity * DiffuseColor).xyz, 1.0f);

	return float4(abs(sin(time))* 3 + AlbedoTex.r,abs(sin(time))* 3,0.0,1.0f);
}