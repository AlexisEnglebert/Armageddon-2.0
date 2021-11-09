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


    //TEST CELUI DE OPENGL : 
    float knee = 0.1f;
    float threshold = 0.8f;
    float3 curve = float3(threshold - knee, knee * 2, 0.25 / knee);

    float br =color ;
    float rq = clamp(br - curve.x, 0.0, curve.y);
    rq = curve.z * rq * rq;

    // Combine and apply the brightness response curve
    color *= max(rq, br - threshold) / max(br, 1e-4);
	return float4(color, 1.0f);
}