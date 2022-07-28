cbuffer TransFormBuffer : register(b0)
{
    row_major float4x4  WorldMat;
    row_major float4x4  ProjectionMat;
    row_major float4x4  ViewMat;
    row_major float4x4  MVP;
    row_major float4x4 InverseProjectionMat;

};
cbuffer WorldCBuffer : register(b4)
{
    float time;
}


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
struct PSinput
{
    float4 position : SV_Position;
    float2 textCoord : TEXCOORD0;
    float3 normal    : NORMAL;
    float3 Tangent   : TANGENT;
    float3 Binormal  : BINORMAL;
    float3 WorldPos : POSITION0;
    float4 LightPosition : LIGHTPOS;
    float3 WordNormal : NORMALPOS;
};

struct VSinput
{
    float3 position  : POSITION;
    float2 textCoord : TEXCOORD0;
    float3 normal    : NORMAL;
    float3 Tangent   : TANGENT;
    float3 Binormal  : BINORMAL;
    
};


PSinput main(VSinput input)
{
    float4x4 matrice = mul(WorldMat, MVP);

    PSinput output = (PSinput) 0;

    output.position = mul(float4(input.position.xyz, 1.0f), matrice ); 

    output.textCoord = input.textCoord;

    output.normal = mul(input.normal, (float3x3)WorldMat);
    output.Tangent = mul(input.Tangent, (float3x3) WorldMat);
    output.Binormal = mul(input.Binormal, (float3x3) WorldMat);
    output.WorldPos = mul(input.position, (float3x3) WorldMat);
    output.WordNormal = input.normal;

    float4x4 mat = mul(WorldMat, LightViewProjection);
    output.LightPosition = mul(float4(input.position.xyz, 1.0f), mat); 
    return output;
}