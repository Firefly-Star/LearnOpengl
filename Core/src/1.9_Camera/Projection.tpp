namespace Firefly
{
	template<Projection::Type type, typename T>
	std::shared_ptr<Projection> Projection::Create(T params)
	{
		if constexpr (type == Projection::Type::Ortho && std::is_same_v<T, std::array<float, 6>>)
		{
			return std::make_shared<OrthoProjection>(params);
		}
		else if constexpr (type == Projection::Type::Perspective && std::is_same_v<T, std::array<float, 4>>)
		{
			return std::make_shared<PerspectiveProjection>(params);
		}
		else
		{
			return nullptr;
		}
	}
}