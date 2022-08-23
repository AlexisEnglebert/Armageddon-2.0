#pragma once
#define GLFW_INCLUDE_VULKAN

#if __linux__
//pass pour l'instant
#else
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"

#include <strsafe.h>
#include <tchar.h>

#endif

#include <windows.h>
#include <WinUser.h>
#include <windef.h>
#include <hidusage.h>


#include <GLFW/glfw3.h>
#include "../Macros.h"
#include "../Log.h"
#include "../Renderer/Renderer.h"
#include "KeyBoard.h"
#include "Events/MouseEvent.h"
#include "imgui.h"
#include <stdio.h>
#include <string>
#include <future>

#include <iostream>
#include <string.h>
#include "../Physics/PhysicsEngine.h"
#include "../Renderer/RendererAPI.h"
namespace Armageddon
{
	class DECL Window
	{
	public:
		Window(){}
		~Window(){}
		Window(int height, int width, std::wstring title, std::wstring wclass);
		void Win32Window(int width, int height, std::wstring title, std::wstring wclass);
		void GLFWindow(int width, int height, std::wstring title, std::wstring wclass);
		LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wparam, LPARAM lparam);

		inline Renderer& GetRenderer() { return m_Renderer;			 };
		inline KeyBoard& GetKeyBoard() { return m_KeyBoard;			 };
		inline 	Armageddon::PhyicsEngine GetPhysicEngine() {return m_PhysEngine;};
		float GetAspectRatio()		   { return(w_width / w_height); };

		/*Use maximum of absatraction tu */
		void SetMouseCallBack(const std::function<void(MouseEvent::MEventType e, float x, float y)>& callbackfunc) { MouseCallBack = callbackfunc; };
		void setKeyBoardCallBack(const std::function<void(const unsigned char Key)>& CallbackFunc) { KeyBoardCallBack = CallbackFunc; };
		std::function<void()> ProceedMessages;



		std::wstring OpenDialog(const char* filter = "All Files (*.*)\0*.*\0", HWND owner = NULL);
		float w_height, w_width;

		bool ProcessMessage();

	private:
		void RegisterWindowClass();

		std::wstring w_title , w_class = L"";
		
		std::function<void(MouseEvent::MEventType e, float x, float y)> MouseCallBack;
		std::function<void(const unsigned char Key)> KeyBoardCallBack;


		static Window* WindowInstance;

		HWND WindowHandle; 
		HINSTANCE ModuleInstance;

		float m_old_mouse_posX = 0.0f;
		float m_old_mouse_posY = 0.0f;

		/*Graphics*/
		Renderer m_Renderer;

		Armageddon::PhyicsEngine m_PhysEngine;
		/*KeyBoard Handler*/
		KeyBoard m_KeyBoard;

	};
}