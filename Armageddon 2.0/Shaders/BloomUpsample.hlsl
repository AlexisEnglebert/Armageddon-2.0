cbuffer BloomProperty : register(b4)
{
	float2 TexelSize;
};
struct PSinput
{
	float4 position : SV_Position;
	float2 textCoord : TEXCOORD0;
};

Texture2D  CombineTex : register(t1);
Texture2D BloomTexture : register(t0);
SamplerState Sampler : register(s0);

float3 uncharted2_tonemap_partial(float3 x)
{
    float A = 0.15f;
    float B = 0.50f;
    float C = 0.10f;
    float D = 0.20f;
    float E = 0.02f;
    float F = 0.30f;
    return ((x * (A * x + C * B) + D * E) / (x * (A * x + B) + D * F)) - E / F;
}

float3 uncharted2_filmic(float3 v)
{
    float exposure_bias = 2.0f;
    float3 curr = uncharted2_tonemap_partial(v * exposure_bias);

    float3 W = float3(11.2f, 11.2f, 11.2f);
    float3 white_scale = float3(1.0f, 1.0f, 1.0f) / uncharted2_tonemap_partial(W);
    return curr * white_scale;
}

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
   // combine = float4(uncharted2_filmic(combine.rgb), 1.0f);
  //  combine = pow(abs(combine), 1 / 2.22);
	return combine;
}