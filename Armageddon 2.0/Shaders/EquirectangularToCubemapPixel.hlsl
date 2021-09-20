struct PSinput
{
    float4 position : SV_Position;
    float3 DefPosition : POSITION;

};

Texture2D HDRmap : register(t0);
SamplerState Sampler : register(s0);

//https://learnopengl.com/PBR/IBL/Diffuse-irradiance

static const float2 invAtan = float2(0.1591, 0.3183);
float2 SampleSphericalMap(float3 v)
{
    float2 uv = float2(atan2(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5f; 
    return uv;
}

float4 main(PSinput input) : SV_TARGET
{
    /*Tout est inversé , on doit flip !*/
    float2 UV = SampleSphericalMap(-normalize(input.DefPosition));
    float3 color = HDRmap.Sample(Sampler, UV.xy).rgb;
    
    return float4(color,1.0f);

}
