struct PSinput
{
    float4 position : SV_Position;
};
float4 main(PSinput input) : SV_TARGET
{
    return float4(input.position.z / input.position.w,input.position.z / input.position.w,input.position.z / input.position.w,1.0f);
}