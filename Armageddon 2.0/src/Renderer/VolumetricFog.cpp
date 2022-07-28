	#include "VolumetricFog.h"

void Armageddon::VolumetricFog::Init(float width,float height)
{
	m_VolumetricPixelShader.Init(Armageddon::Interface::GetDevice(),"..\\bin\\Release-x64\\Armageddon 2.0\\VolumetricFog.cso");
	m_VolumetricTexture.Init(width, height, DXGI_FORMAT_R16G16B16A16_FLOAT);
	m_VolumetricIntegration.Init(160,90,128,1,DXGI_FORMAT_R16G16B16A16_FLOAT); //We define here voxel size
	if (!m_VolumetricInjectShader.Init(Armageddon::Interface::GetDevice(), "..\\bin\\Release-x64\\Armageddon 2.0\\VolumetricDensityAndLight.cso"))
	{
		Armageddon::Log::GetLogger()->error("ERROR TO CREATE THE COMPUTE SHADER");
	}
}
