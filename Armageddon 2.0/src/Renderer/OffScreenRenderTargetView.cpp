#include "OffScreenRenderTargetView.h"
bool OffScreenRenderTarget::Init(ID3D11Device* device, IDXGISwapChain* swapchain, float width, float height)
{

    CreateScreenTexture(device, width, height);
    CreateShaderResourceView(device);
    CreateRenderTargetView(device, swapchain);
	CreateDephtStencil(width, height);

    //output 



    return true;
}
void OffScreenRenderTarget::CreateShaderResourceView(ID3D11Device* device)
{
    //Input Texture -> ressource view on the Texture
    D3D11_SHADER_RESOURCE_VIEW_DESC sRenderTargetDesc;
    sRenderTargetDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    sRenderTargetDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    sRenderTargetDesc.Texture2D.MipLevels = 1;
    sRenderTargetDesc.Texture2D.MostDetailedMip = 0;
    HRESULT hr = device->CreateShaderResourceView(ScreenTex.Get(), &sRenderTargetDesc, ShaderRessourceView.GetAddressOf());
    if (FAILED(hr))
    {
        Armageddon::Log::GetLogger()->error("ERROR WHEN CREATING SHADER RESSOURCE VIEW  {0}", hr);
    }
}

void OffScreenRenderTarget::CreateDephtStencil(int width, int height)
{
	D3D11_TEXTURE2D_DESC DephtStencilDesc;
	DephtStencilDesc.Width = width;
	DephtStencilDesc.Height = height;
	DephtStencilDesc.MipLevels = 1;
	DephtStencilDesc.ArraySize = 1;
	DephtStencilDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	DephtStencilDesc.SampleDesc.Count = 1;
	DephtStencilDesc.SampleDesc.Quality = 0;
	DephtStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	DephtStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	DephtStencilDesc.CPUAccessFlags = 0;    
	DephtStencilDesc.MiscFlags = 0;

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Flags = 0;
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Texture2D.MipSlice = 0;

	Armageddon::Interface::GetDevice()->CreateTexture2D(&DephtStencilDesc, NULL, this->DephtStencilBuffer.GetAddressOf());

	HRESULT result = Armageddon::Interface::GetDevice()->CreateDepthStencilView(this->DephtStencilBuffer.Get(), &dsvDesc, this->DephtStencilView.GetAddressOf());
	if (FAILED(result))
	{
		Armageddon::Log::GetLogger()->error("Erreur lors de la crétion du DephtStencilDesc {0}", result);
	}

	/*/D3D11_DEPTH_STENCIL_DESC Ddesc;
	ZeroMemory(&Ddesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	Ddesc.DepthEnable = true;
	Ddesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	Ddesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;

	result = Armageddon::Interface::GetDevice()->CreateDepthStencilState(&Ddesc, this->DephtStencilState.GetAddressOf());*/
	if (FAILED(result))
	{
		Armageddon::Log::GetLogger()->error("Erreur lors de la crétion du DephtStencilState {0}", result);

	}
	D3D11_SHADER_RESOURCE_VIEW_DESC sRenderTargetDesc;
	sRenderTargetDesc.Format = DXGI_FORMAT_R32_FLOAT;
	sRenderTargetDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	sRenderTargetDesc.Texture2D.MipLevels = 1;
	sRenderTargetDesc.Texture2D.MostDetailedMip = 0;

	HRESULT hr = Armageddon::Interface::GetDevice()->CreateShaderResourceView(DephtStencilBuffer.Get(), &sRenderTargetDesc, DephtResourceView.GetAddressOf());
	if (FAILED(hr))
	{
		Armageddon::Log::GetLogger()->error("ERROR WHEN CREATING SHADER RESSOURCE VIEW FOR DEPTH  {0}", hr);
	}
}

void OffScreenRenderTarget::CleanDephtStencil()
{   
	if (this->DephtStencilBuffer)
	{
		this->DephtStencilBuffer.Reset();
		this->DephtStencilBuffer = (std::nullptr_t)NULL;
	}
	if (this->DephtStencilView)
	{
		this->DephtStencilView.Reset();
		this->DephtStencilView = (std::nullptr_t)NULL;
	}
	if (this->DephtStencilState)
	{
		this->DephtStencilState.Reset();
		this->DephtStencilState = (std::nullptr_t)NULL;
	}
}

void OffScreenRenderTarget::ResizeRenderTargetView(float width, float height, ID3D11Debug* debug)
{
    if (width > 1 && height > 1 && Armageddon::Interface::GetDevice().Get()!= NULL)
    {

        CleanRenderTargetView();
        CleanDephtStencil();
        if (debug != nullptr)
        {
            debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
        }
        Armageddon::Interface::GetSwapChain()->ResizeTarget(0);
        CreateScreenTexture(Armageddon::Interface::GetDevice().Get(), width, height);
        CreateShaderResourceView(Armageddon::Interface::GetDevice().Get());
        CreateRenderTargetView(Armageddon::Interface::GetDevice().Get(), Armageddon::Interface::GetSwapChain().Get());
        CreateDephtStencil(width, height);
    }
}



