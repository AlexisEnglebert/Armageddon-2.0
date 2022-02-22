#pragma once
#if __linux__
// pass pour l'instant
#else
#include <dxgi.h>
#include <d3d11.h>
#include <wrl\client.h>
#endif
#include <vector>
#include "../Macros.h"

namespace Armageddon
{
	class DECL Interface
	{
	public:
		static Microsoft::WRL::ComPtr <ID3D11Device>& GetDevice();
		static Microsoft::WRL::ComPtr <ID3D11DeviceContext>& GetDeviceContext();
		static Microsoft::WRL::ComPtr <IDXGISwapChain>& GetSwapChain();
		static Microsoft::WRL::ComPtr<ID3D11SamplerState>& GetSamplerState();
		static Microsoft::WRL::ComPtr<ID3D11SamplerState>& GetTrilinearSampler();
		static Microsoft::WRL::ComPtr<ID3D11SamplerState>& GetClampSampler();
		static Microsoft::WRL::ComPtr <ID3D11RenderTargetView>& GetRenderTargetView();
		static Microsoft::WRL::ComPtr <ID3D11BlendState>& GetColorBlendState();
		static Microsoft::WRL::ComPtr <ID3D11RasterizerState>& GetDefaultRasterizerState();
		
		
	
	private:
		static Microsoft::WRL::ComPtr <ID3D11Device> Device;
		static Microsoft::WRL::ComPtr <ID3D11DeviceContext> DeviceContext;
		static Microsoft::WRL::ComPtr <IDXGISwapChain> Swapchain;
		static Microsoft::WRL::ComPtr<ID3D11SamplerState> SamplerState;
		static Microsoft::WRL::ComPtr<ID3D11SamplerState> TrilinearSampler;
		static Microsoft::WRL::ComPtr<ID3D11SamplerState> ClampSamplerState;
		static Microsoft::WRL::ComPtr <ID3D11RenderTargetView> RenderTargetView;
		static Microsoft::WRL::ComPtr < ID3D11BlendState>		ColorBlendState;

		/*RasterRizer*/
		static Microsoft::WRL::ComPtr<ID3D11RasterizerState> RasterizerState;

		
	};
}