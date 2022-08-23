#include "Renderer.h"
#include "../Application.h"

ConstantBuffer<LightBuffer> Armageddon::Renderer::g_LightCBuffer;
ConstantBuffer<TransFormBuffer> Armageddon::Renderer::g_TransformCBuffer;
ConstantBuffer<MaterialBuffer>  Armageddon::Renderer::g_PBRCBuffer;
ConstantBuffer<WorldBuffer>  Armageddon::Renderer::g_WorldCBuffer;
ConstantBuffer<CameraBuffer_t>  Armageddon::Renderer::g_CameraCBuffer;

ConstantBuffer<Armageddon::VolumetricBuffer_t> Armageddon::Renderer::g_VolumetricBuffer;

LightBuffer Armageddon::Renderer::g_LightBufferData;
WorldBuffer Armageddon::Renderer::g_WorldBufferData;
CameraBuffer_t Armageddon::Renderer::g_CameraBufferData;
Armageddon::VolumetricBuffer_t Armageddon::Renderer::g_volumetricBufferData;

D3D11_VIEWPORT  Armageddon::Renderer::ViewPort;

Microsoft::WRL::ComPtr < ID3D11BlendState>		Armageddon::Renderer::AlphaBlendState;
Microsoft::WRL::ComPtr < ID3D11BlendState>		Armageddon::Renderer::DefaultBlendState;

