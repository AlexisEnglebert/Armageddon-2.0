#include "DirectX11Renderer.h"

bool Armageddon::DirectX11Renderer::Init(HWND hwnd, float width, float height)
{
    //TODO : Faire en sorte que �a soit bien foutu

     IMGUI_CHECKVERSION();
     this->m_Context = ImGui::CreateContext();
     Armageddon::Log::GetLogger()->info("IMGUI : CREATE CONTEXT");
     LoadImGuiStyle();


     if (Armageddon::RendererAPI::m_CurrentAPI == Armageddon::RendererAPI::API::Vulkan)
     {
     }
     else
     {
         if (!InitSwapChain(hwnd))
         {
             return false;
         }
         if (!CreateDephtStencilBuffer(width, height))
         {
             return false;
         }

         CreateRenderTargetView(width, height);
         m_OffScreenRenderTarget.Init(Armageddon::Interface::GetDevice().Get(), Armageddon::Interface::GetSwapChain().Get(), width, height);
         m_FrameBuffer.Init(width, height,DXGI_FORMAT_R16G16B16A16_FLOAT);
         m_Composite.Init(width, height, DXGI_FORMAT_R16G16B16A16_FLOAT);
         m_DepthPass = RenderTexture((float)width, (float)height, DXGI_FORMAT_R8G8B8A8_UNORM);
         FinalPassVertex = AssetManager::GetOrCreateVertexShader(L"Assets/Shaders/BloomThresholdVertex.cso");
         FinalPassPixel = AssetManager::GetOrCreatePixelShader(L"Assets/Shaders/CombinePixel.cso");

         DisplayVertex = AssetManager::GetOrCreateVertexShader(L"Assets/Shaders/DisplayVertex.cso");
         DisplayPixel = AssetManager::GetOrCreatePixelShader(L"Assets/Shaders/DisplayPixel.cso");
         
         CreateViewPort(width, height);

         /*Default Rasterizer Description*/
         D3D11_RASTERIZER_DESC rDesc; 
         ZeroMemory(&rDesc, sizeof(D3D11_RASTERIZER_DESC));
         rDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
         rDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
         CreateRasterizer(rDesc);

         D3D11_SAMPLER_DESC sDesc;
         ZeroMemory(&sDesc, sizeof(D3D11_SAMPLER_DESC));
         sDesc.Filter = D3D11_FILTER_ANISOTROPIC;
         sDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
         sDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
         sDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
         sDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;
         sDesc.MipLODBias = 0.0f;
         sDesc.MinLOD = 0.0f;
         sDesc.MaxLOD = D3D11_FLOAT32_MAX;
         HRESULT hr = Armageddon::Interface::GetDevice()->CreateSamplerState(&sDesc, Armageddon::Interface::GetSamplerState().GetAddressOf());
         if (FAILED(hr))
         {
             Armageddon::Log::GetLogger()->error("Failed Creating Default Sampler State");
         }

         sDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;

         hr = Armageddon::Interface::GetDevice()->CreateSamplerState(&sDesc, Armageddon::Interface::GetTrilinearSampler().GetAddressOf());
         if (FAILED(hr))
         {
             Armageddon::Log::GetLogger()->error("Failed Creating Trilinear Sampler State");
         }
         sDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
         sDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
         sDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

         hr = Armageddon::Interface::GetDevice()->CreateSamplerState(&sDesc, Armageddon::Interface::GetClampSampler().GetAddressOf());

         if (FAILED(hr))
         {
             Armageddon::Log::GetLogger()->error("Failed Creating Clamp Sampler State");
         }

         //CompareSamplerState

         sDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
         sDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
         sDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
         sDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
         sDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;

         hr = Armageddon::Interface::GetDevice()->CreateSamplerState(&sDesc, Armageddon::Interface::GetCompareSampler().GetAddressOf());

         if (FAILED(hr))
         {
             Armageddon::Log::GetLogger()->error("Failed Creating Compare Sampler State");
         }

         CreateAlphaBlendState();
         CreateDefaultBlendState();
         /*Init ImGui pour DirectX11*/
 
         //..\\Armageddon Editor\\Assets\\Texture\\Skybox\\HDR\\Arches_E_PineTree\\Arches_E_PineTree_3k.hdr
         //io.FontDefault =  io.Fonts->AddFontFromFileTTF("..\\Armageddon 2.0\\Assets\\fonts\\Roboto-Medium.ttf", 14.0f);

         ImGui_ImplWin32_Init(hwnd);
         ImGui_ImplDX11_Init(Armageddon::Interface::GetDevice().Get(), Armageddon::Interface::GetDeviceContext().Get());

         /*
         * Constant Buffer Index :
         * 0 - Transform
         * 1 - Light
         * 2 - RougnessBuffer ( for the prefileterd envmap)
         */

         g_TransformCBuffer.Create(D3D11_USAGE_DYNAMIC, 0);
         g_LightCBuffer.Create(D3D11_USAGE_DYNAMIC, 1);
         g_CameraCBuffer.Create(D3D11_USAGE_DYNAMIC, 2);
         g_PBRCBuffer.Create(D3D11_USAGE_DYNAMIC, 3);
         g_WorldCBuffer.Create(D3D11_USAGE_DYNAMIC, 4);
         g_VolumetricBuffer.Create(D3D11_USAGE_DYNAMIC, 5);

         m_VolumetricFog.Init(width,height);
         m_Envmap = EnvMap(L"Assets/Texture/Skybox/HDR/orbita_4k.hdr");
         m_quad = Armageddon::Renderer2D::GeneratePlane();
         m_bloom.Init();
         m_Cascade.Init(&m_camera);
         Armageddon::Log::GetLogger()->info("fffssssffffsss");

         return true;
     }
     return true;
}

