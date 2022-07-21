struct PSinput
{
    float4 position : SV_Position;
};
float4 main(PSinput input) : SV_TARGET
{
    return input.position.z / input.position.w;
}