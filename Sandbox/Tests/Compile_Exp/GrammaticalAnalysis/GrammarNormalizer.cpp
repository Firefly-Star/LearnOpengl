#include "GrammarNormalizer.h"
#include <algorithm>

namespace Firefly
{
	void GrammarNormalizer::Normalize()
	{
		EliminateLeftRecursion();
		ExtractLeftFactoring();
	}

	void GrammarNormalizer::EliminateLeftRecursion()
	{
		bool changed = true;
		while (changed)
		{
			changed = false;
			std::vector<left_type> nonTerminals = m_Gramma.GetNonterminalSymbol(VectorResult);
			for (size_t i = 0; i < nonTerminals.size(); ++i)
			{
				left_type& symbol = nonTerminals[i];
				bool localChanged = true;
				while (localChanged)
				{
					localChanged = false;
					std::vector<size_t> indexToRemove;
					for (int k = m_Gramma.m_Productions[symbol].size() - 1; k >= 0; --k)
					{
						right_type const& right = m_Gramma.m_Productions[symbol][k];
						if (right.size() > 0 && std::holds_alternative<std::string>(right[0]))
						{
							std::string const& firstAndNonterminal = std::get<std::string>(right[0]);
							for (int j = 0; j < i; ++j)
							{
								if (nonTerminals[j] == firstAndNonterminal)
								{
									localChanged = true;
									indexToRemove.push_back(k);
									std::vector<right_type> rightToPush;
									for (auto substitudePrefix : m_Gramma.m_Productions[firstAndNonterminal])
									{
										substitudePrefix.insert(substitudePrefix.end(), right.begin() + 1, right.end());
										rightToPush.emplace_back(std::move(substitudePrefix));
									}
									for (auto& _right : rightToPush)
									{
										m_Gramma.AddProduction(symbol, _right);
									}
									break;
								}
							}
						}
					}
					for (size_t index : indexToRemove)
					{
						m_Gramma.m_Productions[symbol].erase(m_Gramma.m_Productions[symbol].begin() + index);
					}
				}
				if (EliminateDirectLeftRecursion(symbol))
				{
					changed = true;
				}
			}
		}
	}

	bool GrammarNormalizer::EliminateDirectLeftRecursion(left_type const& symbol)
	{
		std::vector<right_type>& rights = m_Gramma.m_Productions[symbol];
		std::vector<size_t> leftRecursion;
		bool result = false;
		
		for (size_t i = 0;i < rights.size(); ++i)
		{
			right_type& right = rights[i];
			if (right.size() > 0)
			{
				std::visit([&](auto& value) {
					using value_type = std::remove_reference_t<decltype(value)>;
					if constexpr (std::is_same_v<value_type, left_type>)
					{
						if (value == symbol)
						{
							leftRecursion.push_back(i);
						}
					}
					}, right[0]);
			}
		}

		if (leftRecursion.size() > 0)
		{
			result = true;
			left_type newSymbol = GenerateNewSymbol(symbol);
			for (size_t index : leftRecursion)
			{
				if (rights[index].size() == 1) { throw std::runtime_error("bbb"); }
				right_type newright(rights[index].begin() + 1, rights[index].end());
				newright.push_back(newSymbol);
				m_Gramma.AddProduction(newSymbol, newright);
			}
			m_Gramma.AddProduction(newSymbol, {});

			std::sort(leftRecursion.begin(), leftRecursion.end(), std::greater<size_t>());
			for (size_t index : leftRecursion)
			{
				rights.erase(rights.begin() + index);
			}
			for (auto& right : rights)
			{
				right.push_back(newSymbol);
			}
		}
		return result;
	}
	
	void GrammarNormalizer::ExtractLeftFactoring()
	{
		bool changed = true;
		while (changed)
		{
			changed = false;

			for (auto& [left, rights] : m_Gramma.m_Productions)
			{
				std::unordered_map<char, std::vector<size_t>> charMap;
				std::unordered_map<left_type, std::vector<size_t>> stringMap;
				for (size_t i = 0;i < rights.size(); ++i)
				{
					if (rights[i].size() > 0)
					{
						std::visit([&](auto& value) {
							using value_type = std::remove_reference_t<decltype(value)>;
							if constexpr (std::is_same_v<value_type, char>)
							{
								charMap[value].push_back(i);
							}
							else if constexpr (std::is_same_v<value_type, left_type>)
							{
								stringMap[value].push_back(i);
							}
							else
							{
								throw std::runtime_error("aaa");
							}
							}, rights[i][0]);
					}
				}
				std::unordered_map<left_type, std::vector<right_type>> pairToPush;
				for (auto& [c, v] : charMap)
				{
					if (v.size() > 1)
					{
						changed = true;
						// 产生新的表达式
						left_type newSymbol = GenerateNewSymbol(left);
						for (size_t index : v)
						{
							right_type newRight(rights[index].begin() + 1, rights[index].end());
							pairToPush[newSymbol] .push_back(newRight);
						}

						// 移出老的表达式
						std::sort(v.begin(), v.end(), std::greater<size_t>());
						for (size_t index : v)
						{
							rights.erase(rights.begin() + index);
						}
						// 把新的加进去
						pairToPush[left].push_back({c, newSymbol});
					}
				}
				for (auto& [c, v] : stringMap)
				{
					if (v.size() > 1)
					{
						changed = true;
						// 产生新的表达式
						left_type newSymbol = GenerateNewSymbol(left);
						for (size_t index : v)
						{
							right_type newRight(rights[index].begin() + 1, rights[index].end());
							pairToPush[newSymbol].push_back(newRight);
						}

						// 移出老的表达式
						std::sort(v.begin(), v.end(), std::greater<size_t>());
						for (size_t index : v)
						{
							rights.erase(rights.begin() + index);
						}
						// 把新的加进去
						pairToPush[left].push_back({c, newSymbol});
					}
				}
				for (auto& pair : pairToPush)
				{
					for (right_type& _right : pair.second)
					{
						m_Gramma.AddProduction(pair.first, _right);
					}
				}
			}
		}
	}
	GrammarNormalizer::left_type GrammarNormalizer::GenerateNewSymbol(left_type const& origin)
	{
		int i = 1;
		while (true)
		{
			left_type newSymbol = origin + "_" + std::to_string(i);
			if (m_Gramma.m_Productions.find(newSymbol) == m_Gramma.m_Productions.end())
			{
				return newSymbol;
			}
			++i;
		}
	}
}