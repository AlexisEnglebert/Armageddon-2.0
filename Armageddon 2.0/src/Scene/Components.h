#pragma once
#include <string>
#include <DirectXMath.h>

#include "../Renderer/Mesh.h"
#include "../Application.h"
#include "../Renderer/Camera/Camera.h"
#include "../Renderer/Interface.h"
struct TagComponent
{
	std::string Tag;
	TagComponent() = default;
	TagComponent(std::string tag) : Tag(tag) {};

};


		/*POUR L'INSTANT ON LAISSE CA
		MAIS ON DEVRAIT AVOIR UN TRANSFORM COMPONENT
		AINSI QU'UNE CAMERA COMPONENT POUR PRETRE D'UPDATE DIRRECTEMENT DANS LA
		SCENE ET NON DANS LE COMPONENT*/

struct MeshComponent
{
	int m_type = 0; //0 = primitives , 1 = model; 
	bool ShowComponent = true;
	Mesh m_mesh;
	std::string m_path = "";
	MeshComponent() = default;
	MeshComponent(Mesh mesh) : m_mesh(mesh)
	{
		Armageddon::Application::GetApplicationInsatnce()->GetWindow()->GetRenderer().SubmitMesh(mesh);

	};
	MeshComponent(Mesh mesh, std::string path) : m_mesh(mesh), m_path(path)
	{
		Armageddon::Application::GetApplicationInsatnce()->GetWindow()->GetRenderer().SubmitMesh(mesh);

	};
	MeshComponent(std::string path) : m_path(path)
	{
		
		Mesh model = Mesh(path);
		m_mesh = model;
		Armageddon::Application::GetApplicationInsatnce()->GetWindow()->GetRenderer().SubmitMesh(model);
		
	};
};


struct TransformComponent
{
	DirectX::XMFLOAT3 Translation = { 0.0f,0.0f, 0.0f };
	DirectX::XMFLOAT3 Rotation = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 Scale = { 1.0f, 1.0f, 1.0f };
	TransformComponent() = default;
	TransformComponent(DirectX::XMFLOAT3& Transform, DirectX::XMFLOAT3& Rotation, DirectX::XMFLOAT3& Scale) : Translation(Transform), Rotation(Rotation), Scale(Scale)
	{};
	/*TransformComponent(TransformComponent& Transform)
	{
		Translation = Transform.Translation;
		Rotation = Transform.Rotation;
		Scale = Transform.Scale;
	};*/
	TransformComponent(float x, float y, float z, float rx , float ry , float rz , float sx, float sy, float sz)
	{
		Translation = { x,y,z };
		Rotation = { rx,ry,rz };
		Scale = { sx,sy,sz };
	}
	DirectX::XMMATRIX GetTransformMatrix()
	{
		/*1 arg  = scale origin 
		  2 arg = Scale orientation QUATERNION
		  3 arg = vector de scale
		  4 arg = rot origin
		  5 arg = rot QUATERNION
		  6 arg translation vector
		  */
		return DirectX::XMMatrixTransformation(DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f)
			, DirectX::XMVectorSet(Scale.x, Scale.y, Scale.z, 1.0f), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), DirectX::XMQuaternionRotationRollPitchYaw(Rotation.x, Rotation.y, Rotation.z)
			, DirectX::XMVectorSet(Translation.x, Translation.y, Translation.z, 1.0f));
	};

};

struct LightComponent
{
	PointLight m_pointLight;
	DirectionalLight m_directionalLight;
	/*
	Type:
	- 0 = PointLight
	- 1 = DirectionalLight
	*/	
	int type = 0;
	LightComponent() = default;
};

struct SkyComponent
{
	SkyComponent() = default;
};

struct MaterialComponent
{
	//Armageddon::Material m_material = Armageddon::Renderer::g_assetManager.v_material[0];
	Armageddon::Material m_material;
	MaterialComponent() = default;
};