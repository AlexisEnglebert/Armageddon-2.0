#pragma kernel CSMain

Texture3D m_VolumetricIntegration : register(t0); //This could may be half4 because of the 3D texture which is RGBA16F
RWTexture3D<float4> m_VolumeScatter : register(u0); //This could may be half4 because of the 3D texture which is RGBA16F

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
/*
* All math and Theory is from this book : https://research.quanfita.cn/files/Real-Time_Rendering_4th_Edition.pdf
*/

float Transmittance(float Zslice,float density) // AKA Beer-Lambert Law
{
	float tau = Zslice * density; 	// amount of light attenuation | The optical depth is the Zlsice 
	float tr = exp(-tau);
	return tr;
}
// (160X90X128)



float4 ScatterStep(float3 accumulatedLight, float accumulatedTransmittance, float3 sliceLight, float sliceDensity)
{
	sliceDensity = max(sliceDensity, 0.000001);
	float  sliceTransmittance = exp(-sliceDensity / 128.0f);

	float3 sliceLightIntegral = sliceLight * (1.0 - sliceTransmittance) / sliceDensity;
	accumulatedLight += sliceLightIntegral * accumulatedTransmittance;
	accumulatedTransmittance *= sliceTransmittance;

	return float4(accumulatedLight, accumulatedTransmittance);
}


[numthreads(16, 1, 1)]
void main(uint3 id : SV_DispatchThreadID)
{
	float4 accum= float4(0.0f, 0.0f, 0.0f, 1.0f);
	uint Zslice = 128; //Texture definition
	float3 pos = id;
	for (uint i = 0; i < Zslice; i++)
	{
		pos.z = i;
		float4 slice = m_VolumetricIntegration.Load(uint4(pos.xyz,0.0f)); //Fog A is density  -

		accum  = ScatterStep(accum.rgb, accum.a, slice.rgb, slice.a);
		m_VolumeScatter[pos] = accum;

	}

} 