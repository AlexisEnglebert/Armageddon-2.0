#pragma once
#include <string>
#include <vector>
#include "imgui.h"
#include "Material/Texture.h"

static enum class LogType
{
	INFO,
	WARNING,
	DANGER,
};

typedef struct
{
	LogType type;
	std::string log;
}logdata_t;

class OutputWindow
{
//Log system From Imgui_demo.cpp
public:
	OutputWindow();
	void ImGuiDraw();
	void Clear();
	static void AddLog(LogType type,const char* fmt, ...);
	void DrawCell(const char* text);

private:

	Texture m_dangerIcon;

	static ImGuiTextFilter				m_Filter;
	static std::vector<logdata_t>       v_logData;

};