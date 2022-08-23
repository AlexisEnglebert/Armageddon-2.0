#pragma once
#if __linux__
//rien pour l'instant
#else
#include <dxgi.h>
#include <d3d11.h>
#include <wrl\client.h>

#endif

#include <vector>
#include "../Macros.h"
#include "../Log.h"
#include "ConstantBuffer.h"
#include "ConstantBufferTypes.h"
#include "AdaptaterData.h"
#include "OffScreenRenderTargetView.h"
#include "Mesh.h"
#include "Interface.h"
#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"
#include "../Material/AssetManager.h"
#include "../Material/Texture.h"
#include "RendererAPI.h"

#include "RenderPass/VolumetricFog.h"
#include "RenderPass/CascadeShadow.h"
#include "RenderPass/PostProcessing/Bloom.h"
#include "RenderPass/EnvMap.h"

#pragma comment(lib, "dxgi.lib")

namespace Armageddon
{
	class DECL Renderer
	{
	public:
		bool Init(HWND hwnd, float width, float height);
		void ResizeBuffer(float width, float height);
		void RenderFrame();
		void SubmitMesh(Mesh& mesh);
		~Renderer();
		//Renderer* GetRenderer() { return this;};
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

		static Microsoft::WRL::ComPtr < ID3D11BlendState>		AlphaBlendState;
		static Microsoft::WRL::ComPtr < ID3D11BlendState>		DefaultBlendState;

		std::vector<Mesh> m_Meshes;


		static LightBuffer g_LightBufferData;
		static WorldBuffer g_WorldBufferData;
		static CameraBuffer_t g_CameraBufferData;
		static VolumetricBuffer_t g_volumetricBufferData; //todo in scene  !

		static D3D11_VIEWPORT ViewPort;
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

		/*DephtStencil*/
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DephtStencilView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DephtStencilState;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> DephtStencilBuffer;

		bool displayScene = false;

		void CreateViewPort(float width, float height);


	private:

		/*Vulkan*/


		/*DirectX11*/
		bool InitSwapChain(HWND& hwnd);
		bool CreateDephtStencilBuffer(int width, int height);
		void ResetDephtStencileBuffer();
		void CreateRenderTargetView(float width, float height);
		void CleanRenderTargetView();

		void CreateRasterizer(D3D11_RASTERIZER_DESC rDesc);
		void ChangeRasterizer(D3D11_RASTERIZER_DESC rDesc);
		void CreateAlphaBlendState();
		void CreateDefaultBlendState();

		void LoadImGuiStyle();

		AssetManager m_AssetManager;

		


		


		/*Off Screen Render Target View*/
		OffScreenRenderTarget m_OffScreenRenderTarget;

		/*Midle*/
		float m_screenPos[2]{0.0f,0.0f};
		float m_origin[2]{0.0f,0.0f};

		ImGuiContext* m_Context = nullptr;

		



	};

}