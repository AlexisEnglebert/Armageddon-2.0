#pragma once


#if _WIN32
#include "Win32Window.h"

#endif
#include "WindowInterface.h"
#include "GlfwWindow.h"
#include "imgui.h"

#include <stdio.h>
#include <string>
#include <string.h>

//#include "../Physics/PhysicsEngine.h"
#include "../Renderer/RendererAPI.h"
#include "../Renderer/Renderer.h"



namespace Armageddon
{
	class DECL Window
	{
	public:
		Window(){}
		~Window(){delete wind;}
		Window(int width, int height, std::wstring title, std::wstring wclass);
		
		WindowInterface* wind;
		static Window* WindowInstance;

		bool ProcessMessage();
		//inline 	Armageddon::PhyicsEngine GetPhysicEngine() {return m_PhysEngine;};
		//inline Renderer& GetRenderer() { return m_Renderer; };
	private:
		/*Graphics*/
		Renderer m_Renderer;
		/*Physic*/
		//Armageddon::PhyicsEngine m_PhysEngine;



		/*if (!m_Renderer.Init(this->WindowHandle, this->w_height, this->w_width))
		{
			Armageddon::Log::GetLogger()->trace("Erreur lors de la cr�tation du renderer");

		}*/
	};
}