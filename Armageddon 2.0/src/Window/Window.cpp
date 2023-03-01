#include "Window.h"

//extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Armageddon::Window* Armageddon::Window::WindowInstance = nullptr;

Armageddon::Window::Window(int width, int height, std::wstring title, std::wstring wclass)
{
    Armageddon::Log::GetLogger()->info("Creating the Window ??");
    this->WindowInstance = this;
    if(Armageddon::RendererAPI::is_api(Armageddon::RendererAPI::API::DirectX)){
        #if WINDOWS
            this->wind = new Armageddon::Win32Window(width, height, title, wclass);
        #endif
    }else{
        // TODO Ça doit être un simple init ? 
        // TODO trouver mieux que de passer l'instance par argument pour l'instant on laisse comme ça
        this->wind = new Armageddon::GlfwWindow(width, height, title, wclass);
        if(!m_Renderer.InitVulkan(reinterpret_cast<Armageddon::GlfwWindow*>(this->wind)->GetVkInstance()))
        {
            m_closeWindow = true;
        } 
    }
}

bool Armageddon::Window::ProcessMessage()
{
    if(m_closeWindow) return false;

    return wind->ProcessMessage();
}









