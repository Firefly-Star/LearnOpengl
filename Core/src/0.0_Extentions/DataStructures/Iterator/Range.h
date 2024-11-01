#pragma once
#include <type_traits>
namespace Firefly
{
	namespace Range
	{
		namespace Cpo
		{
			template<typename T>
			inline constexpr bool is_Array_v = false;

			template<typename T, size_t N>
			inline constexpr bool is_Array_v<T[N]> = true;

			namespace Begin
			{
				template<typename T>
				concept Has_Member = requires(T obj) { obj.begin(); };

				template<typename T>
				concept Has_ADL = requires(T obj) { begin(obj); };

				class Cpo
				{
				public:
					enum class Strategy
					{
						None = 0,
						Val, Member, ADL
					};
				public:
					template<typename T>
					consteval static Strategy Choose()
					{
						if constexpr (is_Array_v<T>)
						{
							return Strategy::Val;
						}
						else if constexpr (Has_Member<T>)
						{
							return Strategy::Member;
						}
						else if constexpr (Has_ADL<T>)
						{
							return Strategy::ADL;
						}
						else
						{
							static_assert(false, "Cannot accept an invalid type!");
						}
					}
				private:
					template<typename T>
					static constexpr Strategy s_Strategy = Choose<T>();
				private:
					template<typename T>
					constexpr auto operator()(T&& value)
					{
						switch (s_Strategy<T>)
						{
						case Strategy::Val:
						{
							return value;
						}
						case Strategy::Member:
						{
							return value.begin();
						}
						case Strategy::ADL:
						{
							return begin(value);
						}
 						}
					}
				};
			}
			namespace End
			{
				template<typename T>
				concept Has_Member = requires(T obj) { obj.end(); };

				template<typename T>
				concept Has_ADL = requires(T obj) { end(obj); };

				class Cpo
				{
				public:
					enum class Strategy
					{
						None = 0,
						Val, Member, ADL
					};
				public:
					template<typename T>
					consteval static Strategy Choose()
					{
						if constexpr (is_Array_v<T>)
						{
							return Strategy::Val;
						}
						else if constexpr (Has_Member<T>)
						{
							return Strategy::Member;
						}
						else if constexpr (Has_ADL<T>)
						{
							return Strategy::ADL;
						}
						else
						{
							static_assert(false, "Cannot accept an invalid type!");
						}
					}
				private:
					template<typename T>
					static constexpr Strategy s_Strategy = Choose<T>();
				private:
					template<typename T>
					constexpr auto operator()(T&& value)
					{
						switch (s_Strategy<T>)
						{
						case Strategy::Val:
						{
							return value;
						}
						case Strategy::Member:
						{
							return value.end();
						}
						case Strategy::ADL:
						{
							return end(value);
						}
						}
					}
				};
			}

			inline constexpr Begin::Cpo begin;
			inline constexpr End::Cpo end;

			template<typename T>
			using iterator_t = decltype(begin(std::declval<T&>()));
		}
	}
}