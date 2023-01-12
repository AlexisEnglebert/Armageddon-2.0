#pragma once

#ifdef _WIN32
	#include <cstdlib>
	#include <crtdbg.h>
	#include <stdlib.h>  
	#include <crtdbg.h>
	#define _CRTDBG_MAP_ALLOC

#endif

#include <stdio.h>
#include <memory>
#include <iostream>
#include "Application.h"
#include "Log.h"
#include "Window/Window.h"

#include "Renderer/RendererAPI.h"


extern Armageddon::Application* Armageddon::CreateApplication();

int main(int argc, char** argv)
{
	#if WINDOWS
		#if _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		#endif
		//_CrtSetBreakAlloc(898);
		CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	#endif
	
	Armageddon::Log::Init();
#if __linux__
	Armageddon::RendererAPI::SetRenderer(Armageddon::RendererAPI::API::Vulkan);
#else	
	Armageddon::Log::GetLogger()->info("Use Vulkan Rendeer ? (Y/N)");
	std::string result = "";
	std::cin >> result;
	if (result == "Y" || result == "y")
		Armageddon::RendererAPI::SetRenderer(Armageddon::RendererAPI::API::Vulkan);
	else
		Armageddon::RendererAPI::SetRenderer(Armageddon::RendererAPI::API::DirectX);

	Armageddon::Log::GetLogger()->info(Armageddon::RendererAPI::m_CurrentAPI);
#endif

    auto application = Armageddon::CreateApplication();
	application->ApplicationRun();
	delete application;


}
