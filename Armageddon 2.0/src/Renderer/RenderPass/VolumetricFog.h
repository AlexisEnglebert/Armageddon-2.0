#pragma once
#include "../../Material/Texture.h"
#include "../../Renderer/Shaders.h"
#include "../../Renderer/Interface.h"
namespace Armageddon
{
	struct VolumetricBuffer_t  //Alligned with 16 bytes
	{
		DirectX::XMFLOAT3 Scattering = { 0.0f, 0.0f, 0.0f };	//12
		float Extinction = 0.0f;				//16
		DirectX::XMFLOAT3 Emissive = { 0.0f, 0.0f, 0.0f };		// 28
		float Phase = 0.0f;					//32
		float LerpFactor = 1.0f;		//1
		float density = 1.0f;
		float ambientFog = 0.5f;
		bool EnableVolumetricFog = false;
		float FogIntensity = 1.0f;
		float anisotropy;

	}; //this is the "V-buffer" from the hillaire15 presentation

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
		* Then we will compute the density estimation of the participating media  & the lighting calculation
		* 
		* After that we will enter in the volumetricLight Raymarching
		*
		* 
		* 

		* Good exemple (Hillaire Volumetric) : https://www.shadertoy.com/view/XlBSRz
		* https://bartwronski.files.wordpress.com/2014/08/bwronski_volumetric_fog_siggraph2014.pdf
		* https://developer.nvidia.com/sites/default/files/akamai/gameworks/downloads/papers/NVVL/Fast_Flexible_Physically-Based_Volumetric_Light_Scattering.pdf?t=eyJscyI6ImdzZW8iLCJsc2QiOiJodHRwczpcL1wvd3d3Lmdvb2dsZS5jb21cLyJ9
		*/
		RenderTexture m_VolumetricTexture;
		Texture3D m_VolumetricIntegration; //its simply a cube of 160X90X128 so 720p surface
		Texture3D m_VolumetricScatteringAccum; //its simply a cube of 160X90X128 so 720p surface
		PixelShaders m_VolumetricPixelShader;
		ComputeShader m_VolumetricInjectShader;
		ComputeShader m_VolumetricScatteringShader;


	};
}