#include <Tonemap.hlsli>
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

cbuffer LightCBuffer : register(b1)
{
    float3 CameraPos;
	float Padding0;
	
    int PointLightCount;
    int DirectionalLightCount;
	float2 padding1;


    PointLight PointLights[50];
    DirectionalLight DirectionalLights[50];
    row_major float4x4 LightViewProjection;

};

cbuffer MaterialCBuffer : register(b3)
{
    float Roughness;//4
    float Metalic;//8
    bool UseMetalMap;//9
    float _Padding0;//13
    bool UseEmisive;//14
    float3 AlbedoTint; //26
    //float _Padding1;//30
    //bool _Padding3; //31
    //bool _Padding2; //32
    float EmisiveFactor;//4
    float3 EmisiveTint;
};

cbuffer WorldCBuffer : register(b4)
{
    float time;
}

struct PSinput
{
    float4 position : SV_Position;
    float2 textCoord : TEXCOORD0;
    float3 normal    : NORMAL;
    float3 Tangent   : TANGENT;
    float3 Binormal  : BINORMAL;
    float3 WorldPos : POSITION0;
    float4 LightPosition : LIGHTPOS;
    float3 WordNormal : NORMALPOS;
};


Texture2D AlbedoMap : register(t0);
Texture2D NormalMap : register(t1);
Texture2D SpecularMap : register(t2);

Texture2D MetalicMap : register(t3);
Texture2D AmbiantOMap : register(t4);
Texture2D EmissiveMap : register(t5);

Texture2D TestColor : register(t20);
Texture2D TestNormal : register(t21);
Texture2D TestSpec : register(t22);


TextureCube irradianceMap : register(t50);
TextureCube Prefiltered : register(t51);
Texture2D   BRDFIntegration : register(t52);
Texture2D ShadowMap   : register(t53);


SamplerState Sampler : register(s0);
SamplerState ClampSampler : register(s1);



static const float PI = 3.1415926535897932384626433832795f;
//static const float Metalic = 0.4f;
//static const float Roughness = 1.0f;

/*
https://blog.selfshadow.com/publications/s2013-shading-course/karis/s2013_pbs_epic_notes_v2.pdf
*/

float CalculateShadow(float4 LightPosition)
{
    float3 ProjectedCoords = 0;
    float bias = 0.001f;

    
    ProjectedCoords.x = LightPosition.x / LightPosition.w / 2.0f + 0.5f;
    ProjectedCoords.y = -LightPosition.y / LightPosition.w / 2.0f + 0.5f;
    float shadow = 0.0f;
    if ((saturate(ProjectedCoords.x) == ProjectedCoords.x) && (saturate(ProjectedCoords.y) == ProjectedCoords.y))
    {
        float ClosestDepth = ShadowMap.Sample(Sampler, ProjectedCoords.xy).r;
        float lightDepthValue = LightPosition.z / LightPosition.w;
        lightDepthValue = lightDepthValue - bias;
        
        shadow = ClosestDepth;

    }
    
    return shadow;

}

uint QuerySpecularTextureLevels()
{
    uint width, height, levels;
    Prefiltered.GetDimensions(0, width, height, levels);
    return levels;
}

//Disney’s GGX/Trowbridge-Reitz
float NormalDistribution(float3 Normal, float3 HalfWay, float Roughness)
{
    float Roughness2 = Roughness * Roughness;
	float NormalDotHalf = saturate(dot(Normal, HalfWay));
    float NormalDotHalf2 = NormalDotHalf * NormalDotHalf;

    float Numerateur = Roughness2;
    float Denominateur = (NormalDotHalf2 * (Roughness2 - 1.0) + 1.0);
    Denominateur = (Denominateur * Denominateur) * 1.0f/PI; //ajouter PI ? 

    return Numerateur / Denominateur;
}

float GeometrySchlickGGX(float NormalDotView, float Roughness)
{
	float nom = NormalDotView;
	float denom = NormalDotView * (1.0 - Roughness) + Roughness;
	
    return nom / denom;
}
  
float GeometrySmith(float3 Normal, float3 View, float3 Light, float Roughness)
{
	float NdotV = abs(dot(Normal, View)) + 1e-5;
	float NdotL = saturate(dot(Normal, Light));
	float ggx1 = GeometrySchlickGGX(NdotV, Roughness);
	float ggx2 = GeometrySchlickGGX(NdotL, Roughness);
	
	return ggx1 * ggx2;
}

