#pragma once
#include <DirectXMath.h>
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
	DirectX::XMMATRIX WorldMat;
	DirectX::XMMATRIX ProjectionMat;
	DirectX::XMMATRIX ViewMat;
	DirectX::XMMATRIX MVP;
};

struct DECL LightBuffer
{
	DirectX::XMFLOAT3 CameraPos; //12 
	float Padding0; // 4 
	int PointLightCount = 0; //4
	int DirectionalLightCount = 0; //4
	DirectX::XMFLOAT2 padding1; //8 

	PointLight PointLights[50];
	DirectionalLight DirectionalLights[50];

	DirectX::XMMATRIX LightViewProjection;
};

struct DECL SkyBoxBuffer
{
	float TextureLOD;
	float Intensity;
	DirectX::XMFLOAT2 padding0; //8 ;
};

struct DECL RoughnessBuffer
{
	float Rougness;
};


struct DECL MaterialBuffer
{
	float Roughness			= 0.1; //4		.4
	float Metalic		= 0.04;  //4		.8
	bool UseMetalMap	= false; //2		.10
	float _Padding0;		
	bool UseEmisive = false; //2		.12
	DirectX::XMFLOAT3 AlbedoTint = {1.0f,1.0f,1.0f};	//12
	float _Padding1;
	float EmisiveFactor;
	DirectX::XMFLOAT3 EmisiveTint;
};