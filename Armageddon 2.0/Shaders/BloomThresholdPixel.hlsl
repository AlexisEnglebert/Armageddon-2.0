struct PSinput
{
    float4 position : SV_Position;
    float2 textCoord : TEXCOORD0;
};
Texture2D FrameBuffer : register(t0);
SamplerState Sampler : register(s0);

float4 main(PSinput input) : SV_TARGET
{
    float3 color = FrameBuffer.Sample(Sampler,input.textCoord).rgb;

    float knee = 0.5f;
    float threshold = 0.9f;
    float3 curve = float3(threshold - knee, knee * 2, 0.25 / knee);

    float br =color ;
    float rq = clamp(br - curve.x, 0.0, curve.y);
    rq = curve.z * rq * rq;

    // Combine and apply the brightness response curve
    color *= max(rq, br - threshold) / max(br, 1e-4);


    //Test 
   /* float4 BrightColor;
    float brightness = dot(color, float3(0.2126, 0.7152, 0.0722));
    if (brightness > 0.9f)
        BrightColor = float4(color, 1.0);
    else
        BrightColor = float4(0.0, 0.0, 0.0, 1.0);*/

	return float4(color,1.0f);
}