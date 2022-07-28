#pragma kernel CSMain


RWTexture3D<float4> m_VolumetricIntegration; //This could may be half4 because of the 3D texture which is RGBA16F
[numthreads(8, 8, 1)] //TO SEE with the 3D texture

void main( uint3 id : SV_DispatchThreadID )
{
	m_VolumetricIntegration[id] = float4((float)id.x/160, (float)id.y/90, (float)id.z, 1.0f); //Simple test to see how it works
}