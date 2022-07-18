#include <Tonemap.hlsli>
struct PSinput
{
    float4 position : SV_Position;
    float3 textCoord : TEXCOORD0;

};
TextureCube SkyboxTexture : register(t17);
//Texture2D SkyboxTexture : register(t0);

SamplerState Sampler : register(s0);

float4 main(PSinput input) : SV_TARGET
{
    float3 pixelColor = SkyboxTexture.SampleLevel(Sampler, input.textCoord,6); //* Intensity
    pixelColor = uncharted2_filmic(pixelColor);
    return float4(pixelColor,1.0f);

}

