#pragma once
#include <string>

namespace Firefly
{
	class Texture
	{
	public:
		enum class Type
		{
			None = 0,
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
	public:
		Texture2D(const std::string& fileName, Type type = Type::None);
		virtual ~Texture2D();
		virtual void Bind(unsigned int slot = 0) override;

	private:
		void LoadTexture(const std::string& fileName);
	};
}