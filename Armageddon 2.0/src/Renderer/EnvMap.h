#pragma once
#include "../Macros.h"
#include "../Material/Texture.h"
#include "Camera/Camera.h"
#include "Primitives.h"
#include "Interface.h"
#include <filesystem>
class DECL EnvMap
{
public:
	EnvMap() = default;
	EnvMap(const std::filesystem::path& HDRPath);
	Mesh m_Cube;

	void Render(Armageddon::Camera* m_camera);
	EnvTexture m_envMapTexture;
	EnvTexture m_convEnvMapTexture;
	Texture m_BRFLutTexture;
	EnvTexture m_PreFilteredEnvMap;
private:

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;

};