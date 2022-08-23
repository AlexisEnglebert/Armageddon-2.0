#pragma once
#include "../../Macros.h"
#include "../Camera/Camera.h"
#include "../Interface.h"
#include "../Shaders.h"
#include "../../Material/Texture.h"

#define NUM_CASCADE 3
#define SHADOW_MAP_WIDTH 4096.0f
#define SHADOW_MAP_HEIGHT 2160.0f

namespace Armageddon
{

	class DECL CascadeShadow
	{
	public:
		CascadeShadow() {};
		void CalculateMatrices();
		bool Init(Camera* m_camera);
		void ComputeLightMatrix(float nearPlane, float farPlane, DirectX::XMMATRIX InverseViewProjection, int indice);
		DirectX::XMMATRIX LightView = DirectX::XMMatrixIdentity();
		DirectX::XMMATRIX LightProjection = DirectX::XMMatrixIdentity();
		Armageddon::PixelShaders  px;
		Armageddon::VertexShaders vx;
		DirectX::XMMATRIX m_LightViewMatrices[NUM_CASCADE];
		DirectX::XMMATRIX m_LightProjMatrices[NUM_CASCADE];
		DepthTexture m_CascadeShadowMap[NUM_CASCADE];
		float m_FrustumSlices[NUM_CASCADE];
		float m_SplitDepth[NUM_CASCADE];
	private:
		Camera* m_pCamera;

		float lambda = 0.95f;//small correction to avoid sharply change between frustum partition
		
	};
}