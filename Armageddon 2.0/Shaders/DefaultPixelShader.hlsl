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
    float3 CameraPos;
	int PointLightCount;
    float DirectionalLightCount;

    PointLight PointLights[50];
    DirectionalLight DirectionalLights[50];
};




struct PSinput
{
    float4 position : SV_Position;
    float2 textCoord : TEXCOORD0;
    float3 normal    : NORMAL;
    float3 Tangent   : TANGENT;
    float3 Binormal  : BINORMAL;
	float3 WorldPos : POSITION;

};

float4 main(PSinput input) : SV_TARGET
{
   
    float  DiffuseIntensity = saturate(dot(input.normal, float3(0.5f,0.5f,0.5f)));
    float3 DiffuseColor = float3(1.0f, 1.0f, 1.0f);
    float4 Diffuse = float4((DiffuseIntensity * DiffuseColor).xyz, 1.0f);

	return float4(CameraPos,1.0f);
}