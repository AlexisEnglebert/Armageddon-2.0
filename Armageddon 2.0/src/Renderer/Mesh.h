#pragma once
#include "../Log.h"
#include "Shaders.h"
#include "ConstantBufferTypes.h"
#include "Camera/Camera.h"
#include "SubMesh.h"

#include <vector>
#include <stdlib.h>
#include <wrl/client.h>
#include<string>
#include<filesystem>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp/postprocess.h>
#include "../Material/AssetManager.h"
#include "Animation/Joint.h"
/*Cette class gère les models ainsi que le les vertex de bases */
class DECL Mesh
{
public:

	Mesh() = default;
	Mesh(const std::filesystem::path& ModelPath);
	Mesh(std::vector<Vertex> Vertices, std::vector<DWORD> Indices);

	 VertexBuffer<Vertex>* GetVertexBuffer();
	 IndexBuffer<DWORD>*   GetIndexBuffer();

	 bool IsEmpty();
	 void UpdtateTransform(Armageddon::Camera* m_camera);
	 TransFormBuffer* GetTransform() { return &m_TransForm; };
	 
	 std::vector<SubMesh> v_SubMeshes;
	 std::vector <Armageddon::Material > v_Materials;
	 Skeleton m_skeleton;

	 void BindMaterial(int MatIndex);

private:

	void ProcessNode(aiNode* node, const aiScene* scene);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene);
	void ProcessBones(aiMesh* mesh, std::vector<Vertex>& Vertices);
	void ProcessSkeletonNode(aiNode* node,uint8_t ParentID);

	TransFormBuffer m_TransForm;
	Armageddon::Camera* m_camera;

	VertexBuffer<Vertex> m_VertexBuffer;
	IndexBuffer<DWORD>   m_IndexBuffer;


};