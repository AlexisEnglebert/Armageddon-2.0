#pragma once


#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#include <stdlib.h>  
#include <crtdbg.h>
#include <stdio.h>
#include <memory>
#include <iostream>
#include "Application.h";
#include "Log.h";
#include "Window\Window.h";



extern Armageddon::Application* Armageddon::CreateApplication();

int main(int argc, char** argv)
{
#if _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	//_CrtSetBreakAlloc(898);
	CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	Armageddon::Log::Init();
	
    auto application = Armageddon::CreateApplication();

	application->ApplicationRun();
	delete application;


}
