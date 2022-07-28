#pragma once
#include "../Material/Texture.h"
#include "../Renderer/Shaders.h"
#include "../Renderer/Interface.h"
namespace Armageddon
{
	typedef struct //Alligned with 16 bytes
	{
		DirectX::XMFLOAT3 Scattering;	//12
		float Extinction;				//16
		DirectX::XMFLOAT3 Emissive;		// 28
		float Phase;					//32

	} VolumetricBuffer_t; //this is the "V-buffer" from the hillaire15 presentation

	class VolumetricFog
	{
	public:
		VolumetricFog() = default;
		void Init(float width, float height);
		/*
		* homogeneous participating media with ray marching
		* for more information: https://research.quanfita.cn/files/Real-Time_Rendering_4th_Edition.pdf
		* 
		* Maybe will switch to frustum allignes 3D textures participating media with voxelization 
		* Voxelization of the scene to a 3D texture is the first step to learn this shit
		* but for the moment I understand nothing about that shit
		* for what I understand there is 2 stages, the VolumetricLightInjection & The volumetricLight raymarching 
		* 
		* In the VolumetricLightInjection: 
		* There is a temporal volumetric integration voxel grid (see slides 23 of Frostbite presentation)
		* 
		*So first create a 3D texture composed of "Voxel"  as BWRONSKI14 suggested (see slide 35 of his ppt)
		* 
		* 
		* A 3D texture is like an 
		*/
		RenderTexture m_VolumetricTexture;
		Texture3D m_VolumetricIntegration;
		PixelShaders m_VolumetricPixelShader;

	private:

	};
}