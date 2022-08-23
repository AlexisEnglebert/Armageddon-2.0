
//TODO FILE WITH ALL CBUFFER

struct PointLight
{
	//On alterne pour rester alligner sur 16 bytes
	float3 Position; //12
	float Intensity; //4
	float3 Color; //12
	float  Radius; //4
};

struct DirectionalLight
{
	float3 Direction; //12
	float Intensity; //4
	float3 Color; //12
	float padding0; //4
};

cbuffer CameraBuffer : register(b2)
{
    row_major float4x4  ProjectionMat;
    row_major float4x4  ViewMat;
    row_major float4x4  MVP;
    row_major float4x4 InverseProjectionMat;
    row_major float4x4 InverseViewMat;
    row_major float4x4 InverseMVP; //Should be Named InverVP because there is no model here ....

    float3 CameraPos; //12 
    float nearPlane;
    float farPlane;
};

cbuffer LightCBuffer : register(b1)
{
    int PointLightCount;
    int DirectionalLightCount;
    float2 padding1;


    PointLight PointLights[50];
    DirectionalLight DirectionalLights[50];
    row_major float4x4 LightViewProjectionCascade[3]; // TODO BETTER HANDLING OF CASCADE NUM
    float3 FarPlaneSplit; // attention à l'alignement :D 
    int cascadeIndice;

};

cbuffer WorldCBuffer : register(b5)
{
    float3 Scattering;
    float Extinction;				//16
    float3 Emissive;		// 28
    float Phase;					//32
    float LerpFactor;		//1
    float density;
    float ambientFog;
    bool EnableVolumetricFog;
    float Intensity;
    float anisotropy;
}

RWTexture3D<float4> m_VolumetricIntegration; //This could may be half4 because of the 3D texture which is RGBA16F
Texture2D shadowMap : register(t1);
SamplerState Sampler : register(s0);

static const float PI = 3.1415926535897932384626433832795f;



//CE CODE VIENT PAS DE MOI : https://github.com/diharaw/volumetric-lighting/blob/main/src/shaders/light_injection_cs.glsl
//j'essaye juste de comprendre comment ça marche tout ça ! 
float linear_01_to_exp_01_depth(float z, float n, float f)
{
    float z_buffer_params_y = f / n;
    float z_buffer_params_x = 1.0f - z_buffer_params_y;

    return (1.0f / z - z_buffer_params_y) / z_buffer_params_x;
}

float4 get_worldpos_from_id(uint3 id, float near, float far)
{
    //retrieve UV from the 3D textures which is in (160X90X128)
    //Cependant, j'en ai aucune idée de pourquoi en divise par la dimension-1; 
    //Bha oui je suis con car l'id commence à 0 et donc on veut entre [-1 et 1] donc pour avoir 1 il faut diviser par frog - 1
    //TODO Ajouter une constante jitter ! 
    float viewZ = near * pow(far / near, (float(id.z)) / float(127.0f));
    float3 uv = float3(id.x/159.0f, id.y/89.0f, viewZ/far);
    float4 ndc;

    ndc.x = (uv.x * 2.0f) - 1.0f;
    ndc.y = 1.0f - (uv.y * 2.0f); //DirectX 
    ndc.z = 2.0f * linear_01_to_exp_01_depth(uv.z, near, far) - 1.0f;
    ndc.w = 1.0f;

    float4 position = mul(ndc, InverseMVP);
    float4 Worldposition = position / position.w;


    return Worldposition;
}

float CalculatePointLightEvaluation(float dist, float radius, float intensity)
{
    /*
    * Inverse square law(I / distance²) on ajouté un Bias
    * Car plus la distance est grande plus l'attenuation est petite donc on fixe une valeur pour limiter les calculs
    */

    float DistanceSquare = dist * dist;
    float attenuation = intensity / max(DistanceSquare, pow(0.1, 2));
    float factor = DistanceSquare / radius;
    float SmoothFactor = saturate(1 - factor * factor);
    attenuation *= SmoothFactor * SmoothFactor;

    return attenuation;

}

