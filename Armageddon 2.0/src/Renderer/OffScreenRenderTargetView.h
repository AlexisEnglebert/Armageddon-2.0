#pragma once
#if __linux__
//pass pour l'instant 
#else
#include <d3d11.h>
#include <wrl/client.h>
#endif
#include "../Macros.h"
#include "../Log.h"
#include "Interface.h"

	class DECL OffScreenRenderTarget
	{
	public:
		bool Init(ID3D11Device* device, IDXGISwapChain* swapchain, float width, float height);
		void ResizeRenderTargetView(float width, float height, ID3D11Debug* debug);
		void CleanRenderTargetView();
		void CreateRenderTargetView(ID3D11Device* device, IDXGISwapChain* swapchain);
		void CreateScreenTexture(ID3D11Device* device, float width, float height);
		void createDephTexture(ID3D11Device* device, float width, float height);
		void CreateShaderResourceView(ID3D11Device* device);
		void CreateDephtStencil(int width, int height);
		void CleanDephtStencil();
		void Bind(ID3D11DeviceContext* device_context);
		void Bind(ID3D11DeviceContext* device_context, ID3D11DepthStencilView* dephStencil);
		void Clear(ID3D11DeviceContext* device_context);
		void BindBackBuffer(ID3D11DeviceContext* device_context, ID3D11RenderTargetView* render);
		void BindBackBuffer(ID3D11DeviceContext* device_context, ID3D11RenderTargetView** render, ID3D11DepthStencilView* dephStencil);
		void UnBind();
		ID3D11ShaderResourceView* GetShaderRessource() { return ShaderRessourceView.Get(); };
		ID3D11ShaderResourceView** GetAddressOfShaderRessource() { return ShaderRessourceView.GetAddressOf(); };
		ID3D11Texture2D* GetScreenTexture() { return ScreenTex.Get(); };
		Microsoft::WRL::ComPtr <ID3D11RenderTargetView> pTargetView;
		float GetImageX() { return ImageX; };
		float GetImageY() { return ImageY; };
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> DephtResourceView;

	private:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ShaderRessourceView;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> ScreenTex;

		/*DephtStencil*/
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DephtStencilView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DephtStencilState;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> DephtStencilBuffer;
		float ImageX = 0, ImageY = 0;
	};