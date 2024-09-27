#pragma once
#include <glm.hpp>
#include <vector>
#include <array>
#include <iostream>
#include <memory>

#include "1.6_Texture/Texture.h"
#include "1.4_Hello_Triangle/Shader.h"
#include "1.4_Hello_Triangle/VertexArray.h"
#include "1.4_Hello_Triangle/IndexBuffer.h"
#include "1.4_Hello_Triangle/VertexBuffer.h"

namespace Firefly
{
	class Mesh
	{
	public:
#define MAX_BONE_INFLUENCE 4
        struct Vertex {
            glm::vec3 position;
            glm::vec3 normal;
            glm::vec2 texCoord;
            glm::vec3 tangent;
            glm::vec3 bitangent;
            std::array<int, MAX_BONE_INFLUENCE> boneIDs;    
            std::array<float, MAX_BONE_INFLUENCE> weights;  

			Vertex() = default;
        };
		Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, const std::vector<std::shared_ptr<Texture>>& textures);
		void Render(Shader& shader);
	private:
		VertexArray m_VAO;
		VertexBuffer m_VBO;
		IndexBuffer m_IBO;
		std::vector<Vertex> m_Vertices;
		std::vector<unsigned int> m_Indices;
		std::vector<std::shared_ptr<Texture>> m_Textures;
	};

}