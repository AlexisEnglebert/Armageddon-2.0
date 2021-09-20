

struct PSinput
{
    float4 position : SV_Position;
    float2 textCoord : TEXCOORD0;

};
Texture2D AlbedoMap : register(t0);
SamplerState Sampler : register(s0);

float4 main(PSinput input) : SV_TARGET
{
    float4 AlbedoTex = AlbedoMap.Sample(Sampler, input.textCoord);
    return AlbedoTex;
}