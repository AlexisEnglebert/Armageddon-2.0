#pragma once
#include "../Macros.h"
#include "../Log.h"
#include "../Renderer/Renderer.h"
#include "KeyBoard.h"
#include "Events/MouseEvent.h"
#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"
#include <stdio.h>
#include <string>
#include <WinUser.h>
#include <windef.h>
#include <future>
#include <hidusage.h>
#include <strsafe.h>
#include <iostream>
#include <windows.h>
#include <string.h>
#include <tchar.h>
#include "../Physics/PhysicsEngine.h"
namespace Armageddon
{
	class DECL Window
	{
	public:
		Window(){}
		~Window(){}
		Window(int height, int width, std::wstring title, std::wstring wclass);
		LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wparam, LPARAM lparam);
		bool ProcessMessage();

		inline Renderer& GetRenderer() { return m_Renderer;			 };
		inline KeyBoard& GetKeyBoard() { return m_KeyBoard;			 };
		inline 	Armageddon::PhyicsEngine GetPhysicEngine() {return m_PhysEngine;};
		float GetAspectRatio()		   { return(w_width / w_height); };

		void SetMouseCallBack(const std::function<void(MouseEvent::MEventType e, float x, float y)>& callbackfunc) { MouseCallBack = callbackfunc; };
		void setKeyBoardCallBack(const std::function<void(const unsigned char Key)>& CallbackFunc) { KeyBoardCallBack = CallbackFunc; };
		std::wstring OpenDialog(const char* filter = "All Files (*.*)\0*.*\0", HWND owner = NULL);
		int w_height, w_width;

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