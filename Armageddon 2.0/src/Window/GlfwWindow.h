#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "../Macros.h"
#include "WindowInterface.h"


namespace Armageddon
{
    class DECL GlfwWindow : public WindowInterface
    {
        public:
		    GlfwWindow(int width, int height, std::wstring title, std::wstring wclass);
            ~GlfwWindow();
    };
}