std::vector<PointLight>  Armageddon::Renderer::g_PointLightsVector;
std::vector<DirectionalLight> Armageddon::Renderer::g_DirectLightsVector;




 bool Armageddon::Renderer::Init(HWND hwnd, float width,float height)
 {
     //TODO : Faire en sorte que ça soit bien foutu

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

void Armageddon::Renderer::CreateAlphaBlendState()
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

void Armageddon::Renderer::CreateDefaultBlendState()
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

void Armageddon::Renderer::LoadImGuiStyle()
{
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    constexpr auto ColorFromBytes = [](uint8_t r, uint8_t g, uint8_t b)
    {
        return ImVec4((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, 1.0f);
    };

    ImGuiStyle* style = &ImGui::GetStyle();
    ImVec4* colors = style->Colors;

    const ImVec4 bgColor = ColorFromBytes(36, 36, 36);
    const ImVec4 FrameColor = ColorFromBytes(127, 127, 127);
    const ImVec4 bgColorTitle = ImVec4(0.0862745098f, 0.0862745098f, 0.0862745098f, 1.0f);
    const ImVec4 lightBgColor = ColorFromBytes(82, 82, 85);
    const ImVec4 veryLightBgColor = ColorFromBytes(90, 90, 95);

    const ImVec4 panelColor = ColorFromBytes(51, 51, 55);
    const ImVec4 panelHoverColor = ColorFromBytes(29, 151, 236);
    const ImVec4 panelActiveColor = ColorFromBytes(0, 119, 200);

    const ImVec4 textColor = ColorFromBytes(255, 255, 255);
    const ImVec4 textDisabledColor = ColorFromBytes(151, 151, 151);
    const ImVec4 borderColor = ColorFromBytes(52, 52, 52);
    const ImVec4 borderColorShadow = ColorFromBytes(30, 30, 30);

    colors[ImGuiCol_Text] = textColor;
    colors[ImGuiCol_TextDisabled] = textDisabledColor;
    colors[ImGuiCol_TextSelectedBg] = panelActiveColor;
    colors[ImGuiCol_WindowBg] = bgColor;
    colors[ImGuiCol_ChildBg] = bgColor;
    colors[ImGuiCol_PopupBg] = bgColor;
    colors[ImGuiCol_Border] = borderColorShadow;
    colors[ImGuiCol_BorderShadow] = borderColorShadow;
    colors[ImGuiCol_FrameBg] = bgColorTitle;
    colors[ImGuiCol_FrameBgHovered] = panelHoverColor;
    colors[ImGuiCol_FrameBgActive] = panelActiveColor;
    colors[ImGuiCol_TitleBg] = bgColorTitle;
    colors[ImGuiCol_TitleBgActive] = bgColorTitle;
    colors[ImGuiCol_TitleBgCollapsed] = bgColorTitle;
    colors[ImGuiCol_MenuBarBg] = bgColorTitle;
    colors[ImGuiCol_ScrollbarBg] = panelColor;
    colors[ImGuiCol_ScrollbarGrab] = lightBgColor;
    colors[ImGuiCol_ScrollbarGrabHovered] = veryLightBgColor;
    colors[ImGuiCol_ScrollbarGrabActive] = veryLightBgColor;
    colors[ImGuiCol_CheckMark] = panelActiveColor;
    colors[ImGuiCol_SliderGrab] = panelHoverColor;
    colors[ImGuiCol_SliderGrabActive] = panelActiveColor;
    colors[ImGuiCol_Button] = panelColor;
    colors[ImGuiCol_ButtonHovered] = panelHoverColor;
    colors[ImGuiCol_ButtonActive] = panelHoverColor;
    colors[ImGuiCol_Header] = panelColor;
    colors[ImGuiCol_HeaderHovered] = panelHoverColor;
    colors[ImGuiCol_HeaderActive] = panelActiveColor;
    colors[ImGuiCol_Separator] = borderColor;
    colors[ImGuiCol_SeparatorHovered] = borderColor;
    colors[ImGuiCol_SeparatorActive] = borderColor;
    colors[ImGuiCol_ResizeGrip] = bgColor;
    colors[ImGuiCol_ResizeGripHovered] = panelColor;
    colors[ImGuiCol_ResizeGripActive] = lightBgColor;
    colors[ImGuiCol_PlotLines] = panelActiveColor;
    colors[ImGuiCol_PlotLinesHovered] = panelHoverColor;
    colors[ImGuiCol_PlotHistogram] = panelActiveColor;
    colors[ImGuiCol_PlotHistogramHovered] = panelHoverColor;
    colors[ImGuiCol_DragDropTarget] = bgColor;
    colors[ImGuiCol_NavHighlight] = bgColor;
    colors[ImGuiCol_DockingPreview] = panelActiveColor;
    colors[ImGuiCol_Tab] = bgColor;
    colors[ImGuiCol_TabActive] = panelActiveColor;
    colors[ImGuiCol_TabUnfocused] = bgColor;
    colors[ImGuiCol_TabUnfocusedActive] = bgColor;
    colors[ImGuiCol_TabHovered] = bgColor;

    style->ItemSpacing = ImVec2(10, 7);
    style->IndentSpacing = 12;
    style->ScrollbarSize = 10;

    style->WindowRounding = 4;
    style->FrameRounding = 4;
    style->PopupRounding = 4;
    style->ScrollbarRounding = 6;
    style->GrabRounding = 4;
    style->TabRounding = 4;
    style->WindowTitleAlign = ImVec2(1.0f, 0.5f);
    style->DisplaySafeAreaPadding = ImVec2(4, 4);
}

void Armageddon::Renderer::ResizeBuffer(float width, float height)
{
    CleanRenderTargetView();

    m_OffScreenRenderTarget.CleanRenderTargetView();

    ResetDephtStencileBuffer();

  

    HRESULT hr = Armageddon::Interface::GetSwapChain()->ResizeBuffers(0, (UINT)width, (UINT)height, DXGI_FORMAT_UNKNOWN, 0);

    if (FAILED(hr))
    {
        Armageddon::Log::GetLogger()->error("ERROR WHEN RESIZING THE BUFFER  :[{0}]", hr);
    }

    Armageddon::Log::GetLogger()->info("Resizing buffer");

    CreateRenderTargetView(width,height);
    m_OffScreenRenderTarget.ResizeRenderTargetView(width, height,nullptr);
    m_FrameBuffer.ResizeTexture(width, height);
    m_Composite.ResizeTexture(width, height);
    m_DepthPass.ResizeTexture(width, height);
    m_VolumetricFog.m_VolumetricTexture.ResizeTexture(width, height);
    CreateViewPort(width, height);
    CreateDephtStencilBuffer(width, height);  
    m_camera.SetProjectionValues(90.0f, width / height, 0.1f, 100.0f);
}

bool Armageddon::Renderer::InitSwapChain(HWND& hwnd)
{

    std::vector<Adpatater> Adaptater = AdaptaterReader::GetAdaptaterData();
    if (Adaptater.size() < 1)
    {
        Armageddon::Log::GetLogger()->error("Aucun adaptateur trouvé ! ");
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
        Armageddon::Log::GetLogger()->error("erreur lors de la création de la SwapChain");
        return false;
    }

    return true;
}

bool Armageddon::Renderer::CreateDephtStencilBuffer(int width, int height)
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
        Armageddon::Log::GetLogger()->error("Erreur lors de la crétion du DephtStencilDesc {0}",result);
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
        Armageddon::Log::GetLogger()->error("Erreur lors de la crétion du DephtStencilState {0}", result);
        return false;
    }
    return true;
}

