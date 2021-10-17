cbuffer BloomProperty : register(b4)
{
    float2 TexelSize;
};
struct PSinput
{
    float4 position : SV_Position;
    float2 textCoord : TEXCOORD0;
};

Texture2D BloomTexture : register(t0);
SamplerState Sampler : register(s0); 

float4 main(PSinput input) : SV_TARGET
{
    //13 box tap downSampling From : http://goo.gl/eomGso

   // float3 BloomTexture = BloomTexture.Sample(Sampler , ,input.textCoord + TexelSize);


	float4 A = BloomTexture.Sample(Sampler, input.textCoord + TexelSize * float2(-1.0, -1.0));
	float4 B = BloomTexture.Sample(Sampler, input.textCoord + TexelSize * float2(0.0, -1.0));
	float4 C = BloomTexture.Sample(Sampler, input.textCoord + TexelSize * float2(1.0, -1.0));
	float4 D = BloomTexture.Sample(Sampler, input.textCoord + TexelSize * float2(-0.5, -0.5));
	float4 E = BloomTexture.Sample(Sampler, input.textCoord + TexelSize * float2(0.5, -0.5));
	float4 F = BloomTexture.Sample(Sampler, input.textCoord + TexelSize * float2(-1.0, 0.0));
	float4 G = BloomTexture.Sample(Sampler, input.textCoord);
	float4 H = BloomTexture.Sample(Sampler, input.textCoord + TexelSize * float2(1.0, 0.0));
	float4 I = BloomTexture.Sample(Sampler, input.textCoord + TexelSize * float2(-0.5, 0.5));
	float4 J = BloomTexture.Sample(Sampler, input.textCoord + TexelSize * float2(0.5, 0.5));
	float4 K = BloomTexture.Sample(Sampler, input.textCoord + TexelSize * float2(-1.0, 1.0));
	float4 L = BloomTexture.Sample(Sampler, input.textCoord + TexelSize * float2(0.0, 1.0));
	float4 M = BloomTexture.Sample(Sampler, input.textCoord + TexelSize * float2(1.0, 1.0));

	half2 div = (1.0 / 4.0) * half2(0.5, 0.125);

	float4 o = (D + E + I + J) * div.x;
	o += (A + B + G + F) * div.y;
	o += (B + C + H + G) * div.y;
	o += (F + G + L + K) * div.y;
	o += (G + H + M + L) * div.y;

	//

    /*float4 d = TexelSize.xyxy * float4(-1.0, -1.0, 1.0, 1.0);
    float4 s = BloomTexture.Sample(Sampler, (input.textCoord + TexelSize + d.xy ));
    s += BloomTexture.Sample(Sampler, (input.textCoord + TexelSize + d.zy));
    s += BloomTexture.Sample(Sampler, (input.textCoord + TexelSize + d.xw));
    s += BloomTexture.Sample(Sampler, (input.textCoord + TexelSize + d.zw));*/

    float4 kernel = TexelSize.xyxy * float4(-1,-1,1,1);
    float4 color = BloomTexture.Sample(Sampler, input.textCoord  +  kernel.xy);
    color += BloomTexture.Sample(Sampler, input.textCoord + kernel.zy);
    color += BloomTexture.Sample(Sampler, input.textCoord + kernel.xw);
    color += BloomTexture.Sample(Sampler, input.textCoord + kernel.zw);
	float4 a = BloomTexture.Sample(Sampler, input.textCoord);


    return o ;
}