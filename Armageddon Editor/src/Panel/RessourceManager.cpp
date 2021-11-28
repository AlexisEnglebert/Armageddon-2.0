#include "RessourceManager.h"

void RessourceManager::ImGuiDraw()
{
	ImGui::Begin("RessourcesManager");
	ImGui::Columns(3);
	for (auto iterator = AssetManager::m_MaterialMap.begin(); iterator != AssetManager::m_MaterialMap.cend(); iterator++)
	{
		ImGui::Text(std::to_string((uint64_t)iterator->first).c_str());
		ImGui::NextColumn();
		ImGui::Text(iterator->second.m_AssetName.c_str());
		ImGui::NextColumn();
		ImGui::Text(std::to_string((int)iterator->second.m_AssetType).c_str());
		ImGui::NextColumn();

	}
	ImGui::End();
}
