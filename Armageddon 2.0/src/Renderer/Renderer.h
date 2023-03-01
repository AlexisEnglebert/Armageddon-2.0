#pragma once
#include "../Macros.h"

#if WINDOWS
	#include "DirectX11/DirectX11Renderer.h"
#endif
#include <vulkan/vulkan.h>
#include <vector>
/*
#include "../Log.h"
#include "ConstantBuffer.h"
#include "ConstantBufferTypes.h"
#include "AdaptaterData.h"
#include "OffScreenRenderTargetView.h"
#include "Mesh.h"
#include "Interface.h"
#include "imgui.h"

#include "../Material/AssetManager.h"
#include "../Material/Texture.h"

#include "RenderPass/VolumetricFog.h"
#include "RenderPass/CascadeShadow.h"
#include "RenderPass/PostProcessing/Bloom.h"
#include "RenderPass/EnvMap.h"
*/
#include "RendererAPI.h"
#include "Vulkan/VulkanRenderer.h"


namespace Armageddon
{
	class DECL Renderer
	{
	public:
		void init();
		//VulkanRenderer m_vk_renderer;
		bool InitVulkan(VkInstance& instance);
		void InitDirectX();
		/*bool Init(HWND hwnd, float width, float height);
		void ResizeBuffer(float width, float height);
		void RenderFrame();
		void SubmitMesh(Mesh& mesh);
		~Renderer();


		OffScreenRenderTarget& GetOffScreenRenderTarget() { return m_OffScreenRenderTarget; };
		inline ImGuiContext* GetImGuiContext() { return m_Context; };

		Armageddon::Camera m_camera;
		static ConstantBuffer<TransFormBuffer> g_TransformCBuffer;
		static ConstantBuffer<CameraBuffer_t> g_CameraCBuffer;
		static ConstantBuffer<LightBuffer> g_LightCBuffer;
		static ConstantBuffer<MaterialBuffer> g_PBRCBuffer;
		static ConstantBuffer<WorldBuffer> g_WorldCBuffer;
		static ConstantBuffer<VolumetricBuffer_t> g_VolumetricBuffer;
		static std::vector<PointLight> g_PointLightsVector;
		static std::vector<DirectionalLight> g_DirectLightsVector;

	

		std::vector<Mesh> m_Meshes;


		static LightBuffer g_LightBufferData;
		static WorldBuffer g_WorldBufferData;
		static CameraBuffer_t g_CameraBufferData;
		static VolumetricBuffer_t g_volumetricBufferData; //todo in scene  !

	
		RenderTextureDepht m_FrameBuffer;
		RenderTextureDepht m_Composite;

		Armageddon::PixelShaders  FinalPassPixel;
		Armageddon::VertexShaders FinalPassVertex;


		Armageddon::PixelShaders  DisplayPixel;
		Armageddon::VertexShaders DisplayVertex;

		RenderTexture m_DepthPass;

		Armageddon::VolumetricFog m_VolumetricFog;
		Armageddon::CascadeShadow m_Cascade;
		Armageddon::EnvMap m_Envmap;
		Armageddon::Bloom m_bloom;

		Mesh m_quad;


		bool displayScene = false;*/


	private:

		/*Vulkan*/
		VulkanRenderer m_vk_renderer;

		/*void LoadImGuiStyle();*/

		//AssetManager m_AssetManager;

		/*Off Screen Render Target View*/

		/*Midle*/
		/*
		float m_screenPos[2]{0.0f,0.0f};
		float m_origin[2]{0.0f,0.0f};

		ImGuiContext* m_Context = nullptr;

		*/



	};

}