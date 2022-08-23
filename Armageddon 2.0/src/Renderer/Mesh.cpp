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
	Armageddon::Log::GetLogger()->trace("LOADING MESH {0}", ModelPath);
	if (model->HasMaterials())
	{
		for (UINT i = 0; i < model->mNumMaterials; i++)
		{
			auto& mat = model->mMaterials[i];
			auto& property = mat->mProperties[0];
			aiString m_mname;
			mat->Get(AI_MATKEY_NAME, m_mname);
			Armageddon::Log::GetLogger()->info("Material {0}", (const char*)m_mname.C_Str());
			if (m_mname.length <= 0)
			{
				m_mname = "Unamed Material #" + std::to_string(i);
			}
			v_MaterialReference.push_back(AssetManager::GetOrCreateMaterial(m_mname.C_Str()));
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
		std::shared_ptr<Joint> p = nullptr;
		Armageddon::Log::GetLogger()->trace("Bones debug : {0}", mesh->mNumBones);
		ProcesBoneNode(mesh->mBones[0]->mNode,0,p);
		//Now we can bind the InverseBlindPose to each joint
		for (size_t i = 0; i < mesh->mNumBones; i++)
		{
			DirectX::XMMATRIX mat;
			mat.r[0].m128_f32[0] = mesh->mBones[i]->mOffsetMatrix.a1;
			mat.r[0].m128_f32[1] = mesh->mBones[i]->mOffsetMatrix.b1;
			mat.r[0].m128_f32[2] = mesh->mBones[i]->mOffsetMatrix.c1;
			mat.r[0].m128_f32[3] = mesh->mBones[i]->mOffsetMatrix.d1;

			mat.r[1].m128_f32[0] = mesh->mBones[i]->mOffsetMatrix.a2;
			mat.r[1].m128_f32[1] = mesh->mBones[i]->mOffsetMatrix.b2;
			mat.r[1].m128_f32[2] = mesh->mBones[i]->mOffsetMatrix.c2;
			mat.r[1].m128_f32[3] = mesh->mBones[i]->mOffsetMatrix.d2;


			mat.r[2].m128_f32[0] = mesh->mBones[i]->mOffsetMatrix.a3;
			mat.r[2].m128_f32[1] = mesh->mBones[i]->mOffsetMatrix.b3;
			mat.r[2].m128_f32[2] = mesh->mBones[i]->mOffsetMatrix.c3;
			mat.r[2].m128_f32[3] = mesh->mBones[i]->mOffsetMatrix.d3;

			mat.r[3].m128_f32[0] = mesh->mBones[i]->mOffsetMatrix.a4;
			mat.r[3].m128_f32[1] = mesh->mBones[i]->mOffsetMatrix.b4;
			mat.r[3].m128_f32[2] = mesh->mBones[i]->mOffsetMatrix.c4;
			mat.r[3].m128_f32[3] = mesh->mBones[i]->mOffsetMatrix.d4;

			m_skeleton.m_Bonemap[mesh->mBones[i]->mName.C_Str()]->m_inverseBlindPose = mat ;
		}
}

void Mesh::ProcesBoneNode(aiNode* node, uint8_t ParentID, std::shared_ptr<Joint>& parentJoint)
{
	uint8_t id = ParentID+1;
	std::shared_ptr<Joint> currentJoint;
	if (ParentID == 0) //TODO C'EST MOCHE ON PEUT MIEUX FAIRE QUAND MEME...
	{
		//ROOT
		Armageddon::Log::GetLogger()->trace("Root bone: {0} ",node->mName.C_Str());

		JoinPose pos;
		aiVector3D translation;
		aiVector3D scale;
		aiVector3D rotation;
		
		node->mTransformation.Decompose(scale, rotation, translation);

		pos.m_translation = { translation.x,translation.y,translation.z };
		pos.m_rotation = { rotation.x,rotation.y,rotation.z };
		pos.m_scale = scale.x;

		Armageddon::Log::GetLogger()->trace("bone translation: {0} {1} {2} ", translation.x , translation.y , translation.z);
		Armageddon::Log::GetLogger()->trace("bone scale: {0} {1} {2} ", scale.x, scale.y, scale.z);
		Armageddon::Log::GetLogger()->trace("bone rotation: {0} {1} {2} ", rotation.x, rotation.y, rotation.z);


		currentJoint = std::make_shared<Joint>();
		currentJoint->m_name = node->mName.C_Str();
		currentJoint->m_Parentid = 0;
		currentJoint->m_position = pos;

		m_skeleton.m_Bonemap[node->mName.C_Str()] = currentJoint;
		//Set the inverse blind pos:
		m_skeleton.m_rootJoint = currentJoint;
	}
	else
	{
	


		Armageddon::Log::GetLogger()->trace("Bone name : {0}", node->mName.C_Str());
		
		JoinPose pos;

		aiVector3D translation;
		aiVector3D scale;
		aiVector3D rotation;

		node->mTransformation.Decompose(scale, rotation, translation);


		pos.m_translation = { translation.x,translation.y,translation.z };
		pos.m_rotation = { rotation.x,rotation.y,rotation.z };
		pos.m_scale = scale.x;


		Armageddon::Log::GetLogger()->trace("bone translation: {0} {1} {2} ", translation.x, translation.y, translation.z);
		Armageddon::Log::GetLogger()->trace("bone scale: {0} {1} {2} ", scale.x, scale.y, scale.z);
		Armageddon::Log::GetLogger()->trace("bone rotation: {0} {1} {2} ", rotation.x, rotation.y, rotation.z);

		currentJoint = std::make_shared<Joint>();
		currentJoint->m_name = node->mName.C_Str();
		currentJoint->m_Parentid = ParentID - 1;
		currentJoint->m_position = pos;
		parentJoint->m_child.push_back(currentJoint);
		m_skeleton.m_Bonemap[node->mName.C_Str()] = currentJoint;
	}
	
	m_skeleton.m_JointsCount += 1;



	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		ProcesBoneNode(node->mChildren[i], id, currentJoint);
	}
}

Mesh::Mesh(std::vector<Vertex> Vertices, std::vector<DWORD> Indices)
{

	v_SubMeshes.push_back(SubMesh(Vertices, Indices,0));
	v_MaterialReference.push_back(AssetManager::GetOrCreateMaterial("DefaultMaterial"));

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


 void Mesh::BindRessource(int MatIndex)
 {
	 AssetManager::m_MaterialMap[v_MaterialReference[MatIndex]].BindRessources();

 }

 void Mesh::BindShaders(int MaterialIndex)
 {
	 AssetManager::m_MaterialMap[v_MaterialReference[MaterialIndex]].BindShaders();

 }

 void Mesh::BindMaterial(int MatIndex)
 {
	 BindRessource(MatIndex);
	 BindShaders(MatIndex);
 }