void Armageddon::DirectX11Renderer::CreateViewPort(float width, float height)
{
    ZeroMemory(&Armageddon::Renderer::ViewPort, sizeof(D3D11_VIEWPORT));

	Armageddon::Renderer::ViewPort.TopLeftX = 0;
    Armageddon::Renderer::ViewPort.TopLeftY = 0;
    Armageddon::Renderer::ViewPort.Width = width;
    Armageddon::Renderer::ViewPort.Height = height;
    Armageddon::Renderer::ViewPort.MinDepth = 0.0f;
    Armageddon::Renderer::ViewPort.MaxDepth = 1.0f;

    this->m_screenPos[0] = width / 2.f;
    this->m_screenPos[1] = height / 2.f;

    Armageddon::Interface::GetDeviceContext()->RSSetViewports(1, &ViewPort);
}

bool Armageddon::DirectX11Renderer::InitSwapChain(HWND &hwnd)
{
    std::vector<Adpatater> Adaptater = AdaptaterReader::GetAdaptaterData();
    if (Adaptater.size() < 1)
    {
        Armageddon::Log::GetLogger()->error("Aucun adaptateur trouv� ! ");
        return false;
    }

    DXGI_SWAP_CHAIN_DESC swapCD;
    ZeroMemory(&swapCD, sizeof(DXGI_SWAP_CHAIN_DESC));
    swapCD.BufferDesc.Width = 0;
    swapCD.BufferDesc.Height = 0;
    swapCD.BufferDesc.RefreshRate.Numerator = 60;
    swapCD.BufferDesc.RefreshRate.Denominator = 1;
    swapCD.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapCD.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapCD.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapCD.SampleDesc.Count = 1;
    swapCD.SampleDesc.Quality = 0;
    swapCD.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapCD.BufferCount = 2;
    swapCD.OutputWindow = hwnd;
    swapCD.Windowed = TRUE;
    swapCD.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapCD.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    int flags = D3D11_CREATE_DEVICE_DISABLE_GPU_TIMEOUT;
//#if _DEBUG
    flags = D3D11_CREATE_DEVICE_DISABLE_GPU_TIMEOUT | D3D11_CREATE_DEVICE_DEBUG;
//#endif
    HRESULT result = D3D11CreateDeviceAndSwapChain(Adaptater[0].ptrAdaptater,
        D3D_DRIVER_TYPE_UNKNOWN,
        NULL,
        flags,
        featureLevelArray,
        2,
        D3D11_SDK_VERSION,
        &swapCD,
        Armageddon::Interface::GetSwapChain().GetAddressOf(),
        Armageddon::Interface::GetDevice().GetAddressOf(),
        &featureLevel,
        Armageddon::Interface::GetDeviceContext().GetAddressOf());

    if (FAILED(result))
    {
        Armageddon::Log::GetLogger()->error("erreur lors de la cr�ation de la SwapChain");
        return false;
    }

    return true;
}

