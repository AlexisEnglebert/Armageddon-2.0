
cbuffer TransFormBuffer : register(b0)
{
    row_major float4x4 WorldMat;
    row_major float4x4 ProjectionMat;
    row_major float4x4 ViewMat;
    row_major float4x4 MVP;
};

struct VSinput
{
    float3 position : POSITION;
    float2 textCoord : TEXCOORD0;

};

struct PSinput
{
    float4 position : SV_Position;
    float2 textCoord : TEXCOORD0;

};
PSinput main(VSinput input)
{
    float4x4 matrice = mul(WorldMat, MVP);

    PSinput output = (PSinput) 0;

    
    float4x4 modelView = mul(WorldMat, ViewMat);
    // Column 0:
    modelView[0][0] = 1;
    modelView[0][1] = 0;
    modelView[0][2] = 0;

// Column 1:
    modelView[1][0] = 0;
    modelView[1][1] = 1;
    modelView[1][2] = 0;

// Column 2:
    modelView[2][0] = 0;
    modelView[2][1] = 0;
    modelView[2][2] = 1;
    
    float4x4 MVP_ = mul(modelView, ProjectionMat);
    output.position = mul(float4(input.position.xyz, 1.0f), MVP_);
    output.textCoord = input.textCoord;

    return output;
}