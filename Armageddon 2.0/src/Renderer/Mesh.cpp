#include "Mesh.h"
//#include "../Material/Material.h"


Mesh::Mesh(const std::filesystem::path& ModelPath)
{
	Assimp::Importer importer;
	importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
	const aiScene* model = importer.ReadFile(ModelPath.string().c_str(), aiProcess_Triangulate |
		aiProcess_GenNormals |
		aiProcess_GenUVCoords |
		aiProcess_OptimizeMeshes |
		aiProcess_ConvertToLeftHanded |
		aiProcess_ValidateDataStructure |
		aiProcess_JoinIdenticalVertices |
		aiProcess_CalcTangentSpace |
		aiProcess_PopulateArmatureData);

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
	if(mesh->HasBones())
		ProcessBones(mesh, Vertices,scene);

	if (mesh->mMaterialIndex >= 0) {
		v_SubMeshes.push_back(SubMesh(Vertices, Indices, mesh->mMaterialIndex));
	}
	else
	{
		v_SubMeshes.push_back(SubMesh(Vertices, Indices));

	}

}


void Mesh::ProcessBones(aiMesh* mesh, std::vector<Vertex>& Vertices,const aiScene* scene)
{
		auto& InverseTransformation = scene->mRootNode->mTransformation;
		InverseTransformation.Inverse();
		ProcesBoneNode(mesh->mBones[0]->mNode,0);
		for (UINT j = 0; j< mesh->mNumBones; j++)
		{
			//mesh->mBones[i]->
	

			std::string BoneName = (const char*)mesh->mBones[j]->mName.C_Str();

			Armageddon::Log::GetLogger()->trace("Bones name: {0}", BoneName.c_str());


			const aiMatrix4x4& offset = mesh->mBones[j]->mOffsetMatrix;
			XMMATRIX meshToBoneTransform = XMMatrixTranspose(
				XMMATRIX(offset.a1, offset.a2, offset.a3, offset.a4,
					offset.b1, offset.b2, offset.b3, offset.b4,
					offset.c1, offset.c2, offset.c3, offset.c4,
					offset.d1, offset.d2, offset.d3, offset.d4));
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
			
			

			//m_skeleton
		}
	
}

void Mesh::ProcesBoneNode(aiNode* node, uint8_t ParentID)
{
	uint8_t id = ParentID + 1 ;
	Armageddon::Log::GetLogger()->trace("Bone name : {0}", node->mName.C_Str());
	Armageddon::Log::GetLogger()->trace("Parent ID : {0}", id-1);
	


	const aiMatrix4x4& Transformation = node->mTransformation;
	XMMATRIX meshToBoneTransform = XMMatrixTranspose(
		XMMATRIX(Transformation.a1, Transformation.a2, Transformation.a3, Transformation.a4,
			Transformation.b1, Transformation.b2, Transformation.b3, Transformation.b4,
			Transformation.c1, Transformation.c2, Transformation.c3, Transformation.c4,
			Transformation.d1, Transformation.d2, Transformation.d3, Transformation.d4));


	m_skeleton.m_aJoints.push_back(Joint(std::string(node->mName.C_Str()),(uint8_t)(id - 1) ,meshToBoneTransform));
	m_skeleton.m_JointsCount += 1;
	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		ProcesBoneNode(node->mChildren[i], id);
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



