
struct VSinput
{
    float3 position : POSITION;

    
};

struct DirectionalLight
{
    float3 Direction; //12
    float Intensity; //4
    float3 Color; //12
    float padding0; //4
};
struct PointLight
{
    //On alterne pour rester alligner sur 16 bytes
    float3 Position; //12
    float Intensity; //4
    float3 Color; //12
    float Radius; //4
};

cbuffer LightCBuffer : register(b1)
{
    float3 CameraPos;
    float Padding0;
	
    int PointLightCount;
    int DirectionalLightCount;
    float2 padding1;


    PointLight PointLights[50];
    DirectionalLight DirectionalLights[50];
    row_major float4x4 LightViewProjection;
};
cbuffer TransFormBuffer : register(b0)
{
    row_major float4x4 WorldMat;
    row_major float4x4 ProjectionMat;
    row_major float4x4 ViewMat;
    row_major float4x4 MVP;
};
struct PSinput
{
    float4 position : SV_Position;
};
PSinput main(VSinput input)
{

    float4x4 mat = mul(WorldMat, LightViewProjection);
    PSinput output = (PSinput) 0;
    output.position = mul(float4(input.position.xyz, 1.0f), mat); // X Y Z W

    return output;
}