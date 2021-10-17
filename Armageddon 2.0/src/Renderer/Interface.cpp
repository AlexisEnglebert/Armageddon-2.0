#include "Interface.h"

Microsoft::WRL::ComPtr <ID3D11Device> Armageddon::Interface::Device = nullptr;
Microsoft::WRL::ComPtr <ID3D11DeviceContext>  Armageddon::Interface::DeviceContext = nullptr;
Microsoft::WRL::ComPtr <IDXGISwapChain>  Armageddon::Interface::Swapchain = nullptr;
Microsoft::WRL::ComPtr<ID3D11SamplerState>  Armageddon::Interface::SamplerState = nullptr;
Microsoft::WRL::ComPtr<ID3D11SamplerState>  Armageddon::Interface::TrilinearSampler = nullptr;
Microsoft::WRL::ComPtr<ID3D11SamplerState>  Armageddon::Interface::ClampSamplerState = nullptr;
Microsoft::WRL::ComPtr < ID3D11BlendState>	Armageddon::Interface::ColorBlendState = nullptr;

 Microsoft::WRL::ComPtr <ID3D11RenderTargetView> Armageddon::Interface::RenderTargetView = nullptr;

Microsoft::WRL::ComPtr<ID3D11Device>& Armageddon::Interface::GetDevice()
{
    return Armageddon::Interface::Device;
}

Microsoft::WRL::ComPtr<ID3D11DeviceContext>& Armageddon::Interface::GetDeviceContext()
{
    return Armageddon::Interface::DeviceContext;
}

Microsoft::WRL::ComPtr<IDXGISwapChain>& Armageddon::Interface::GetSwapChain()
{
    return Armageddon::Interface::Swapchain;
}

Microsoft::WRL::ComPtr<ID3D11SamplerState>& Armageddon::Interface::GetSamplerState()
{
    return Armageddon::Interface::SamplerState;
}

Microsoft::WRL::ComPtr<ID3D11SamplerState>& Armageddon::Interface::GetTrilinearSampler()
{
	return Armageddon::Interface::TrilinearSampler;
}

Microsoft::WRL::ComPtr<ID3D11SamplerState>& Armageddon::Interface::GetClampSampler()
{
    return Armageddon::Interface::ClampSamplerState;
}

Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& Armageddon::Interface::GetRenderTargetView()
{
    return  Armageddon::Interface::RenderTargetView;
}

Microsoft::WRL::ComPtr<ID3D11BlendState>& Armageddon::Interface::GetColorBlendState()
{
    return Armageddon::Interface::ColorBlendState;
}

