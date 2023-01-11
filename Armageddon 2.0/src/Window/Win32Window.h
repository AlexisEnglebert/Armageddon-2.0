#pragma once

#include <windows.h>
#include <windef.h>
#include <hidusage.h>
#include <strsafe.h>
#include <tchar.h>

#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"

#include "../Macros.h"
#include "WindowInterface.h"

namespace Armageddon
{
    class DECL Win32Window : public WindowInterface
    {
        public:
		    Win32Window(int width, int height, std::wstring title, std::wstring wclass);
            ~Win32Window();
        	std::function<void()> ProceedMessages;
        private:
            void RegisterWindowClass();
            LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wparam, LPARAM lparam);

            HWND WindowHandle; 
		    HINSTANCE ModuleInstance;

    };
}