float3 fresnelSchlick(float cosTheta, float3 F0)
{
    float f = pow(clamp(1.0 - cosTheta,0.0f,1.0f), 5.0f);
    return f + F0 * (1.0f - f);
}

float3 fresnelSchlickRoughness(float cosTheta, float3 F0, float roughness)
{
    //return max(float3(1.0 - roughness, 1.0 - roughness, 1.0 - roughness), F0) - F0;
    //return F0 + (max(float3(1.0 - roughness, 1.0 - roughness, 1.0 - roughness), F0) - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
   // return (max(float3(1.0 - roughness, 1.0 - roughness, 1.0 - roughness), F0) - F0);
   return F0 + (max(float3(1.0 - roughness.x, 1.0 - roughness.x, 1.0 - roughness.x), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);

}

float CalculatePointLightEvaluation(float dist,float radius,float intensity)
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


float3 CalculateBRDF(PSinput input,float3 View, float4 AlbedoTex,float3 Normal,float RoughnessTex,float AmbiantOclusionTex,float MetalicTex,float3 Light,float3 F0)
{
    float3 Halfway = normalize(View + Light);
        
    float D = NormalDistribution(Normal, Halfway, RoughnessTex); //0.1 est une valeur de TEST (Roughness)
    float G = GeometrySmith(Normal, View, Light, RoughnessTex); //0.1 est une valeur de TEST (Roughness)
    float3 F = fresnelSchlick(saturate(dot(Halfway, View)), F0);
        
        
    float3 kS = F;
    float3 kD = float3(1.0, 1.0, 1.0) - kS;
    kD *= 1.0 - MetalicTex; // 0.04 est une valeur de TEST (metalic)
        
        
    float3 Numerateur = D * F * G;
    float Denominateur = 4.0f * abs(dot(Normal, View)) + 1e-5 * saturate(dot(Normal, Light)) + 0.001f;
    float3 specular = Numerateur / Denominateur;
        
    float NdotL = saturate(dot(Normal, Light));
        
    return (kD * AlbedoTex.rgb / PI + specular)  * NdotL;
}
float4 main(PSinput input) : SV_TARGET
{
     
    float4 AlbedoTex            = AlbedoMap.Sample(Sampler, input.textCoord);
    float3 NormalTex            = NormalMap.Sample(Sampler, input.textCoord);
    float RoughnessTex          = SpecularMap.Sample(Sampler, input.textCoord).r;
    float AmbiantOclusionTex    = AmbiantOMap.Sample(Sampler, input.textCoord);


    AmbiantOclusionTex = AmbiantOclusionTex != 0 ? AmbiantOclusionTex : 1.0f;
    
    float MetalicTex = UseMetalMap ? MetalicMap.Sample(Sampler, input.textCoord).r : float3(0.04f, 0.04f, 0.04f);
    float3 EmisiveMap = UseEmisive ? EmissiveMap.Sample(Sampler, input.textCoord) : float3(0.0f, 0.0f, 0.0f);
   // EmisiveMap *= EmisiveTint;
  //  RoughnessTex = RoughnessTex != 1 ? RoughnessTex : Roughness;
    //NormalTex = NormalTex != 0 ? NormalTex : normalize(input.normal);
    //AlbedoTex = AlbedoTex != 0 ? AlbedoTex : float3(1.0f, 1.0f, 1.0f);
    AlbedoTex *= float4(AlbedoTint, AlbedoTex.a);
    
    
    //MetalicTex = Metalic;
   // RoughnessTex = Roughness;
    
    NormalTex = (NormalTex * 2.0f) - float3(1.0f, 1.0f, 1.0f);
    float3x3 TBN = float3x3(normalize(input.Tangent), normalize(input.Binormal), normalize(input.normal));
    float3 BumpNormal = mul(transpose(TBN), NormalTex);

    //float3 BumpNormal = (NormalTex.x * normalize(input.Tangent)) + (NormalTex.y * normalize(input.Binormal)) + (NormalTex.z * normalize(input.normal));
    BumpNormal = normalize(BumpNormal);
    

   
      float3 LightOut = float3(0.0f,0.0f,0.0f);
	  float3 F0 = float3(0.04f, 0.04f, 0.04f);
      float3 Normal = BumpNormal;
      float3 camPosNormal = float3(CameraPos.x, CameraPos.y, CameraPos.z);
	  float3 View = normalize(camPosNormal - input.WorldPos);

      F0 = lerp(F0, AlbedoTex.rgb, MetalicTex);
      for (uint i = 0; i < PointLightCount; i++)         
      {
        
        float3 Light = normalize(PointLights[i].Position - input.WorldPos);
        float distance = length(PointLights[i].Position - input.WorldPos);

        float attenuation = CalculatePointLightEvaluation(distance, PointLights[i].Radius, PointLights[i].Intensity);
        float3 lightColor = PointLights[i].Color * PI * PointLights[i].Intensity / (4 * PI); 
        float3 radiance = lightColor * attenuation;
        float3 spec = CalculateBRDF(input, View, AlbedoTex, Normal, RoughnessTex, AmbiantOclusionTex, MetalicTex, Light, F0);
        LightOut += spec * radiance;
		
	  }
    
    for (uint j = 0; j < DirectionalLightCount; j++)
    {
        float3 Light = normalize(DirectionalLights[j].Direction);   
        float3 radiance = DirectionalLights[j].Color * DirectionalLights[j].Intensity;
        float3 spec = CalculateBRDF(input, View, AlbedoTex, Normal, RoughnessTex, AmbiantOclusionTex, MetalicTex, Light, F0);

        float2 projectTexCoord = 0;
        float depthValue = 0;
        float lightDepthValue = 0;
        float bias = 0.001f;
        float shadows = 1.0f;

        projectTexCoord.x = input.LightPosition.x / input.LightPosition.w / 2.0f + 0.5f;
        projectTexCoord.y = -input.LightPosition.y / input.LightPosition.w / 2.0f + 0.5f;
        depthValue = ShadowMap.Sample(ClampSampler, projectTexCoord).r;

        // Determine if the projected coordinates are in the 0 to 1 range.  If so then this pixel is in the view of the light.
        if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
        {

            lightDepthValue = input.LightPosition.z / input.LightPosition.w;
            lightDepthValue = lightDepthValue - bias;

            if (lightDepthValue > depthValue)
            {
                shadows = 0.1f;

            }

        }
           
        LightOut += (spec * (radiance * shadows));
       // LightOut += shadows;
        
        
        //spec * radiance + (1 - Shadow);

    }
    float test =  saturate(dot(Normal, View));
    float3 _kS = fresnelSchlickRoughness(saturate(dot(Normal,-View)), F0, RoughnessTex);
    float3 _kD = 1.0 - _kS;
    _kD *= 1.0f - MetalicTex;
    
    float3 irradiance = irradianceMap.Sample(Sampler, Normal);
    float3 diffuse = AlbedoTex.rgb * irradiance;
	
	
    float3 prefilteredColor = Prefiltered.SampleLevel(Sampler, normalize(reflect(-View, Normal)), RoughnessTex * 7).rgb;
    float2 envBRDF = BRDFIntegration.Sample(Sampler, float2(saturate(dot(Normal, View)), RoughnessTex)).rg;
    float3 specular = prefilteredColor * (_kS * envBRDF.x + envBRDF.y);
	
	
    float3 ambient = (_kD * diffuse  ) * AmbiantOclusionTex;
	
    float3 color = ambient + LightOut + specular;
    
    color += EmisiveMap * EmisiveFactor * EmisiveTint ;
    
    
    //  color = color / (1 + color);

    //float3 ambient = float3(0.03f, 0.3f, 0.3f) * float3(1.0f, 1.0f, 1.0f) * 1.0f; //vec3(0.03) * albedo * ao;
    //float3 color = ambient + LightOut;
	
  //  color = color / (color + float3(1.0f, 1.0f, 1.0f));
   // color = pow(color, float3(1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f));
	
  //  color = pow(abs(color), 1 / 2.22);
    
    
    
    /*Debug cascade : get the depht = posZ/posW * 0.5 + 0.5*/
    
           
  /*Test */
    
    float2 projectTexCoord = 0;
    float depthValue = 0;
    float lightDepthValue = 0;
    float bias = 0.001f;
    float shadows = 1.0f;

    projectTexCoord.x = input.LightPosition.x / input.LightPosition.w / 2.0f + 0.5f;
    projectTexCoord.y = -input.LightPosition.y / input.LightPosition.w / 2.0f + 0.5f;
    depthValue = ShadowMap.Sample(ClampSampler, projectTexCoord).r;

    // Determine if the projected coordinates are in the 0 to 1 range.  If so then this pixel is in the view of the light.
    if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
    {

        lightDepthValue = input.LightPosition.z / input.LightPosition.w;
        lightDepthValue = lightDepthValue - bias;

        if (lightDepthValue > depthValue)
        {
            shadows = 0.1f;

        }

    }
 
   color = uncharted2_filmic(color);

    return float4(color, AlbedoTex.a);
}