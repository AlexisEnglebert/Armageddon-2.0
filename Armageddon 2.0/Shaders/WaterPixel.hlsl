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

cbuffer WorldCBuffer : register(b4)
{
    float time;
}

Texture2D DepthMap : register(t0);
Texture2D Normal1 : register(t1);

SamplerState Sampler : register(s0);


float4 main(PSinput input) : SV_TARGET
{
   float WaterSpeed = 0.01f;
   float4 normal1 = Normal1.Sample(Sampler,input.textCoord + time *WaterSpeed );
   float4 normal2 = Normal1.Sample(Sampler,input.textCoord - time *WaterSpeed);
   float3 normalMap = normal1.rgb * normal2.rgb;
   //Perform normal calculation
   normalMap = (normalMap * 2.0f) - float3(1.0f, 1.0f, 1.0f);
   float3x3 TBN = float3x3(normalize(input.Tangent), normalize(input.Binormal), normalize(input.normal));
   float3 BumpNormal = mul(transpose(TBN), normalMap);
   BumpNormal = normalize(BumpNormal);

   float4 depth = DepthMap.Sample(Sampler,input.textCoord);
   float4 result = depth - float4(input.position.xyz,1.0f);

   return result;
}