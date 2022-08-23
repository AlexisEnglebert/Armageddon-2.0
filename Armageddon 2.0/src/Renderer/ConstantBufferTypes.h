#pragma once
#if __linux__
//pass pour l'instant
#else
#include <DirectXMath.h>
#endif
#include "Lights.h"
#include "../Macros.h"
/*attention tout doit être alligné sur 16 bytes donc*/

/*
Buffer Registry: 
			
			1- Transform
			2- Light
			3- SkyBox

*/
struct DECL TransFormBuffer
{
	DirectX::XMMATRIX WorldMat = DirectX::XMMatrixIdentity();

};

struct CameraBuffer_t
{
	DirectX::XMMATRIX ProjectionMat;
	DirectX::XMMATRIX ViewMat;
	DirectX::XMMATRIX MVP;
	DirectX::XMMATRIX InverseProjectionMat;
	DirectX::XMMATRIX InverseViewMat;
	DirectX::XMMATRIX InverseMVP;

	DirectX::XMFLOAT3 CameraPos; //12 
	float nearPlane;
	float farPlane;
};

struct DECL LightBuffer
{
	int PointLightCount = 0; //4
	int DirectionalLightCount = 0; //4
	DirectX::XMFLOAT2 padding1; //8 

	PointLight PointLights[50];
	DirectionalLight DirectionalLights[50];

	DirectX::XMMATRIX LightViewProjectionCascade[3];
	DirectX::XMFLOAT3 FarPlane = {0.0f,0.0f,0.0f};
	int CascadeIndice = 0 ;

};

struct DECL SkyBoxBuffer
{
	float TextureLOD;
	float Intensity;
	DirectX::XMFLOAT2 padding0; //8 ;
};



struct DECL MaterialBuffer
{
	float Roughness;//4
	float Metalic;//8
	bool UseMetalMap;//9
	float _Padding0;//13
	bool UseEmisive;//14
	DirectX::XMFLOAT3 AlbedoTint = {1.0f,1.0f,1.0f}; //26
	float EmisiveFactor;//4
	DirectX::XMFLOAT3 EmisiveTint;//12
};

struct DECL WorldBuffer
{
	float Time;

};