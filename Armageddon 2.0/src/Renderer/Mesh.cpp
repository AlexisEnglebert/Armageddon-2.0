#include "Mesh.h"
//#include "../Material/Material.h"


Mesh::Mesh(const std::filesystem::path& ModelPath)
{
	Assimp::Importer importer;
	const aiScene* model = importer.ReadFile(ModelPath.string().c_str(), aiProcess_Triangulate |
		aiProcess_GenNormals |
		aiProcess_GenUVCoords |
		aiProcess_OptimizeMeshes |
		aiProcess_ConvertToLeftHanded |
		aiProcess_ValidateDataStructure |
		aiProcess_JoinIdenticalVertices |
		aiProcess_CalcTangentSpace);

	if (model == nullptr)
	{
		Armageddon::Log::GetLogger()->error(importer.GetErrorString());
		return;
	}
	if (model->HasMaterials())
	{
		for (UINT i = 0; i < model->mNumMaterials; i++)
		{
			auto& mat = model->mMaterials[i];
			auto& property = mat->mProperties[0];
			aiString m_mname;
			mat->Get(AI_MATKEY_NAME, m_mname);
			Armageddon::Log::GetLogger()->info("Material {0}", (const char*)m_mname.C_Str());

			v_Materials.push_back(AssetManager::GetOrCreateMaterial(m_mname.C_Str()));
		}
	}
	

	ProcessNode(model->mRootNode, model);


}

void Mesh::ProcessNode(aiNode* node, const aiScene* scene)
{
	
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		//modelMeshes.push_back(ProcessMesh(mesh, scene));
		
		ProcessMesh(mesh, scene);

	}
	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

void Mesh::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> Vertices;
	std::vector<DWORD> Indices;
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		if (mesh->mTextureCoords[0])
		{
			if (mesh->HasNormals())
			{


				if (mesh->HasTangentsAndBitangents())
				{
					//Armageddon::Log::GetLogger()->trace("Mesh With all : {0}", i);

					Vertices.push_back(Vertex(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z,
						mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y,
						mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z,
						mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z,
						mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z));

				}
				else
				{
					//Armageddon::Log::GetLogger()->trace("Mesh With Normals : {0}", i);
				
					Vertices.push_back(Vertex(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z,
						mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y,
						mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));
				}
			}
			else
			{
			//	Armageddon::Log::GetLogger()->trace("Mesh With only position and TEXCOORD : {0}", i);
				Vertices.push_back(Vertex(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z,
					mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y));
			}
		}
		else
		{
		//	Armageddon::Log::GetLogger()->trace("Mesh With only position : {0}",i);
			Vertices.push_back(Vertex(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z));
		}


	}


	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		if (mesh->mFaces[i].mNumIndices != 3)
		{
			Armageddon::Log::GetLogger()->error("ERROR MESH HAS 3 INDICES !");
		}
		for (UINT j = 0; j < face.mNumIndices; j++)
		{
			Indices.push_back(face.mIndices[j]);
		}

	}
	ProcessBones(mesh, Vertices);

	if (mesh->mMaterialIndex >= 0) {
		v_SubMeshes.push_back(SubMesh(Vertices, Indices, mesh->mMaterialIndex));
	}
	else
	{
		v_SubMeshes.push_back(SubMesh(Vertices, Indices));

	}

}


void Mesh::ProcessBones(aiMesh* mesh, std::vector<Vertex>& Vertices)
{
		for (UINT j = 0; j< mesh->mNumBones; j++)
		{
			//mesh->mBones[i]->
			std::string BoneName = (const char*)mesh->mBones[j]->mName.C_Str();

			DirectX::XMMATRIX Blind = DirectX::XMMATRIX(
				mesh->mBones[j]->mOffsetMatrix.a1, mesh->mBones[j]->mOffsetMatrix.a2, mesh->mBones[j]->mOffsetMatrix.a3, mesh->mBones[j]->mOffsetMatrix.a4,
				mesh->mBones[j]->mOffsetMatrix.b1, mesh->mBones[j]->mOffsetMatrix.b2, mesh->mBones[j]->mOffsetMatrix.b3, mesh->mBones[j]->mOffsetMatrix.b4,
				mesh->mBones[j]->mOffsetMatrix.c1, mesh->mBones[j]->mOffsetMatrix.c2, mesh->mBones[j]->mOffsetMatrix.c3, mesh->mBones[j]->mOffsetMatrix.c4,
				mesh->mBones[j]->mOffsetMatrix.d1, mesh->mBones[j]->mOffsetMatrix.d2, mesh->mBones[j]->mOffsetMatrix.d3, mesh->mBones[j]->mOffsetMatrix.d4);
			
			Armageddon::Log::GetLogger()->trace("Bones name: {0}", BoneName.c_str());

			auto weightInfluence =  mesh->mBones[j]->mWeights;
			for (UINT k = 0; k < mesh->mBones[j]->mNumWeights; k++)
			{
				UINT VertId = weightInfluence[k].mVertexId;
				float weight = weightInfluence[k].mWeight;
				if(VertId < Vertices.size())
				{
					for (UINT l = 0; l < 200; l++)
					{
						Vertices[VertId].weights[l] = weight;
						Vertices[VertId].boneIDs[l] = m_skeleton.m_JointsCount;

					}
				}
				else
				{
					Armageddon::Log::GetLogger()->error("Vertex ID out of range !  : {0}", VertId);

				}
			}

			m_skeleton.m_aJoints.push_back(Joint(BoneName, 0, Blind));
			m_skeleton.m_JointsCount += 1;

			//m_skeleton
		}
	
}

void Mesh::ProcessSkeletonNode(aiNode* node, uint8_t ParentID)
{
	Armageddon::Log::GetLogger()->trace("Node name : {0}", node->mName.C_Str());
	Armageddon::Log::GetLogger()->trace("Parent ID : {0}", ParentID);
	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		ProcessSkeletonNode(node->mChildren[i], ++ParentID);
	}
}

Mesh::Mesh(std::vector<Vertex> Vertices, std::vector<DWORD> Indices)
{

	v_SubMeshes.push_back(SubMesh(Vertices, Indices,0));
	v_Materials.push_back(AssetManager::GetOrCreateMaterial("DefaultMaterial"));

}

 VertexBuffer<Vertex>* Mesh::GetVertexBuffer()
{
	return &m_VertexBuffer;
}

 IndexBuffer<DWORD>* Mesh::GetIndexBuffer()
{
	return &m_IndexBuffer;
}



 bool Mesh::IsEmpty()
 {
	 if (m_VertexBuffer.BufferSize() == 0 && v_SubMeshes.size() == 0)
	 {
		 return true;
	 }
	 return false;
 }

 void Mesh::UpdtateTransform(Armageddon::Camera* m_camera)
 {
	 m_TransForm.WorldMat = DirectX::XMMatrixIdentity();
	 m_TransForm.ProjectionMat = m_camera->GetProjectionMatrix();
	 m_TransForm.ViewMat = m_camera->GetViewMatrix();
	 m_TransForm.MVP =   DirectX::XMMatrixMultiply(m_camera->GetViewMatrix() , m_camera->GetProjectionMatrix());
 }

 void Mesh::BindMaterial(int MatIndex)
 {
	 v_Materials[MatIndex].Bind();
 }



