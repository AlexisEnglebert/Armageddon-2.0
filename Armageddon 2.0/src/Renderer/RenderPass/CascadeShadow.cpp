#include "CascadeShadow.h"
#include "../Renderer.h"
#include "../../Application.h"



void Armageddon::CascadeShadow::CalculateMatrices()
{
	DirectX::XMMATRIX ViewProjection = DirectX::XMMatrixMultiply(m_pCamera->GetViewMatrix(), m_pCamera->GetProjectionMatrix());
	DirectX::XMMATRIX InverseViewProjection = DirectX::XMMatrixInverse(nullptr, ViewProjection);
	//TODO IF WE CHANGE THE NEAR OF FAR CLIP
	float nearClip = m_pCamera->NearZ;
	float farClip = m_pCamera->FarZ;
	float clipRange = farClip - nearClip;

	float minZ = nearClip;
	float maxZ = nearClip + clipRange;
	float range = maxZ - minZ;
	float ratio = maxZ / minZ;
	//Calculate the sub-frusta via : https://developer.nvidia.com/sites/all/modules/custom/gpugems/books/GPUGems3/elementLinks/0207equ02.jpg
	for (unsigned int i = 0; i < NUM_CASCADE; ++i)
	{
		float p = (i + 1) / static_cast<float>(NUM_CASCADE);
		float log = minZ * std::pow(ratio, p);
		float uniform = minZ + range * p;
		float d = lambda * (log - uniform) + uniform;
		m_FrustumSlices[i] = (d - nearClip) / clipRange;
		m_SplitDepth[i] = (m_pCamera->NearZ + m_FrustumSlices[i] * clipRange) * 1.0f;
		//m_SplitDepth[i] = (m_pCamera->NearZ + m_FrustumSlices[i] * clipRange) * 1.0f;
	}

	for (size_t i = 0; i < NUM_CASCADE; i++)
	{
		if (i == 0)
		{
			ComputeLightMatrix(0.0f, m_FrustumSlices[i], InverseViewProjection,i);
		}
		else
		{
			ComputeLightMatrix(m_FrustumSlices[i - 1], m_FrustumSlices[i], InverseViewProjection,i);
		}
	}

}


bool Armageddon::CascadeShadow::Init(Camera* m_camera)
{
	//TODO texture depht buffer ou  cascade 

	px = AssetManager::GetOrCreatePixelShader(L"Assets/Shaders/ShadowMapPixel.cso");
	vx = AssetManager::GetOrCreateVertexShader(L"Assets/Shaders/ShadowMapVertex.cso");

	//DIVIDE THE FRUSTUM into little pieces
	//good reference : https://developer.download.nvidia.com/SDK/10.5/opengl/src/cascaded_shadow_maps/doc/cascaded_shadow_maps.pdf
	for (unsigned int i = 0; i < NUM_CASCADE; ++i)
	{
		m_CascadeShadowMap[i].Init(SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT, DXGI_FORMAT_R32_TYPELESS);
	}


	m_pCamera = m_camera;
	return true;
}
//TODO BETTER WAY TO DO THIS
void Armageddon::CascadeShadow::ComputeLightMatrix(float nearPlane, float farPlane, DirectX::XMMATRIX InverseViewProjection,int indice)
{
	//Basic frustum corner (by default)
	DirectX::XMVECTOR frustumCorners[8] = 
	{
		DirectX::XMVECTOR{-1.0f,  1.0f, -1.0f,1.0f},
		DirectX::XMVECTOR{1.0f,  1.0f, -1.0f,1.0f},
		DirectX::XMVECTOR{1.0f, -1.0f, -1.0f,1.0f},
		DirectX::XMVECTOR{-1.0f, -1.0f, -1.0f,1.0f},
		DirectX::XMVECTOR{-1.0f,  1.0f,  1.0f,1.0f},
		DirectX::XMVECTOR{1.0f,  1.0f,  1.0f,1.0f},
		DirectX::XMVECTOR{1.0f, -1.0f,  1.0f,1.0f},
		DirectX::XMVECTOR{-1.0f, -1.0f,  1.0f,1.0f},
	};
	//Project corners into the world space

	for (uint32_t i = 0; i < 8; i++) 
	{
		DirectX::XMVECTOR invCorner = DirectX::XMVector4Transform(frustumCorners[i], InverseViewProjection);
		invCorner.m128_f32[0] /= invCorner.m128_f32[3];
		invCorner.m128_f32[1] /= invCorner.m128_f32[3];
		invCorner.m128_f32[2] /= invCorner.m128_f32[3];
		frustumCorners[i] = invCorner;

	}

	for (uint32_t i = 0; i < 4; i++) 
	{
		DirectX::XMVECTOR dist = frustumCorners[i + 4] - frustumCorners[i];
		frustumCorners[i + 4] = frustumCorners[i] + (dist * farPlane);
		frustumCorners[i] = frustumCorners[i] + (dist * nearPlane);
	}

	//Get the frustum center to compute the view & projection matrix
	DirectX::XMVECTOR frustumCenter = DirectX::XMVECTOR{ 0.0f };
	for (uint32_t i = 0; i < 8; i++) {
		frustumCenter += frustumCorners[i];
	}
	frustumCenter /= 8.0f;

	float radius = 0.0f;

	for (uint32_t i = 0; i < 8; i++) {
		DirectX::XMVECTOR distance = DirectX::XMVector4Length(frustumCorners[i] - frustumCenter);
		radius = std::max(radius, distance.m128_f32[0]);
	}
	radius = std::ceil(radius * 16.0f) / 16.0f;


	DirectX::XMVECTOR maxExtents = DirectX::XMVECTOR{ radius,radius,radius };
	DirectX::XMVECTOR minExtents = -maxExtents;

	DirectX::XMVECTOR lightDir = DirectX::XMVector4Normalize(-DirectX::XMVectorSet(Armageddon::Renderer::g_LightBufferData.DirectionalLights[0].Direction.x, Armageddon::Renderer::g_LightBufferData.DirectionalLights[0].Direction.y, Armageddon::Renderer::g_LightBufferData.DirectionalLights[0].Direction.z, 0.0f));
	
	m_LightViewMatrices[indice] = DirectX::XMMatrixLookAtLH(frustumCenter - lightDir * -minExtents.m128_f32[2], frustumCenter, DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	m_LightProjMatrices[indice] = DirectX::XMMatrixOrthographicLH(maxExtents.m128_f32[0] - minExtents.m128_f32[0], maxExtents.m128_f32[1]  - minExtents.m128_f32[1], 0.0f, maxExtents.m128_f32[2] - minExtents.m128_f32[2]);
	
	
}
