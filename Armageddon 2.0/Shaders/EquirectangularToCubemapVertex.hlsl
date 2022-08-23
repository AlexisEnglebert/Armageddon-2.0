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

struct VSinput
{
    uint VertexID : SV_VERTEXID;
};

struct PSinput
{
    float4 position : SV_Position;
    float3 DefPosition : POSITION;

};

float3 CreateCube(uint vertexID)
{
    uint b = 1 << vertexID;
    float x = (0x287a & b) != 0;
    float y = (0x02af & b) != 0;
    float z = (0x31e3 & b) != 0;
    return float3(x, y, z);
}

PSinput main(VSinput input)
{
    PSinput output = (PSinput) 0;
    float3 pos = CreateCube(input.VertexID) - float3(0.5, 0.5, 0.5);
    float4x4 mat = mul(ViewMat, ProjectionMat);
    output.position = mul(float4(pos, 1.0f), mat);
    output.DefPosition = pos;
    
    return output;
}