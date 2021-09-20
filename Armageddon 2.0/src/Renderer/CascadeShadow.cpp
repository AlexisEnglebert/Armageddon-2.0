#include "CascadeShadow.h"
#include "Renderer.h"
#include "../Application.h"
Armageddon::CascadeShadow::CascadeShadow(Camera* m_cam,Scene& m_scene) : m_pCamera(m_cam) , m_scene(m_scene)
{
	//TODO texture depht buffer ou  cascade 
	m_CascadeLightTex.Init(Armageddon::Interface::GetDevice().Get(), Armageddon::Interface::GetSwapChain().Get(), Armageddon::Application::GetApplicationInsatnce()->GetWindow()->w_width, Armageddon::Application::GetApplicationInsatnce()->GetWindow()->w_height
		);

	px = AssetManager::GetOrCreatePixelShader(L"..\\bin\\Debug-x64\\Armageddon 2.0\\ShadowMapPixel.cso");
    vx = AssetManager::GetOrCreateVertexShader(L"..\\bin\\Debug-x64\\Armageddon 2.0\\ShadowMapVertex.cso");
}

void Armageddon::CascadeShadow::CalculateMatrices()
{
/*	DirectX::XMMATRIX ViewProjection = DirectX::XMMatrixMultiply(m_pCamera->GetViewMatrix(), m_pCamera->GetProjectionMatrix());
	DirectX::XMMATRIX InverseViewProjection = DirectX::XMMatrixInverse(nullptr,ViewProjection);
	DirectX::XMMATRIX InverseView = DirectX::XMMatrixInverse(nullptr, m_pCamera->GetViewMatrix());*/

	/*/if (Armageddon::Renderer::g_DirectLightsVector.size() > 0) {
		DirectX::XMVECTOR InvLightDir = -DirectX::XMVectorSet(Armageddon::Renderer::g_DirectLightsVector[0].Direction.x, Armageddon::Renderer::g_DirectLightsVector[0].Direction.y, Armageddon::Renderer::g_DirectLightsVector[0].Direction.z, 0.0f);
		LightView = DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),InvLightDir , DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
		LightProjection = DirectX::XMMatrixOrthographicLH(10, -10, 0.1f, 1000.0);
	}*/

	/*Premièrement on doit calculer les 8 points du Frustum */
	/*DirectX::XMFLOAT3 FurstumCorners[8] =
	{
		//Near face
		{  1.0f,  1.0f, -1.0f },
		{ -1.0f,  1.0f, -1.0f },
		{  1.0f, -1.0f, -1.0f },
		{ -1.0f, -1.0f, -1.0f},

		//Far face
		{  1.0f,  1.0f, 1.0f },
		{ -1.0f,  1.0f, 1.0f },
		{  1.0f, -1.0f, 1.0f },
		{ -1.0f, -1.0f, 1.0f },
	};

	XMFLOAT3 frustumCenter = { 0, 0, 0 };
	for (size_t i = 0; i < 8; i++)
	{
		frustumCenter = XMFLOAT3(frustumCenter.x + FurstumCorners[i].x,
			frustumCenter.y + FurstumCorners[i].y,
			frustumCenter.z + FurstumCorners[i].z);
	}


	/*Après on doit transformer les coordonées qui sont dans la viewSpace en dans la WorldSpace et dans la light space 
	
	float minX = 0;
	float maxX = 0;
	float minY = 0;
	float maxY = 0;
	float minZ = 0;
	float maxZ = 0;

	for (UINT i = 0; i < NUM_CASCADE; i++)
	{
		XMVECTOR FrustumVec = XMLoadFloat3(&FurstumCorners[i]);

		FrustumVec = XMVector4Transform(FrustumVec, InverseViewProjection);
		FrustumVec = XMVector4Transform(FrustumVec, LightView);
		XMStoreFloat3(&FurstumCorners[i], FrustumVec);

		/*Après on génère une Bouding Box qui nous donne les valeurs min et max de la projection orthographique 
		minX = std::min(minX, FurstumCorners[i].x);
		maxX = std::max(maxX, FurstumCorners[i].x);
		minY = std::min(minY, FurstumCorners[i].y);
		maxY = std::max(maxY, FurstumCorners[i].y);
		minZ = std::min(minZ, FurstumCorners[i].z);
		maxZ = std::max(maxZ, FurstumCorners[i].z);
	}

	 LightProjection = DirectX::XMMatrixOrthographicLH(maxX - minX, maxY - minY,minZ,maxZ);*/


	
}

void Armageddon::CascadeShadow::Update()
{
	if (Armageddon::Renderer::g_DirectLightsVector.size() > 0) 
	{
		DirectX::XMVECTOR InvLightDir = -DirectX::XMVectorSet(Armageddon::Renderer::g_LightBufferData.DirectionalLights[0].Direction.x, Armageddon::Renderer::g_LightBufferData.DirectionalLights[0].Direction.y, Armageddon::Renderer::g_LightBufferData.DirectionalLights[0].Direction.z, 0.0f);
		DirectX::XMVECTOR Position = DirectX::XMVectorSet(Armageddon::Application::GetApplicationInsatnce()->GetWindow()->GetRenderer().m_camera.GetPos().x, Armageddon::Application::GetApplicationInsatnce()->GetWindow()->GetRenderer().m_camera.GetPos().y, Armageddon::Application::GetApplicationInsatnce()->GetWindow()->GetRenderer().m_camera.GetPos().z, 0.0f);
		LightView = DirectX::XMMatrixLookAtLH( DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),InvLightDir, DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
		//LightView = Armageddon::Application::GetApplicationInsatnce()->GetWindow()->GetRenderer().m_camera.GetViewMatrix();
		LightProjection = DirectX::XMMatrixOrthographicLH(100.0f,100.0f, 1.0f, 7.5f);
		//LightProjection = Armageddon::Application::GetApplicationInsatnce()->GetWindow()->GetRenderer().m_camera.GetProjectionMatrix();

	}

	//Get la direction de la light 
	// 
	// 	   https://github.com/SurgeTechnologies/Electro/blob/master/Electro/src/Renderer/RenderPass/ShadowPass.cpp
	// 	   https://devansh.space/cascaded-shadow-maps
	// 	   https://ogldev.org/www/tutorial49/tutorial49.html
	// 	   https://gen-graphics.blogspot.com/2018/07/implementing-cascaded-shadow-maps-in.html
	// 
	// 	   https://github.com/Unity-Technologies/VolumetricLighting/blob/master/Assets/VolumetricFog/Shaders/Scatter.compute
	// 	   https://www.evl.uic.edu/sjames/cs525/shader.html
	// 	   http://www.alexandre-pestana.com/volumetric-lights/
	// 	   https://bartwronski.files.wordpress.com/2014/08/bwronski_volumetric_fog_siggraph2014.pdf
	// 	   https://fr.slideshare.net/BenjaminGlatzel/volumetric-lighting-for-many-lights-in-lords-of-the-fallen
	//loat lightAngleX = (float) Math.toDegrees(Math.acos(lightDirection.z));
	/* /float lightAngleY = (float)Math.toDegrees(Math.asin(lightDirection.x));
	float lightAngleZ = 0;
	Transformation.updateGenericViewMatrix(lightPosition, new Vector3f(lightAngleX, lightAngleY, lightAngleZ), lightViewMatrix)*/
}

