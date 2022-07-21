#include<Tonemap.hlsli>
cbuffer BloomProperty : register(b4)
{
    float2 TexelSize;
};

struct PSinput
{
    float4 position : SV_Position;
    float2 textCoord : TEXCOORD0;
};

Texture2D FrameBuffer : register(t0);
Texture2D BloomTexture : register(t1);

SamplerState Sampler : register(s0);

float4 main(PSinput input) : SV_TARGET
{
    float4 FrameTex = FrameBuffer.Sample(Sampler, input.textCoord);
    
    
    float4 d = TexelSize.xyxy * float4(1.0, 1.0, -1.0, 0.0);

    float4 s;
     s = BloomTexture.Sample(Sampler, (input.textCoord - d.xy));
    s += BloomTexture.Sample(Sampler, (input.textCoord - d.wy)) * 2.0;
    s += BloomTexture.Sample(Sampler, (input.textCoord - d.zy));

    s += BloomTexture.Sample(Sampler, (input.textCoord + d.zw)) * 2.0;
    s += BloomTexture.Sample(Sampler, (input.textCoord)) * 4.0;
    s += BloomTexture.Sample(Sampler, (input.textCoord + d.xw)) * 2.0;

    s += BloomTexture.Sample(Sampler, (input.textCoord + d.zy));
    s += BloomTexture.Sample(Sampler, (input.textCoord + d.wy)) * 2.0;
    s += BloomTexture.Sample(Sampler, (input.textCoord + d.xy));

    float3 color =  (s * (1.0 / 16.0));
    color = uncharted2_filmic(color.rgb);

    return float4(color + FrameTex.rgb,1.0f);
}