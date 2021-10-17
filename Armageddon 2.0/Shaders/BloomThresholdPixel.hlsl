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
    // ON DOIT IMPROVE CA
    float brightness = dot(color.rgb, float3(0.2126, 0.7152, 0.0722));
    if (brightness > 0.9F) color = 1.0F * color.rgb, 1.0;
    else color = float4(0.0F, 0.0F, 0.0F, 1.0F);
	return float4(color, 1.0f);
}