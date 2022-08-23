	#include "VolumetricFog.h"

void Armageddon::VolumetricFog::Init(float width,float height)
{
	m_VolumetricPixelShader.Init(Armageddon::Interface::GetDevice(),"Assets/Shaders/VolumetricFog.cso");

	m_VolumetricTexture.Init(width, height, DXGI_FORMAT_R16G16B16A16_FLOAT);
	m_VolumetricIntegration.Init(160,90,128,1,DXGI_FORMAT_R16G16B16A16_FLOAT); //We define here voxel size
	m_VolumetricScatteringAccum.Init(160, 90, 128, 1, DXGI_FORMAT_R16G16B16A16_FLOAT); //Must be the same as VolumetricIntegration ! 
	if (!m_VolumetricInjectShader.Init(Armageddon::Interface::GetDevice(), "Assets/Shaders/VolumetricDensityAndLight.cso"))
	{
		Armageddon::Log::GetLogger()->error("ERROR TO CREATE THE COMPUTE SHADER");
	}
	if (!m_VolumetricScatteringShader.Init(Armageddon::Interface::GetDevice(), "Assets/Shaders/VolumetricRaymarching.cso"))
	{
		Armageddon::Log::GetLogger()->error("ERROR TO CREATE THE COMPUTE SHADER");
	}
}