void OffScreenRenderTarget::CleanRenderTargetView()
{
    if (pTargetView)
    {
        pTargetView.Reset();
        pTargetView = (std::nullptr_t) NULL;
    }
    if (ScreenTex)
    {
        ScreenTex.Reset();
        ScreenTex = (std::nullptr_t) NULL;
    }
    if (ShaderRessourceView)
    {
        ShaderRessourceView.Reset();
        ShaderRessourceView = (std::nullptr_t) NULL;
    }

}

void OffScreenRenderTarget::CreateRenderTargetView(ID3D11Device* device, IDXGISwapChain* swapchain)
{
    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
    renderTargetViewDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    renderTargetViewDesc.Texture2D.MipSlice = 0;


    HRESULT hr = device->CreateRenderTargetView(ScreenTex.Get(), &renderTargetViewDesc, pTargetView.GetAddressOf());
    if (FAILED(hr))
    {
        Armageddon::Log::GetLogger()->error("ERROR WHEN CREATING RENDER TARGETVIEW FOR SCENE TEX {0}", hr);
    }
}

void OffScreenRenderTarget::CreateScreenTexture(ID3D11Device* device, float width, float height)
{
    //crée la texture
    D3D11_TEXTURE2D_DESC SceneDesc;
    SceneDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    SceneDesc.Width = width;
    SceneDesc.Height = height;
    SceneDesc.ArraySize = 1;
    SceneDesc.MipLevels = 1;
    SceneDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    SceneDesc.Usage = D3D11_USAGE_DEFAULT;
    SceneDesc.CPUAccessFlags = 0;
    SceneDesc.MiscFlags = 0;
    SceneDesc.SampleDesc.Count = 1;
    SceneDesc.SampleDesc.Quality = 0;

    HRESULT hr = device->CreateTexture2D(&SceneDesc, NULL, ScreenTex.GetAddressOf());
    if (FAILED(hr))
    {
        Armageddon::Log::GetLogger()->error("ERROR WHEN CREATING TEXTURE2D FOR SCENE TEX {0}", hr);
    }
    this->ImageX = width;
    this->ImageY = height;
}

void OffScreenRenderTarget::createDephTexture(ID3D11Device* device, float width, float height)
{
}



void OffScreenRenderTarget::Bind(ID3D11DeviceContext* DeviceContext)
{
	ID3D11ShaderResourceView* null[] = { nullptr, nullptr , nullptr , nullptr , nullptr , nullptr , nullptr , nullptr , nullptr , nullptr };
	Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(0, 10, null);
    Armageddon::Interface::GetDeviceContext()->OMSetRenderTargets(1, pTargetView.GetAddressOf(), DephtStencilView.Get());

    //Armageddon::Interface::GetDeviceContext()->OMSetRenderTargets(1, pTargetView.GetAddressOf(), nullptr);

}
void OffScreenRenderTarget::Bind(ID3D11DeviceContext* DeviceContext, ID3D11DepthStencilView* dephStencil)
{
    
    Armageddon::Interface::GetDeviceContext()->OMSetRenderTargets(1, pTargetView.GetAddressOf(), dephStencil);

}

void OffScreenRenderTarget::Clear(ID3D11DeviceContext* DeviceContext)
{
    float color[] = { 0.1f,0.1f,0.1f,1.0f };

    Armageddon::Interface::GetDeviceContext()->ClearRenderTargetView(pTargetView.Get(), color);
    Armageddon::Interface::GetDeviceContext()->ClearDepthStencilView(this->DephtStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL  , 1.0f, 1);

}

void OffScreenRenderTarget::BindBackBuffer(ID3D11DeviceContext* DeviceContext, ID3D11RenderTargetView* render)
{
    Armageddon::Interface::GetDeviceContext()->OMSetRenderTargets(1, pTargetView.GetAddressOf(), nullptr);

}

void OffScreenRenderTarget::BindBackBuffer(ID3D11DeviceContext* DeviceContext, ID3D11RenderTargetView** renderTarget, ID3D11DepthStencilView* dephStencil)
{
    Armageddon::Interface::GetDeviceContext()->OMSetRenderTargets(1, renderTarget, dephStencil);

}

void OffScreenRenderTarget::UnBind()
{
	ID3D11RenderTargetView* RDTVNULL = nullptr;
	Armageddon::Interface::GetDeviceContext()->OMSetRenderTargets(1, &RDTVNULL, nullptr);
	
	
}
