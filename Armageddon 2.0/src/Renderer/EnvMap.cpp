#include "EnvMap.h"
#include "Renderer.h"

EnvMap::EnvMap(const std::filesystem::path& HDRPath)
{
	m_Cube = Armageddon::Primitives::GenerateCube();
 

	m_Cube.v_MaterialReference[0] = AssetManager::GetOrCreateMaterial("EnvmapMaterial");
	AssetManager::m_MaterialMap[m_Cube.v_MaterialReference[0]].SetPixelShader(L"..\\bin\\Debug-x64\\Armageddon 2.0\\SkyBoxPixel.cso");
	AssetManager::m_MaterialMap[m_Cube.v_MaterialReference[0]].SetVertexShader(L"..\\bin\\Debug-x64\\Armageddon 2.0\\SkyBoxVertex.cso");
	m_envMapTexture.CreateCubeMap(HDRPath);
	m_convEnvMapTexture.CreateIrradiancedMap(m_envMapTexture.GetRessourceViewPtr());
	m_BRFLutTexture.Create(L"..\\Armageddon Editor\\Assets\\Texture\\Skybox\\ibl_brdf_lut.png");
	m_PreFilteredEnvMap.CreatePreFilteredMap(m_envMapTexture.GetRessourceViewPtr());
	D3D11_RASTERIZER_DESC rDesc;
 	ZeroMemory(&rDesc, sizeof(D3D11_RASTERIZER_DESC));
	rDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
	 Armageddon::Interface::GetDevice()->CreateRasterizerState(&rDesc, rasterizerState.GetAddressOf());
}

void EnvMap::Render(Armageddon::Camera* m_camera)	
{
	
	Armageddon::Interface::GetDeviceContext()->RSSetState(rasterizerState.Get());

	TransFormBuffer m_TransForm;
	DirectX::XMMATRIX ViewMatrix = m_camera->GetViewMatrix();
	ViewMatrix.r[3].m128_f32[0] = 0.0f;
	ViewMatrix.r[3].m128_f32[1] = 0.0f;
	ViewMatrix.r[3].m128_f32[2] = 0.0f;
	DirectX::XMMatrixRotationRollPitchYaw(m_camera->GetRotation().x, m_camera->GetRotation().y, m_camera->GetRotation().z);
	m_TransForm.ProjectionMat = m_camera->GetProjectionMatrix();
	m_TransForm.ViewMat = ViewMatrix;
	Armageddon::Renderer::g_TransformCBuffer.SetDynamicData(&m_TransForm);
	Armageddon::Renderer::g_TransformCBuffer.BindPS();
	Armageddon::Renderer::g_TransformCBuffer.BindVS();
	
	Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(17,1, m_envMapTexture.GetRessourceViewPtr());
	Armageddon::Interface::GetDeviceContext()->PSSetSamplers(0, 1, Armageddon::Interface::GetSamplerState().GetAddressOf());

	m_Cube.v_SubMeshes[0].BindVertexBuffer();
	m_Cube.v_SubMeshes[0].BindIndexBuffer();
	m_Cube.BindMaterial(0);
	UINT numViewport = 1;
	D3D11_VIEWPORT Viewport;
	Armageddon::Interface::GetDeviceContext()->RSGetViewports(&numViewport, &Viewport);
	Viewport.MinDepth = 0.999999f;
	Viewport.MaxDepth = 1.0f;
	Armageddon::Interface::GetDeviceContext()->RSSetViewports(1,&Viewport);
	m_Cube.v_SubMeshes[0].DrawIndexed();
	Viewport.MinDepth = 0.0f;
	Viewport.MaxDepth = 1.0f;
	Armageddon::Interface::GetDeviceContext()->RSSetViewports(1, &Viewport);

	Armageddon::Interface::GetDeviceContext()->RSSetState(Armageddon::Interface::GetDefaultRasterizerState().Get());


}
