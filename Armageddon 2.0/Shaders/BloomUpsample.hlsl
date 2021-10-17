cbuffer BloomProperty : register(b4)
{
	float2 TexelSize;
};
struct PSinput
{
	float4 position : SV_Position;
	float2 textCoord : TEXCOORD0;
};

Texture2D  CombineTex : register(t0);
Texture2D BloomTexture : register(t1);
SamplerState Sampler : register(s0);

float4 main(PSinput input) : SV_TARGET
{
	//3x3 tent filter  From : http://goo.gl/eomGso
    float4 d = TexelSize.xyxy * float4(1.0, 1.0, -1.0, 0.0);

    float4 s;
    s = BloomTexture.Sample( Sampler, (input.textCoord - d.xy));
    s += BloomTexture.Sample( Sampler, (input.textCoord - d.wy)) * 2.0;
    s += BloomTexture.Sample( Sampler, (input.textCoord - d.zy));

    s += BloomTexture.Sample( Sampler, (input.textCoord + d.zw)) * 2.0;
    s += BloomTexture.Sample( Sampler, (input.textCoord)) * 4.0;
    s += BloomTexture.Sample( Sampler, (input.textCoord + d.xw)) * 2.0;

    s += BloomTexture.Sample( Sampler, (input.textCoord + d.zy));
    s += BloomTexture.Sample( Sampler, (input.textCoord + d.wy)) * 2.0;
    s += BloomTexture.Sample( Sampler, (input.textCoord + d.xy));

   //COMBINE 2 TEXTURES:
    float4 combine = CombineTex.Sample(Sampler, input.textCoord);
    combine = combine + (s * (1.0 / 16.0));
	return combine;
}