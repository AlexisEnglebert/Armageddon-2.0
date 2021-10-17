#pragma once
#include <dxgi.h>
#include <d3d11.h>
#include <vector>
#include <wrl\client.h>
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

#pragma comment(lib, "dxgi.lib")

namespace Armageddon
{
	class DECL Renderer
	{
	public:
		bool Init(HWND hwnd, int height, int width);
		void ResizeBuffer(float width, float height);
		void RenderFrame();
		void SubmitMesh(Mesh& mesh);
		~Renderer();
		//Renderer* GetRenderer() { return this;};
		OffScreenRenderTarget& GetOffScreenRenderTarget() { return m_OffScreenRenderTarget; };
		inline ImGuiContext* GetImGuiContext() { return m_Context; };
		Armageddon::Camera m_camera;
		static ConstantBuffer<TransFormBuffer> g_TransformCBuffer;
		static ConstantBuffer<LightBuffer> g_LightCBuffer;
		static ConstantBuffer<RoughnessBuffer> g_RoughnessCBuffer;
		static ConstantBuffer<MaterialBuffer> g_PBRCBuffer;
		static std::vector<PointLight> g_PointLightsVector;
		static std::vector<DirectionalLight> g_DirectLightsVector;

		static Microsoft::WRL::ComPtr < ID3D11BlendState>		AlphaBlendState;
		static Microsoft::WRL::ComPtr < ID3D11BlendState>		DefaultBlendState;

		std::vector<Mesh> m_Meshes;
		static LightBuffer g_LightBufferData;

		static D3D11_VIEWPORT ViewPort;
		OffScreenRenderTarget m_FrameBuffer;

	private:
		bool InitSwapChain(HWND& hwnd);
	
		bool CreateDephtStencilBuffer(int width, int height);
		void ResetDephtStencileBuffer();

		void CreateRenderTargetView(float width, float height);
		void CleanRenderTargetView();

		void CreateViewPort(float width, float height);

		void CreateRasterizer(D3D11_RASTERIZER_DESC rDesc);
		void ChangeRasterizer(D3D11_RASTERIZER_DESC rDesc);

		void CreateAlphaBlendState();
		void CreateDefaultBlendState();

		AssetManager m_AssetManager;

		/*DirectX11 Base*/


		/*DephtStencil*/
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DephtStencilView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DephtStencilState;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> DephtStencilBuffer;

		/*RasterRizer*/
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> RasterizerState;

		/*Off Screen Render Target View*/
		OffScreenRenderTarget m_OffScreenRenderTarget;

		/*Midle*/
		float m_screenPos[2]{0.0f,0.0f};
		float m_origin[2]{0.0f,0.0f};

		ImGuiContext* m_Context = nullptr;

		



	};

}