#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

#include "1.4_Hello_Triangle/Shader.h"
#include "3.1_Mesh/Mesh.h"

struct aiScene;
struct aiNode;
struct aiMesh;
struct aiMaterial;

namespace Firefly
{
	class Model
	{
	public:
		Model(const std::string& path);
		void Render(Shader& shader);
		void RenderInstance(Shader& shader, int count);
		void AppendLayout(std::vector<VertexArray::Layout> const& layouts);
	private:
		std::vector<Mesh> m_Meshes;
		std::unordered_map<std::string, std::shared_ptr<Firefly::Texture>> m_TextureCache;
		const std::string m_Directory;
	private:
		void LoadModel(const std::string& path);
		void ProcessNode(aiNode* node, const aiScene* scene);
		void ProcessMesh(aiMesh* mesh, const aiScene* scene);
		void loadMaterialTextures(aiMaterial* material, Texture::Type type, std::vector<std::shared_ptr<Texture>>& destination);
	};
}