void Armageddon::Renderer::ResetDephtStencileBuffer()
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

void Armageddon::Renderer::CreateRenderTargetView(float width, float height)
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
    Armageddon::Log::GetLogger()->info("Render TargetView a bien été créé");
    pBackBuffer->Release();
}

void Armageddon::Renderer::CleanRenderTargetView()
{
        Armageddon::Interface::GetRenderTargetView().Reset();
        Armageddon::Interface::GetRenderTargetView() = (std::nullptr_t) NULL;
}



void Armageddon::Renderer::CreateViewPort(float width, float height)
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

void Armageddon::Renderer::CreateRasterizer(D3D11_RASTERIZER_DESC rDesc)
{
    HRESULT result = Armageddon::Interface::GetDevice()->CreateRasterizerState(&rDesc, Armageddon::Interface::GetDefaultRasterizerState().GetAddressOf());
    if (FAILED(result))
    {
        Armageddon::Log::GetLogger()->error("FAILED CREATING THE RASTERIZER STATE");
    }
}

void Armageddon::Renderer::ChangeRasterizer(D3D11_RASTERIZER_DESC rDesc)
{
    Armageddon::Interface::GetDefaultRasterizerState().Reset();
    CreateRasterizer(rDesc);
}
bool show_demo_window = true;
bool show_another_window = false;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

void Armageddon::Renderer::RenderFrame()
{
    float BackGroundColor[] = { 0.1f,0.1f,0.1f,1.0f };

    Armageddon::Interface::GetDeviceContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    Armageddon::Interface::GetDeviceContext()->RSSetState(Armageddon::Interface::GetDefaultRasterizerState().Get());
    Armageddon::Interface::GetDeviceContext()->RSSetViewports(1, &Armageddon::Renderer::ViewPort);

   
    //ICI
    Armageddon::Application::GetApplicationInsatnce()->ImGuiBegin();
    Armageddon::Application::GetApplicationInsatnce()->ImGuiRender();
	Armageddon::Application::GetApplicationInsatnce()->OnRender();
    ImGui::Render();

    m_OffScreenRenderTarget.BindBackBuffer(Armageddon::Interface::GetDeviceContext().Get(),Armageddon::Interface::GetRenderTargetView().GetAddressOf(), this->DephtStencilView.Get()); // on remet le buffer normal
   
    Armageddon::Interface::GetDeviceContext()->ClearRenderTargetView(Armageddon::Interface::GetRenderTargetView().Get(), BackGroundColor);
    Armageddon::Interface::GetDeviceContext()->ClearDepthStencilView(this->DephtStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    Armageddon::Interface::GetDeviceContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    if (displayScene)
    {
        Armageddon::Interface::GetDeviceContext()->PSSetSamplers(0, 1, Armageddon::Interface::GetSamplerState().GetAddressOf());
        Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(0, 1, m_Composite.GetRessourceViewPtr());

        for (auto& submesh : m_quad.v_SubMeshes)
        {
            Armageddon::Interface::GetDeviceContext()->IASetInputLayout(DisplayVertex.GetInputLayout());
            Armageddon::Interface::GetDeviceContext()->PSSetShader(DisplayPixel.GetShader(), nullptr, 0);
            Armageddon::Interface::GetDeviceContext()->VSSetShader(DisplayVertex.GetShader(), nullptr, 0);

            submesh.BindVertexBuffer();
            submesh.BindIndexBuffer();
            submesh.DrawIndexed();
        }
    }

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }

   HRESULT hr =  Armageddon::Interface::GetSwapChain()->Present(1, 0);  //1 = Vsync
   if (FAILED(hr))
   {
       Armageddon::Log::GetLogger()->error("ERROR WHILE PRESENTING [{0}]", hr);
       if (hr == DXGI_ERROR_DEVICE_REMOVED)
       {
           Armageddon::Log::GetLogger()->error("DEVICE REMOVED");
           HRESULT reason = Armageddon::Interface::GetDevice()->GetDeviceRemovedReason();
           Armageddon::Log::GetLogger()->error("DEVICE REMOVED REASON : [{0}]",reason);


       }
   }

}

void Armageddon::Renderer::SubmitMesh( Mesh& mesh)
{
    this->m_Meshes.push_back(mesh);
}

Armageddon::Renderer::~Renderer()
{
    ImGui::DestroyContext(this->m_Context);
}
