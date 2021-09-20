#include "Application.h"
namespace Armageddon
{
	Application* Application::Instance = nullptr;

	Application::Application()
	{
		Instance = this;
		auto window = new Armageddon::Window(1920, 1080, L"Armageddon", L"ArmageddonClass");
		this->IWind = window;

	}

	Application::~Application()
	{
		delete this->IWind;
	}
	void Armageddon::Application::ImGuiBegin()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();	
		ImGuizmo::BeginFrame();

	}
	void Armageddon::Application::ImGuiEnd()
	{

	}
	void Armageddon::Application::ApplicationRun()
	{
       this->OnInit();
	   Armageddon::Log::GetLogger()->trace("ApplicationRun");

		while (IWind->ProcessMessage())
		{
			this->OnUpdate();
		}
	}
}
