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
	int PointLightCount;
	int DirectionalLightCount;
	float2 padding1;


	PointLight PointLights[50];
	DirectionalLight DirectionalLights[50];
	row_major float4x4 LightViewProjectionCascade[3]; // TODO BETTER HANDLING OF CASCADE NUM
	float FarPlane[3]; // attention à l'alignement :D 
	int cascadeIndice;

};

cbuffer VolumetricBuffer : register(b6)
{
	float Absorption;
	float Scattering;
	float Phase;
};

cbuffer WorldCBuffer : register(b4)
{
    float time;
}

cbuffer CameraBuffer : register(b2)
{
	row_major float4x4  ProjectionMat;
	row_major float4x4  ViewMat;
	row_major float4x4  MVP;
	row_major float4x4 InverseProjectionMat;
	row_major float4x4 InverseViewMat;
	row_major float4x4 InverseMVP;

	float3 CameraPos; //12 
	float nearPlane;
	float farPlane;
};

struct PSinput
{
	float4 position : SV_Position;
	float2 textCoord : TEXCOORD0;

};

#define VOLUMETRIC_SAMPLE 50 

Texture2D SceneDepth : register(t0);
SamplerState Sampler : register(s0);

static const float PI = 3.1415926535897932384626433832795f;




/*
* All math and Theory is from this book : https://research.quanfita.cn/files/Real-Time_Rendering_4th_Edition.pdf
*/

/*
*Tr(xa, xb) = e^−τ 
*τ = integral from x=xa to xb of extinction(X)
xa = pos1 & xb = pos2
*/
float3 Transmittance(float3 campos, float3 pointPosition,float3 extinction) // AKA Beer-Lambert Law
{
	float dist = distance(campos, pointPosition);
	float3 tau = extinction * dist; 	// amount of light attenuation | The optical depth TODO: Understand this i think it's not correct to calculate TAU like this
	float3 tr = exp(-tau);
	return tr;
}

/*p = Scattering / (Scattering + Absorption) = scattering / Extinction*/
float3 Albedo(float3 scattering, float3 extinction)
{
	return scattering / extinction;
}

/* 
* lci = direction of the i light
* PI * sum {i=1 -> n}( phase(v, lci) * v(x, plighti) * c * lighti(abs(x - Plighti) )  )
*/
//phase
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


//TODO améliorer ça car c'est pas top top on fait déjà le calcul dans le PBR 
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


float3 Scatter(float3 position, float3 direction)
{
	float3 scatterOutput = float3(0.0f,0.0f,0.0f);
	for(int i = 0 ; i < PointLightCount; i ++)
	{
		float3 directionVec = position - PointLights[i].Position;
		scatterOutput += phase(1,direction,directionVec) * CalculatePointLightEvaluation(length(directionVec),PointLights[i].Radius, PointLights[i].Intensity);//TODO * view() 
	}

	scatterOutput *= PI;
	return scatterOutput;
}

float3 visibility()
{

}




float4 VolumetricCompute(float3 worldPos)
{

	float3 Lo = float3(0.0f, 0.0f, 0.0f);
	float3 extinction = Scattering + Absorption;
	extinction = float3(0.5, 1, 2);
	for (uint i = 0; i < PointLightCount; i++)
	{
		Lo += Transmittance(CameraPos, PointLights[i].Position, extinction);
	}
	Lo *= Scatter(CameraPos,CameraPos-worldPos);
	Lo *= PI;
	return float4(Lo, 1.0f);
}

float3 TransformToWorldSpace(float depth, PSinput input) //replace this with GBuffer
{
	float z = depth * 2.0 - 1.0;
	float4 clipSpacePosition = float4(input.textCoord.x * 2.0 - 1.0, input.textCoord.y * 2.0 - 1.0, z, 1.0);
	float4 viewSpacePosition = mul(clipSpacePosition, InverseProjectionMat);
	viewSpacePosition /= viewSpacePosition.w;

	float4 worldSpacePosition  = mul(viewSpacePosition, InverseViewMat);
    return worldSpacePosition.xyz;

}


//SIMPLE TESTE UWU

float3 absorptionTransmittance(float dist) {
    return float3(1.0f,1.0f,1.0f)*exp(-dist*(0.030f+0.2f));
}

float random(float2 co) {
    return frac(sin(dot(co.xy, float2(12.9898, 78.233))) * 43758.5453123);
}

float3 volumetricScattering(float3 fragPosition) {
	float scatteringSamples = 50;
    float3 result = float3(0.0, 0.0, 0.0);
    float3 camToFrag = fragPosition - CameraPos;
    if(length(camToFrag) > 1000.0f) {
        camToFrag = normalize(camToFrag) * 1000.0f;
    }
    float3 deltaStep = camToFrag / (scatteringSamples+1);
    float3 fragToCamNorm = normalize(CameraPos - fragPosition);
    float3 x = CameraPos;
    
    // Randomize the sampling points along the ray to trade flickering
    // for plain noise
    float rand = random(fragPosition.xy+fragPosition.z);
    x += (deltaStep*rand);
    
    for(int i = 0; i < scatteringSamples; ++i) {
        float visibility = 1.0; //- shadowFactor(PointLights[i], x)
        float3 lightToX = x - PointLights[i].Position;
        float lightDist = length(lightToX);
        float omega = 4 * PI * lightDist * lightDist;
        float3 Lin = absorptionTransmittance(lightDist) * visibility * PointLights[i].Color * PointLights[i].Intensity / omega;
        float3 Li = Lin * 1 * float3(1.0f,1.0f,1.0f) * phase(1,normalize(lightToX), fragToCamNorm);
        result += Li * absorptionTransmittance(distance(x, CameraPos)) * length(deltaStep);
        x += deltaStep;
    }
    
    return result;
}


float4 main(PSinput input) : SV_TARGET
{
	float3 depth = SceneDepth.Sample(Sampler,input.textCoord);
	float3 vertPos = TransformToWorldSpace(depth.r,input);
	float3 outcolor = volumetricScattering(vertPos);
	return float4(outcolor,1.0f);
}