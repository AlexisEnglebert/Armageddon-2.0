
cbuffer TransFormBuffer : register(b0)
{
    row_major float4x4  WorldMat;
};
cbuffer CameraBuffer : register(b2)
{
    row_major float4x4  ProjectionMat;
    row_major float4x4  ViewMat;
    row_major float4x4  MVP;
    row_major float4x4 InverseProjectionMat;
    row_major float4x4 InverseViewMat;
    row_major float4x4 InverseMVP;

    float3 CameraPos; //12 
    float nearPlane;
    float farPlane;
};
cbuffer WorldCBuffer : register(b4)
{
    float time;
}

struct VSinput
{
    float3 position  : POSITION;
    float2 textCoord : TEXCOORD0;
    float3 normal    : NORMAL;
    float3 Tangent   : TANGENT;
    float3 Binormal  : BINORMAL;
    
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
    int PointLightCount;
    int DirectionalLightCount;
    float2 padding1;


    PointLight PointLights[50];
    DirectionalLight DirectionalLights[50];
    row_major float4x4 LightViewProjectionCascade[3]; // TODO BETTER HANDLING OF CASCADE NUM
    float3 FarPlane; // attention à l'alignement :D 
    int cascadeIndice;

};

struct PSinput
{
    float4 position : SV_Position;
    float2 textCoord : TEXCOORD0;
    float3 normal    : NORMAL;
    float3 Tangent   : TANGENT;
    float3 Binormal  : BINORMAL;
	float3 WorldPos : POSITION0;
    float4 LightPosition[3] : LIGHTPOS;
    float3 WordNormal : NORMALPOS;
    float4 ViewPos : POSITION1;
};
PSinput main(VSinput input)
{
   // float4x4 _MVP = mul(ProjectionMat, ViewMat);
    float4x4 matrice = mul(WorldMat, MVP);

    PSinput output = (PSinput) 0;
    output.position = mul(float4(input.position.xyz, 1.0f), matrice ); // X Y Z W
    //output.position = mul(float4(input.position.xyz, 1.0f), mat); // X Y Z W
    output.textCoord = input.textCoord;
    output.normal = input.normal;
    output.Tangent = input.Tangent;
    output.Binormal = input.Binormal;
    output.WorldPos = mul(float4(input.position,1.0f),WorldMat).xyz;
    output.WordNormal = input.normal;
 
    for (unsigned int i = 0; i < 3; i++)
    {
        float4x4 mat = mul(WorldMat, LightViewProjectionCascade[i]);
        output.LightPosition[i] = mul(float4(input.position.xyz, 1.0f), mat); // X Y Z W
    }
    float4x4 ViewTransformMat = mul(WorldMat,ViewMat);
    output.ViewPos = mul(float4(input.position, 1.0f), ViewTransformMat);


    return output;
}
