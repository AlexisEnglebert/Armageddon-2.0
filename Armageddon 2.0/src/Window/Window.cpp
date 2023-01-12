#include "Window.h"

//extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Armageddon::Window* Armageddon::Window::WindowInstance = nullptr;

Armageddon::Window::Window(int width, int height, std::wstring title, std::wstring wclass)
{
    this->WindowInstance = this;
    if(Armageddon::RendererAPI::is_api(Armageddon::RendererAPI::API::DirectX)){
        #if WINDOWS
            //this->wind = (Armageddon::WindowInterface)Armageddon::Win32Window(width, height, title, wclass);
        #endif
    }else{
        //this->wind = (Armageddon::WindowInterface)Armageddon::GlfwWindow(width, height, title, wclass);
    }
}

bool Armageddon::Window::ProcessMessage()
{
    return this->wind.ProcessMessage();
}









