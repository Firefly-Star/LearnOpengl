#pragma once

#include <variant>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
namespace Firefly
{
	struct SetResult_t {};
	struct VectorResult_t {};
	inline constexpr SetResult_t SetResult = SetResult_t{};
	inline constexpr VectorResult_t VectorResult = VectorResult_t{};

	class GrammarNormalizer;
	class Grammar
	{
		friend class GrammarNormalizer;
	public:
		using left_type = std::string;
		using member_type = std::variant<std::string, char>;
		using right_type = std::vector<member_type>;
		using set_type = std::unordered_set<member_type>;
	public:
		void SetStartSymbol(left_type const& startSymbol)
		{
			m_StartSymbol = startSymbol;
		}

		void AddProduction(left_type const& left, right_type const& right)
		{
			if (m_Productions.find(left) != m_Productions.end())
			{
				for (right_type const& _right : m_Productions[left])
				{
					if (_right == right)
					{
						return;
					}
				}
			}
			m_Productions[left].push_back(right);
		}

		void PrintProductions() const
		{
			for (auto& pair : m_Productions)
			{
				for (auto& right : pair.second)
				{
					std::cout << pair.first << " -> ";
					if (right.size() == 0)
					{
						std::cout << "epsilon ";
					}
					else
					{
						for (auto& v : right)
						{
							std::visit([](auto& value) {
									std::cout << value << " ";
								}, v);
						}
					}
					std::cout << "\n";
				}
			}
		}

		[[nodiscard]] std::unordered_set<left_type> GetNonterminalSymbol(SetResult_t)
		{
			std::unordered_set<left_type> set;
			for (auto& [key, value] : m_Productions)
			{
				set.insert(key);
			}
			return set;
		}
		[[nodiscard]] std::vector<left_type> GetNonterminalSymbol(VectorResult_t)
		{
			std::vector<left_type> vector;
			for (auto& [key, value] : m_Productions)
			{
				vector.push_back(key);
			}
			return vector;
		}
		[[nodiscard]] auto GetProductions() const -> std::unordered_map<left_type, std::vector<right_type>> const&
		{
			return m_Productions;
		}
		[[nodiscard]] left_type const& GetStartSymbol() const { return m_StartSymbol; }
	private:
		left_type m_StartSymbol;
		std::unordered_map<left_type, std::vector<right_type>> m_Productions;
	};
}