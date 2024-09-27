#include "Model.h"
#include <iostream>

#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

namespace Firefly
{
	Model::Model(const std::string& path)
		:m_Directory(path.substr(0, path.find_last_of('/')))
	{
		LoadModel(path);
		m_TextureCache.clear();
	}

	void Model::Render(Shader& shader)
	{
		static unsigned int n = static_cast<unsigned int>(m_Meshes.size());
		for (unsigned int i = 0; i < n; ++i)
		{
			m_Meshes[i].Render(shader);
		}
	}

	namespace
	{
		unsigned int config =
			aiProcess_Triangulate |
			aiProcess_FlipUVs |
			aiProcess_GenSmoothNormals |
			aiProcess_CalcTangentSpace;
	}
	
	void Model::LoadModel(const std::string& path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, config);

		if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr)
		{
			std::cout << "Failed to load the model: " << path << "\n" << "ERROR::ASSIMP: " << importer.GetErrorString() << "\n";
			return;
		}
		m_Meshes.reserve(scene->mNumMeshes);
		ProcessNode(scene->mRootNode, scene);
	}
	
	void Model::ProcessNode(aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; ++i)
		{
			ProcessMesh(scene->mMeshes[node->mMeshes[i]], scene);
		}
		for (unsigned int i = 0; i < node->mNumChildren; ++i)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}
	
	void Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Mesh::Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<std::shared_ptr<Texture>> textures;
		
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Mesh::Vertex vertex;
			// positions
			vertex.position.x = mesh->mVertices[i].x;
			vertex.position.y = mesh->mVertices[i].y;
			vertex.position.z = mesh->mVertices[i].z;
			// normals
			if (mesh->HasNormals())
			{
				vertex.normal.x = mesh->mNormals[i].x;
				vertex.normal.y = mesh->mNormals[i].y;
				vertex.normal.z = mesh->mNormals[i].z;
			}
			// texture coordinates
			if (mesh->mTextureCoords[0]) 
			{
				vertex.texCoord.x = mesh->mTextureCoords[0][i].x;
				vertex.texCoord.y = mesh->mTextureCoords[0][i].y;
				// tangent
				vertex.tangent.x = mesh->mTangents[i].x;
				vertex.tangent.y = mesh->mTangents[i].y;
				vertex.tangent.z = mesh->mTangents[i].z;
				// bitangent
				vertex.bitangent.x = mesh->mBitangents[i].x;
				vertex.bitangent.y = mesh->mBitangents[i].y;
				vertex.bitangent.z = mesh->mBitangents[i].z;
			}
			else
				vertex.texCoord = glm::vec2(0.0f, 0.0f);

			vertices.push_back(vertex);
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; ++j)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		loadMaterialTextures(material, Firefly::Texture::Type::Diffuse, textures);
		loadMaterialTextures(material, Firefly::Texture::Type::Specular, textures);
		loadMaterialTextures(material, Firefly::Texture::Type::Normal, textures);
		loadMaterialTextures(material, Firefly::Texture::Type::Ambient, textures);

		m_Meshes.emplace_back(vertices, indices, textures);
	}
	
	void Model::loadMaterialTextures(aiMaterial* material, Texture::Type type, std::vector<std::shared_ptr<Texture>>& destination)
	{
		aiTextureType aiType;
		switch (type)
		{
		case Texture::Type::Ambient:	aiType = aiTextureType_AMBIENT; break;
		case Texture::Type::Specular:	aiType = aiTextureType_SPECULAR; break;
		case Texture::Type::Diffuse:	aiType = aiTextureType_DIFFUSE; break;
		default: return;
		}
		unsigned int n = material->GetTextureCount(aiType);
		for (unsigned int i = 0; i < n; i++)
		{
			aiString str;
			material->GetTexture(aiType, i, &str);
			// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
			auto it = m_TextureCache.find(std::string(str.C_Str()));
			if (it != m_TextureCache.end())
			{
				destination.push_back(it->second);
			}
			else
			{
				std::shared_ptr<Texture> texture = std::make_shared<Texture2D>(m_Directory + "/" + str.C_Str(), type);
				destination.push_back(texture);
				m_TextureCache[std::string(str.C_Str())] = texture;  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
			}
		}

	}
}