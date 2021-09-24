struct PSinput
{
    float4 position : SV_Position;
};
float4 main(PSinput input) : SV_TARGET
{
    return float4(0.0f,0.0f,0.0f,1.0f);
}