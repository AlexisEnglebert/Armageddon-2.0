struct PSinput
{
    float4 position : SV_Position;
    float3 DefPosition : POSITION;

};

TextureCube SkyboxTexture : register(t0);
SamplerState Sampler : register(s0);

//https://learnopengl.com/PBR/IBL/Diffuse-irradiance

const static float PI = 3.14159265359;



float4 main(PSinput input) : SV_TARGET
{
    /*Tout est inversé , on doit flip !*/
    float3 normal = normalize(input.DefPosition);
    float3 irradiance = float3(0.0f,0.0f,0.0f);
    
    float3 up = float3(0.0f, 1.0f, 0.0f);
    float3 right = normalize(cross(up, normal));
    up = normalize(cross(normal, right));
    
    float sampleDelta = 0.025;
    float nrSamples = 0.0;
    for (float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
    {
        for (float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
        {
        // spherical to cartesian (in tangent space)
            float3 tangentSample = float3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
        // tangent space to world
            float3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal    ;

            irradiance += SkyboxTexture.Sample(Sampler, sampleVec) * cos(theta) * sin(theta);
            nrSamples++;
        }
    }
    irradiance = PI * irradiance * (1.0 / float(nrSamples));

    

    
    float4 color = float4(irradiance, 1.0f);
    
    return float4(color);

}
