struct PSinput
{
    float4 position : SV_Position;
    float2 textCoord : TEXCOORD0;
};

Texture2D FinalImage : register(t0);
SamplerState Sampler : register(s0);

float4 main(PSinput input) : SV_TARGET
{
    float4 color = FinalImage.Sample(Sampler,input.textCoord);
	return color;
}