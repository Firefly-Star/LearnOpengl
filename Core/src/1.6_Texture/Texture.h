#pragma once
#include <string>
#include <vector>
#include "Macros.h"

namespace Firefly
{
	class Texture
	{
		MOVE_ONLY(Texture);
	public:
		enum class Type
		{
			None = 0,
			Cube,
			Ambient, Diffuse, Specular,
			Normal
		};
	public:
		Texture(const std::string& path = "", Type type = Type::None) : m_RendererId(0), m_Type(type), m_Path(path) {}
		static void Active(int slot = 0);
		virtual ~Texture() {}
		Type GetType() const { return m_Type; }
		const std::string& GetPath() { return m_Path; }
		virtual void Bind(unsigned int slot = 0) = 0;
	protected:
		unsigned int m_RendererId;
		Type m_Type;
		std::string m_Path;
	};

	class Texture2D : public Texture
	{
		MOVE_ONLY(Texture2D);
	public:
		Texture2D(const std::string& fileName, Type type = Type::None);
		Texture2D(unsigned char* data, int width, int height);
		virtual ~Texture2D();
		virtual void Bind(unsigned int slot = 0) override;
		
	private:
		void LoadTexture(const std::string& fileName);
	};

	class TextureCube : public Texture
	{
		MOVE_ONLY(TextureCube);
	public:
		TextureCube(const std::vector<std::string>& fileNames);
		virtual ~TextureCube();
		virtual void Bind(unsigned int slot = 0) override;
	private:
		void LoadTexture(const std::vector<std::string>& fileNames);
	};
}