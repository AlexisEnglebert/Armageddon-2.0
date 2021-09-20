#include "ContentBrowser.h"
char ContentBrowser::SearchString[64] = "";
char ContentBrowser::SearchStringDelta[64] = "";


ContentBrowser::ContentBrowser()
{
	m_FileIcon.Create(L"Ressources//Icones//ContentBrowser//FileIcon.png");
	m_FolderIcon.Create(L"Ressources//Icones//ContentBrowser//FolderIcon.png");
	m_SearchIcon.Create(L"Ressources//Icones//ContentBrowser//SearchIcon.png");
	m_BackIcon.Create(L"Ressources//Icones//ContentBrowser//BackIcon.png");
	m_FileIconFBX.Create(L"Ressources//Icones//ContentBrowser//FileIconFBX.png");
	m_FileIconMAT.Create(L"Ressources//Icones//ContentBrowser//FileIconMAT.png");
	m_FileIconTIFF.Create(L"Ressources//Icones//ContentBrowser//FileIconTIFF.png");
	m_FileIconJPEG.Create(L"Ressources//Icones//ContentBrowser//FileIconJPEG.png");
	m_FileIconPNG.Create(L"Ressources//Icones//ContentBrowser//FileIconPNG.png");
	m_FileIconHDR.Create(L"Ressources//Icones//ContentBrowser//FileIconHDR.png");
}
std::filesystem::path ContentBrowser::Rootpath = "Assets";

void ContentBrowser::ImGuiDraw()
{

	
	 ImGui::Begin("Content Browser");
	 ImGui::Columns(2);
	 ImGui::SetColumnWidth(0, 150.0f);
	 DrawDirectoryBundle();
	 ImGui::NextColumn();
	 ImGui::BeginChild("Content");
	 	/*La search bar*/
	 if (ImGui::ImageButton(m_BackIcon.GetRessourceView(), { 30,30 }))
	 {
		 if(Rootpath != "Assets")
			Rootpath = Rootpath.parent_path();
	 }
	 ImGui::SameLine();
	 ImGui::InputTextWithHint("##Search","Search....", SearchString, 64);
	 ImGui::SameLine();
	 ImGui::PushStyleColor(ImGuiCol_Button, (0, 0, 0, 0));
	 ImGui::ImageButton(m_SearchIcon.GetRessourceView(), { 28, 28 });
	 ImGui::PopStyleColor();
	 ImGui::Separator();
	 
	 /*Les cellules avec les fichiers*/
	float padding = 10.0f;
	float ButtonSize = 96.0f;
	float CellSize = padding + ButtonSize;
	float WindowWidth = ImGui::GetWindowContentRegionWidth();
	int nColumns = WindowWidth / CellSize;
	if (nColumns <= 1)
		nColumns = 1;
	ImGui::Columns(nColumns,0,false);

	if (strnlen(SearchString, sizeof(SearchString)) == 0 )
	{

		for (auto& file : std::filesystem::directory_iterator(Rootpath))
		{
			GenerateCells(file, ButtonSize, Rootpath);
		}
	}
	else
	{
		if (memcmp(&SearchStringDelta, &SearchString, sizeof(SearchString))) {
			v_SearchMatch.clear();
			memcpy(&SearchStringDelta, &SearchString, sizeof(SearchString));
			RecursiveSearch(Rootpath);
		}
		for (auto& file : v_SearchMatch)
		{
			GenerateCells(file, ButtonSize, Rootpath);
		}
	}
	ImGui::EndChild();
	//ImGui::Columns(1);
	ImGui::End();
}

void ContentBrowser::RecursiveSearch(std::filesystem::path path)
{
					
	for (auto& file : std::filesystem::directory_iterator(path))
	{
		
		if (file.is_directory())
		{
			std::filesystem::path newPath = path /= file.path().filename();
		//	Armageddon::Log::GetLogger()->trace(newPath);
			RecursiveSearch(newPath);
			path = file.path().parent_path();
		}
		else
		{
			if (!file.path().filename().string().find(SearchString))
			{
			//	Armageddon::Log::GetLogger()->trace(file.path().filename().string().c_str());
				v_SearchMatch.push_back(file);
			}
		}
		if (file.path().parent_path() == Rootpath)
		{
			return;
		}
	}
	
	//Armageddon::Log::GetLogger()->trace("Finished a RecursiveSearch thread   {0}" ,path.string().c_str());
}

void ContentBrowser::GenerateCells(const std::filesystem::directory_entry& file,float ButtonSize, std::filesystem::path& path)
{
	ImGui::PushStyleColor(ImGuiCol_Button, (0, 0, 0, 0));
	ImGui::PushID(file.path().string().c_str());
	Texture icon = ChosseIcon(file);
	ImGui::ImageButton(icon.GetRessourceView(), { ButtonSize,ButtonSize });

	if (ImGui::BeginDragDropSource())
	{
		ImGui::SetDragDropPayload("ASSET", file.path().string().c_str(), file.path().string().size() + 1 * sizeof(char));
		ImGui::Image(icon.GetRessourceView(), { ButtonSize / 2 ,ButtonSize / 2 });
		ImGui::Text(file.path().string().c_str());
		ImGui::EndDragDropSource();
	}

	ImGui::PopStyleColor();
	if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
	{
		if (file.is_directory())
		{
		//	Armageddon::Log::GetLogger()->trace(file.path().filename());
			path /= file.path().filename();
		//	Armageddon::Log::GetLogger()->trace(path);

		}
	}
	ImGui::PopID();
	ImGui::TextWrapped(file.path().filename().string().c_str());

	ImGui::NextColumn();
}

void ContentBrowser::DrawDirectoryBundle()
{
	if (ImGui::TreeNodeEx(Rootpath.string().c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed))
	{
		for (auto& file : std::filesystem::directory_iterator(Rootpath))
		{
			if (file.is_directory())
				ImGui::Text(file.path().string().c_str());
		}
	ImGui::TreePop();
	}
}

Texture& ContentBrowser::ChosseIcon(const std::filesystem::directory_entry& file)
{

	if (file.is_directory())
		return m_FolderIcon;

	if (file.path().extension() == ".fbx")
		return m_FileIconFBX;
	if (file.path().extension() == ".mat")
		return m_FileIconMAT;
	if (file.path().extension() == ".tiff" || file.path().extension() == ".tif")
		return m_FileIconTIFF; 
	if (file.path().extension() == ".jpg" || file.path().extension() == ".jpeg")
		return m_FileIconJPEG;
	if (file.path().extension() == ".png")
		return m_FileIconPNG; 
	if (file.path().extension() == ".hdr")
		return m_FileIconHDR;
	return m_FileIcon;
}