float3 phase(int indice, float3 inVectorDir, float3 OutVectorDir)
{
    float output;
    float angle = length(inVectorDir) * length(OutVectorDir);
    float cos2 = cos(angle) * cos(angle);
    /*
    * 1 - Rayleigh
    * 2 - Mie
    * 3 - geometric
    */
    switch (indice)
    {
    case 1: //Rayleigh
    default:
        float scalar = 3 / 16 * PI;
        output = scalar * (1 + cos2);
        break;

    case 2: //Mie

        break;
    case 3:
        break;
    }

    return output;
}

float3 ParsePointLight(float3 position)
{
    float3 cout = float3(0.0f,0.0f,0.0f);
    for(uint i = 0 ; i < PointLightCount ; i++)
    {
        float3 LightDir = PointLights[i].Position - position;
        float3 ViewDir = normalize(CameraPos.xyz - position);
        float _distance = length(LightDir);
        float attenuation = CalculatePointLightEvaluation(_distance,PointLights[i].Radius,PointLights[i].Intensity); //TODO TYPO
        float3 PhaseFunction = phase(1, ViewDir, LightDir);
        cout += PointLights[i].Color * attenuation /** PhaseFunction*/;
    }
    return cout;
}

//Visibility function from directional light because we don't have shadow map for pointLight & for spotLight
//v(x, plighti) = shadowMap(x, plighti) · volShad(x, plighti)
//volShad(x, plighti) = Tr(x, plighti).
float phaseFunction()
{
    return 1.0 / (4.0 * 3.14);
}

float VisibilityFunction(float3 Worldposition)
{
    float3 ProjCoords = 0;
    float4 lightSpacePos = mul(float4(Worldposition, 1.0f), LightViewProjectionCascade[2]);
    float shadow = 0.0f;
    ProjCoords.x = lightSpacePos.x / lightSpacePos.w / 2.0f + 0.5f;
    ProjCoords.y = -lightSpacePos.y / lightSpacePos.w / 2.0f + 0.5f;
    ProjCoords.z = lightSpacePos.z / lightSpacePos.w;

    shadow = shadowMap.SampleLevel(Sampler, ProjCoords,0);
    if (shadow < ProjCoords.z + 0.00001)
        return 0.0;
    else
        return 1.0;
}
float HenyeyGreensteinPhaseFunction(float3 viewDir, float3 lightDir, float g)
{
    float cos_theta = dot(viewDir, lightDir);
    float denom = 1.0f + g * g + 2.0f * g * cos_theta;
    return (1.0f / (4.0f * PI)) * (1.0f - g * g) / max(pow(denom, 1.5f), 0.00001);
}

float3 ParseDirectionalLight(float3 position,float3 viewDirection)
{
    float3 cout = float3(0.0f,0.0f,0.0f);
    float att = 1;
    for(uint i = 0 ; i < DirectionalLightCount ; i++)
    {
        //TODO WITH SHADOW MAP;
        att *= VisibilityFunction(position);
        cout += DirectionalLights[i].Color /* HenyeyGreensteinPhaseFunction(viewDirection, DirectionalLights[i].Direction,anisotropy)*/;
    }
    return cout * att;
}



[numthreads(8, 2, 8)] //TO SEE with the 3D texture // 16x2x16 thread ???
void main( uint3 id : SV_DispatchThreadID )
{

    float3 color = float3(ambientFog.xxx);

	//Get the world position of the Froxel
	//We need the near and far plane frome the frustum (d'ou le mot Froxel pour frustum voxel)
	float4 pos = get_worldpos_from_id(id, nearPlane, farPlane);


    float3 Wo = normalize(CameraPos.xyz - pos);
    /*float visibility = VisibilityFunction(pos);
    if(visibility > 0.000001)
        color += visibility * DirectionalLights[0].Color * phase(1, Wo, -DirectionalLights[0].Direction.xyz);
        */

    
    //So for the moment the density is just a value sended via the CPU to the GPU

    
    if(DirectionalLightCount > 0)
    {
        color.rgb += ParseDirectionalLight(pos, Wo);
    }
  
    if(PointLightCount > 0)
    {
        color.rgb += ParsePointLight(pos);
    }

    float4 result = float4(color * Intensity * density, density);

    //The color W or A is the FogDensity
    //id.z = 2;
    m_VolumetricIntegration[id] = result;

}