#include "OuputLog.h"


ImGuiTextFilter				 OutputWindow::m_Filter;
std::vector<logdata_t>       OutputWindow::v_logData;


OutputWindow::OutputWindow()
{
	m_dangerIcon.Create(L"Ressources//Icones//Log//warning-512.png");

	Clear();
}

void OutputWindow::ImGuiDraw()
{
	ImGui::Begin("Output console");
	bool clear = ImGui::Button("Clear");
	ImGui::SameLine();
	bool copy = ImGui::Button("Copy");
	ImGui::SameLine();
	m_Filter.Draw("Filter", -100.0f);
	ImGui::Separator();
	ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_AlwaysVerticalScrollbar);
	if (clear)
		Clear();
	if (copy)
		ImGui::LogToClipboard();

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));


	if (m_Filter.IsActive())
	{
		// In this example we don't use the clipper when Filter is enabled.
		// This is because we don't have a random access on the result on our filter.
		// A real application processing logs with ten of thousands of entries may want to store the result of
		// search/filter.. especially if the filtering function is not trivial (e.g. reg-exp).
		for (int line_no = 0; line_no < v_logData.size(); line_no++)
		{
			if (m_Filter.PassFilter(v_logData[line_no].log.c_str()))
				DrawCell(v_logData[line_no].log.c_str());
		}
	}
	else
	{
		ImGuiListClipper clipper;
		clipper.Begin(v_logData.size());
		while (clipper.Step())
		{
			for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
				DrawCell(v_logData[line_no].log.c_str());

		}
		clipper.End();
	}
	ImGui::PopStyleVar();

	if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
		ImGui::SetScrollHereY(1.0f);
	ImGui::EndChild();
	ImGui::End();
	
}

void OutputWindow::Clear()
{
	v_logData.clear();
}

void OutputWindow::AddLog(LogType type,const char* fmt, ...)
{

	logdata_t data = {type,std::string(fmt)};
	v_logData.push_back(data);
	
}


//TODO: Improve this 
void OutputWindow::DrawCell(const char* text)
{
	ImGui::BeginChild("cell", ImVec2(0, 0), false, ImGuiWindowFlags_AlwaysUseWindowPadding);
	ImGui::Image(m_dangerIcon.GetRessourceView(), { 30,30 });
	ImGui::SameLine();
	ImGui::SetCursorPosY(( ImGui::GetCursorPosY() + 5.0f));
	ImGui::TextUnformatted(text);
	ImGui::EndChild();

}
