struct VSinput
{
    float3 position  : POSITION;
    float2 textCoord : TEXCOORD0;


};
struct PSinput
{
    float4 position : SV_Position;
    float2 textCoord : TEXCOORD0;
};

PSinput main(VSinput input)
{
    PSinput output = (PSinput)0;
    output.position = float4(input.position, 1.0f);
    output.textCoord = input.textCoord;
	return output;
}