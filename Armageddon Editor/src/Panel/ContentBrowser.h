#pragma once
#include "Log.h"
#include "imgui.h"
#include <iostream>
#include <filesystem>
#include <cstring>
#include "Material/Texture.h"
class ContentBrowser
{
public:
	ContentBrowser();
	void ImGuiDraw();
	void RecursiveSearch(std::filesystem::path path);
	void GenerateCells(const std::filesystem::directory_entry& file, float ButtonSize, std::filesystem::path& path);
	void DrawDirectoryBundle();
	Texture& ChosseIcon(const std::filesystem::directory_entry& file);
private:
	Texture m_FileIcon;

	Texture m_FileIconFBX;
	Texture m_FileIconMAT;
	Texture m_FileIconTIFF;
	Texture m_FileIconJPEG;
	Texture m_FileIconPNG;
	Texture m_FileIconHDR;

	Texture m_FolderIcon;
	Texture m_SearchIcon;
	Texture m_BackIcon;
	static std::filesystem::path Rootpath;

	static char SearchString[64];
	static char SearchStringDelta[64];
	std::vector<std::filesystem::directory_entry> v_SearchMatch;
};