#pragma once

#include "Window/Window.h"
#include <stdio.h>
#include <memory>
#include <iostream>
#include "Macros.h"
//#include "ImGuizmo.h"
#include "Renderer/RendererAPI.h"

#ifdef _WIN32
	#include "backends/imgui_impl_dx11.h"
	#include "backends/imgui_impl_win32.h"
#endif

#include "backends/imgui_impl_vulkan.h"
int main(int argc, char** argv);


namespace Armageddon
{
	class DECL Application
	{
	public :

		Application();
		virtual ~Application();

		virtual void OnUpdate() = 0 ;
		virtual void OnRender() = 0;
		virtual void OnInit() = 0;
		virtual void ImGuiRender() = 0;

		void ImGuiBegin();
		void ImGuiEnd();

		void ApplicationRun();

		//inline Armageddon::Window* GetWindow()				{ return IWind; };
		inline static Application* GetApplicationInsatnce() { return Instance; };
	private:

		static Application* Instance;
		Armageddon::Window* IWind;
		friend int ::main(int argc, char** argv);

	};

	Application* CreateApplication();

}