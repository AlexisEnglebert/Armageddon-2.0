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
    float Roughness;
    float Metalic;
    bool UseMetalMap;
    float _Padding0;
    bool UseEmisive;
    float3 AlbedoTint; 
    float _Padding1;
};


struct PSinput
{
    float4 position : SV_Position;
    float2 textCoord : TEXCOORD0;
    float3 normal    : NORMAL;
    float3 Tangent   : TANGENT;
    float3 Binormal  : BINORMAL;
	float3 WorldPos : POSITION;
    float4 LightPosition : LIGHTPOS;

};

Texture2D AlbedoMap : register(t0);
Texture2D NormalMap : register(t1);
Texture2D SpecularMap : register(t2);
Texture2D MetalicMap : register(t3);
Texture2D AmbiantOMap : register(t4);
Texture2D EmissiveMap : register(t5);
Texture2D ShadowMap   : register(t9);
Texture2D bloom   : register(t10);

TextureCube irradianceMap : register(t6);
TextureCube Prefiltered : register(t7);
Texture2D   BRDFIntegration : register(t8);
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
    Denominateur = (Denominateur * Denominateur); //ajouter PI ? 

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
	float NdotV = max(dot(Normal, View), 0.0);
	float NdotL = max(dot(Normal, Light), 0.0);
	float ggx1 = GeometrySchlickGGX(NdotV, Roughness);
	float ggx2 = GeometrySchlickGGX(NdotL, Roughness);
	
	return ggx1 * ggx2;
}

float3 fresnelSchlick(float cosTheta, float3 F0)
{
	return F0 + (1.0f - F0) * pow(1.0 - cosTheta, 5.0f);
}