bool Armageddon::DirectX11Renderer::CreateDephtStencilBuffer(int width, int height)
{
     D3D11_TEXTURE2D_DESC DephtStencilDesc;
    DephtStencilDesc.Width = width;
    DephtStencilDesc.Height = height;
    DephtStencilDesc.MipLevels = 1;
    DephtStencilDesc.ArraySize = 1;
    DephtStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
    DephtStencilDesc.SampleDesc.Count = 1;
    DephtStencilDesc.SampleDesc.Quality = 0;
    DephtStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    DephtStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    DephtStencilDesc.CPUAccessFlags = 0;
    DephtStencilDesc.MiscFlags = 0;

    Armageddon::Interface::GetDevice()->CreateTexture2D(&DephtStencilDesc, NULL, this->DephtStencilBuffer.GetAddressOf());

    HRESULT result = Armageddon::Interface::GetDevice()->CreateDepthStencilView(this->DephtStencilBuffer.Get(), NULL, this->DephtStencilView.GetAddressOf());
    if (FAILED(result))
    {
        Armageddon::Log::GetLogger()->error("Erreur lors de la cr�tion du DephtStencilDesc {0}",result);
        return false;
    }



    D3D11_DEPTH_STENCIL_DESC Ddesc;
    ZeroMemory(&Ddesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
    Ddesc.DepthEnable = true;
    Ddesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
    Ddesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_EQUAL;

    result = Armageddon::Interface::GetDevice()->CreateDepthStencilState(&Ddesc, this->DephtStencilState.GetAddressOf());
    if (FAILED(result))
    {
        Armageddon::Log::GetLogger()->error("Erreur lors de la cr�tion du DephtStencilState {0}", result);
        return false;
    }
    return true;
}

void Armageddon::DirectX11Renderer::ResetDephtStencileBuffer()
{
    if (this->DephtStencilBuffer)
    {

        this->DephtStencilBuffer.Reset();
        this->DephtStencilBuffer = (std::nullptr_t) NULL;

    }
    if (this->DephtStencilView)
    {
        this->DephtStencilView.Reset();
        this->DephtStencilView = (std::nullptr_t) NULL;
    }
    if (this->DephtStencilState)
    {
        this->DephtStencilState.Reset();
        this->DephtStencilState = (std::nullptr_t) NULL;
    }
}

void Armageddon::DirectX11Renderer::CreateRenderTargetView(float width, float height)
{

    /*DXGI_MODE_DESC modeDesc;
    modeDesc.Width = width;
    modeDesc.Height = height,
    modeDesc.RefreshRate*/


    Armageddon::Interface::GetSwapChain()->ResizeTarget(0);

    ID3D11Texture2D* pBackBuffer;
    HRESULT result = Armageddon::Interface::GetSwapChain()->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));

    if (FAILED(result))
    {
        Armageddon::Log::GetLogger()->error("FAILED GETTING BUFFER FOR THE SWAP CHAIN");

    }
    result = Armageddon::Interface::GetDevice()->CreateRenderTargetView(pBackBuffer, NULL, Armageddon::Interface::GetRenderTargetView().GetAddressOf());

    if (FAILED(result))
    {
        Armageddon::Log::GetLogger()->error("FAILED CREATING THE RENDERTARGETVIEW {0}", result);

    }
    Armageddon::Log::GetLogger()->info("Render TargetView a bien �t� cr��");
    pBackBuffer->Release();
}

void Armageddon::DirectX11Renderer::CleanRenderTargetView()
{
      Armageddon::Interface::GetRenderTargetView().Reset();
      Armageddon::Interface::GetRenderTargetView() = (std::nullptr_t) NULL;
}

void Armageddon::DirectX11Renderer::CreateRasterizer(D3D11_RASTERIZER_DESC rDesc)
{
    HRESULT result = Armageddon::Interface::GetDevice()->CreateRasterizerState(&rDesc, Armageddon::Interface::GetDefaultRasterizerState().GetAddressOf());
    if (FAILED(result))
    {
        Armageddon::Log::GetLogger()->error("FAILED CREATING THE RASTERIZER STATE");
    }
}

void Armageddon::DirectX11Renderer::ChangeRasterizer(D3D11_RASTERIZER_DESC rDesc)
{
     Armageddon::Interface::GetDefaultRasterizerState().Reset();
    CreateRasterizer(rDesc);
}

void Armageddon::DirectX11Renderer::CreateAlphaBlendState()
{
      D3D11_BLEND_DESC BlendDesc;
	ZeroMemory(&BlendDesc, sizeof(D3D11_BLEND_DESC));
    BlendDesc.AlphaToCoverageEnable = true;
    BlendDesc.RenderTarget[0].BlendEnable = true;
    BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HRESULT hr = Armageddon::Interface::GetDevice()->CreateBlendState(&BlendDesc, Armageddon::Renderer::AlphaBlendState.GetAddressOf());
    if (FAILED(hr))
    {
        Armageddon::Log::GetLogger()->error("Error creating Alpha Blend state");
    }
}

void Armageddon::DirectX11Renderer::CreateDefaultBlendState()
{
    D3D11_BLEND_DESC BlendDesc;
	ZeroMemory(&BlendDesc, sizeof(D3D11_BLEND_DESC));
	BlendDesc.RenderTarget[0].BlendEnable = FALSE;
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	Armageddon::Interface::GetDevice()->CreateBlendState(&BlendDesc, Armageddon::Renderer::DefaultBlendState.GetAddressOf());
    BlendDesc.AlphaToCoverageEnable = false;
    BlendDesc.IndependentBlendEnable = false;
    BlendDesc.RenderTarget[0].BlendEnable = false;
    BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
    BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
    BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    Armageddon::Interface::GetDevice()->CreateBlendState(&BlendDesc, Armageddon::Interface::GetColorBlendState().GetAddressOf());

}
