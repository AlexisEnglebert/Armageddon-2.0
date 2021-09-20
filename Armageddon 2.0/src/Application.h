#pragma once

#include <stdio.h>
#include <memory>
#include <iostream>
#include "Macros.h"
#include "Window\Window.h"
#include "ImGuizmo.h"


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

		inline Armageddon::Window* GetWindow()				{ return IWind; };
		inline static Application* GetApplicationInsatnce() { return Instance; };
	private:

		static Application* Instance;
		Armageddon::Window* IWind;
		friend int ::main(int argc, char** argv);

	};

	Application* CreateApplication();

}