float3 fresnelSchlickRoughness(float cosTheta, float3 F0, float roughness)
{
    return F0 + (max(float3(1.0 - roughness, 1.0 - roughness, 1.0 - roughness), F0) - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}

float CalculatePointLightAttenuation(float distance,float radius)
{
    float denom = (distance / radius + 1);
    float demnom2 = denom * denom;
    float attenuation = 1.0f / demnom2;
    
    return attenuation;

}



float3 uncharted2_tonemap_partial(float3 x)
{
    float A = 0.15f;
    float B = 0.50f;
    float C = 0.10f;
    float D = 0.20f;
    float E = 0.02f;
    float F = 0.30f;
    return ((x * (A * x + C * B) + D * E) / (x * (A * x + B) + D * F)) - E / F;
}

float3 uncharted2_filmic(float3 v)
{
    float exposure_bias = 2.0f;
    float3 curr = uncharted2_tonemap_partial(v * exposure_bias);

    float3 W = float3(11.2f, 11.2f, 11.2f);
    float3 white_scale = float3(1.0f, 1.0f, 1.0f) / uncharted2_tonemap_partial(W);
    return curr * white_scale;
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
    float Denominateur = 4.0f * max(dot(Normal, View), 0.0f) * max(dot(Normal, Light), 0.0f) + 0.001f;
    float3 specular = Numerateur / Denominateur;
        
    float NdotL = max(dot(Normal, Light), 0.0f);
        
    return (kD * AlbedoTex.rgb / PI + specular)  * NdotL;
}
float4 main(PSinput input) : SV_TARGET
{
      
    float4 AlbedoTex            = AlbedoMap.Sample(Sampler, input.textCoord);
    float3 NormalTex            = NormalMap.Sample(Sampler, input.textCoord);
    float RoughnessTex          = SpecularMap.Sample(Sampler, input.textCoord).r;
    float AmbiantOclusionTex    = AmbiantOMap.Sample(Sampler, input.textCoord);
    float4 bloomtex = bloom.Sample(Sampler, input.textCoord);

    AmbiantOclusionTex = AmbiantOclusionTex != 0 ? AmbiantOclusionTex : 1.0f;
    
    float MetalicTex = UseMetalMap ? MetalicMap.Sample(Sampler, input.textCoord).r : float3(0.04f, 0.04f, 0.04f);
    float3 EmisiveMap = UseEmisive ? EmissiveMap.Sample(Sampler, input.textCoord) : float3(0.0f, 0.0f, 0.0f);
    //RoughnessTex = RoughnessTex != 0 ? RoughnessTex : 1.0f;
    //NormalTex = NormalTex != 0 ? NormalTex : normalize(input.normal);
    //AlbedoTex = AlbedoTex != 0 ? AlbedoTex : float3(1.0f, 1.0f, 1.0f);
    AlbedoTex *= float4(AlbedoTint, AlbedoTex.a);
    
    
    //MetalicTex = Metalic;
    RoughnessTex = Roughness;
    
    NormalTex = (NormalTex * 2.0f) - 1.0f;
    float3 BumpNormal = (NormalTex.x * input.Tangent) + (NormalTex.y * input.Binormal) + (NormalTex.z * input.normal);
    BumpNormal = normalize(BumpNormal);
    
      float3 LightOut = float3(0.0f,0.0f,0.0f);
	  float3 F0 = float3(0.04f, 0.04f, 0.04f);
      float3 Normal = BumpNormal;
	  float3 View = normalize(CameraPos - input.WorldPos);

      F0 = lerp(F0, AlbedoTex.rgb, MetalicTex);
      for (uint i = 0; i < PointLightCount; i++)
      {
        
        float3 Light = normalize(PointLights[i].Position - input.WorldPos);
        float distance = length(PointLights[i].Position - input.WorldPos);
        float attenuation = CalculatePointLightAttenuation(distance, PointLights[i].Radius);
        float3 radiance = PointLights[i].Color * PointLights[i].Intensity * attenuation;
        float3 spec = CalculateBRDF(input, View, AlbedoTex, Normal, RoughnessTex, AmbiantOclusionTex, MetalicTex, Light, F0);
        LightOut += spec * radiance;
		
	  }
    
    for (uint j = 0; j < DirectionalLightCount; j++)
    {
        float3 Light = normalize(DirectionalLights[j].Direction);   
        float3 radiance = DirectionalLights[j].Color * DirectionalLights[j].Intensity;
        float3 spec = CalculateBRDF(input, View, AlbedoTex, Normal, RoughnessTex, AmbiantOclusionTex, MetalicTex, Light, F0);
        //float Shadow = CalculateShadow(input.LightPosition.xyzw);
         
      	// Calculate the projected texture coordinates.
        float2 projectTexCoord = 0;
        float depthValue = 0;
        float lightDepthValue = 0;
        float bias = 0.001f;
        float shadows = 1.0f;

        projectTexCoord.x = input.LightPosition.x / input.LightPosition.w / 2.0f + 0.5f;
        projectTexCoord.y = -input.LightPosition.y / input.LightPosition.w / 2.0f + 0.5f;

	    // Determine if the projected coordinates are in the 0 to 1 range.  If so then this pixel is in the view of the light.
        if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
        {
            lightDepthValue = input.LightPosition.z / input.LightPosition.w;
            depthValue = ShadowMap.Sample(ClampSampler, projectTexCoord).r;

		// Sample the shadow map depth value from the depth texture using the sampler at the projected texture coordinate location.

		// Calculate the depth of the light.

		// Subtract the bias from the lightDepthValue.
            lightDepthValue = lightDepthValue - bias;

		// Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
		// If the light is in front of the object then light the pixel, if not then shadow this pixel since an object (occluder) is casting a shadow on it.
            if (lightDepthValue > depthValue && depthValue != 1 )
            {
                shadows = 0.0f;

            }
        }
        
           
        LightOut += (spec * radiance) ;
        
        
        //spec * radiance + (1 - Shadow);

    }
	
    float3 _kS = fresnelSchlickRoughness(max(dot(Normal, View), 0.0), F0, RoughnessTex);
    float3 _kD = 1.0 - _kS;
    _kD *= 1.0f - MetalicTex;
    
    float3 irradiance = irradianceMap.Sample(Sampler, Normal);
    float3 diffuse = AlbedoTex.rgb * irradiance;
	
	
    float3 prefilteredColor = Prefiltered.SampleLevel(Sampler, normalize(reflect(-View, Normal)), RoughnessTex * 7).rgb;
    float2 envBRDF = BRDFIntegration.Sample(Sampler, float2(saturate(dot(Normal, View)), RoughnessTex)).rg;
    float3 specular = prefilteredColor * (_kS * envBRDF.x + envBRDF.y);
	
	
    float3 ambient = (_kD * diffuse  + specular) * AmbiantOclusionTex;
	
    float3 color = ambient + LightOut;  
    
    color += EmisiveMap + bloomtex;
    
    
   // color = ReinhardToneMap(color, 4.0f);
    //  color = color / (1 + color);

    //float3 ambient = float3(0.03f, 0.3f, 0.3f) * float3(1.0f, 1.0f, 1.0f) * 1.0f; //vec3(0.03) * albedo * ao;
    //float3 color = ambient + LightOut;
	
  //  color = color / (color + float3(1.0f, 1.0f, 1.0f));
   // color = pow(color, float3(1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f));
	
    color = uncharted2_filmic(color);
    color = pow(abs(color), 1 / 2.22);
    
    
    
    /*Debug cascade : get the depht = posZ/posW * 0.5 + 0.5*/
    
           
  

   /* float2 projectTexCoord = 0;
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
            shadows = 0.0f;

        }
    
    }*/
 
    
    return float4(color, AlbedoTex.a);
}