#include "Mesh.h"

#include <string>
#include <glad/glad.h>
#include <iostream>
namespace Firefly
{
	Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, const std::vector<std::shared_ptr<Texture>>& textures)
		:m_VAO(), 
		m_VBO(&vertices[0], sizeof(Vertex)* static_cast<unsigned int>(vertices.size())),
		m_IBO(&indices[0], sizeof(unsigned int) * static_cast<unsigned int>(indices.size())),
		m_Vertices(vertices), m_Indices(indices), m_Textures(textures)
	{
		m_VAO.SetLayout({
			{ VertexArray::Type::Float, 3 },
			{ VertexArray::Type::Float, 3 },
			{ VertexArray::Type::Float, 2 },
			{ VertexArray::Type::Float, 3 },
			{ VertexArray::Type::Float, 3 },
			{ VertexArray::Type::Int, 4 },
			{ VertexArray::Type::Float, 4 }
			});

		VertexArray::UnBind();

	}

	void Mesh::Render(Shader& shader)
	{
		shader.Bind();
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;
		int n = static_cast<int>(m_Textures.size());
		for (int i = 0; i < n; ++i)
		{
			m_Textures[i]->Bind(i);
			std::string name;
			Texture::Type type = m_Textures[i]->GetType();
			switch (type)
			{
			case Texture::Type::Diffuse:  name = "model.texture_diffuse" + std::to_string(diffuseNr++); break;
			case Texture::Type::Specular: name = "model.texture_specular" + std::to_string(specularNr++); break;
			case Texture::Type::Normal: name = "model.texture_normal" + std::to_string(normalNr++); break;
			case Texture::Type::Ambient: name = "model.texture_height" + std::to_string(heightNr++); break;
			}
			glUniform1i(shader.GetLocation(("material." + name).c_str()), i);
		}
		m_VAO.Bind();
		glDrawElements(GL_TRIANGLES, m_IBO.GetCount(), GL_UNSIGNED_INT, 0);

		VertexArray::UnBind();
		Texture::Active(0);
	}
}