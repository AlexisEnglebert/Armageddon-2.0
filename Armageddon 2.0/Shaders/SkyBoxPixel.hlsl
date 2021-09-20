struct PSinput
{
    float4 position : SV_Position;
    float3 textCoord : TEXCOORD0;

};
TextureCube SkyboxTexture : register(t17);
//Texture2D SkyboxTexture : register(t0);

SamplerState Sampler : register(s0);
float3 ReinhardToneMap(float3 color, float max_white)
{
    float White2 = max_white * max_white;
    float3 numerator = color * (1.0f + (color / float3(White2, White2, White2)));
    float denominator = (color + 1.0f);
    return numerator / denominator;

}
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
    float3 pixelColor = SkyboxTexture.SampleLevel(Sampler, input.textCoord,6); //* Intensity
    /*pixelColor = pixelColor / (pixelColor + 1.0f);
    pixelColor = pow(pixelColor, (1.0f / 2.2f));*/
   // pixelColor = pixelColor / (1 + pixelColor);
    pixelColor = uncharted2_filmic(pixelColor);
    pixelColor = pow(pixelColor, 1 / 2.22);
    return float4(pixelColor,1.0f);

}

