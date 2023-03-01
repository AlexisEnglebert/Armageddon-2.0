#include "Renderer.h"
#include "../Application.h"

/*
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
*/

void Armageddon::Renderer::InitVulkan(VkInstance& instance)
{
<<<<<<< HEAD
    if(RendererAPI::is_api(RendererAPI::API::Vulkan)){
        // TODO NOT FOR DEBUG PURPOSE
        m_vk_renderer.InitVkSwapChain();
    }else{
        #if WINDOWS

        #endif
=======
    if(Armageddon::RendererAPI::is_api(Armageddon::RendererAPI::API::Vulkan)){
        // TODO NOT FOR DEBUG PURPOSE
        m_vk_renderer.Init(instance);
        
>>>>>>> 3a8a62a (src: working on vulkan renderer, selecting devices)
    }
}
void Armageddon::Renderer::InitDirectX()
{
    #if WINDOWS

    #endif
}
