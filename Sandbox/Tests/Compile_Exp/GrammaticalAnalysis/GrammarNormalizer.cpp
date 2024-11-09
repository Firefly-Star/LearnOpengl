#include "GrammarNormalizer.h"
#include <algorithm>

namespace Firefly
{
	void GrammarNormalizer::Normalize()
	{
		EliminateLeftRecursion();
		m_Gramma.PrintProductions();
		std::cout << "\n";
		ExtractLeftFactoring();
		m_Gramma.PrintProductions();
		std::cout << "\n";
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

			std::unordered_map<left_type, std::vector<std::pair<left_type, right_type>>> newProductionsToAdd;
			std::unordered_map<left_type, std::vector<size_t>> indicesToRemove;

			for (const auto& [left, rights] : m_Gramma.m_Productions)
			{
				std::unordered_map<member_type, std::vector<size_t>> record;
				for (size_t i = 0; i < rights.size(); ++i)
				{
					if (!rights[i].empty())
					{
						record[rights[i][0]].push_back(i);
					}
				}

				for (const auto& [first, indices] : record)
				{
					if (indices.size() > 1)
					{
						changed = true;
						left_type newSymbol = GenerateNewSymbol(left);
						std::vector<right_type> newProductions;

						for (size_t index : indices)
						{
							newProductions.emplace_back(rights[index].begin() + 1, rights[index].end());
						}

						for (const auto& newProduction : newProductions)
						{
							newProductionsToAdd[newSymbol].emplace_back(newSymbol, newProduction);
						}

						indicesToRemove[left].insert(indicesToRemove[left].end(), indices.begin(), indices.end());

						newProductionsToAdd[left].emplace_back(left, right_type{ first, newSymbol });
					}
				}
			}

			for (const auto& [symbol, additions] : newProductionsToAdd)
			{
				for (const auto& [_, production] : additions)
				{
					m_Gramma.m_Productions[symbol].push_back(production);
				}
			}

			for (const auto& [symbol, indices] : indicesToRemove)
			{
				std::vector<size_t> sortedIndices = indices;
				std::sort(sortedIndices.begin(), sortedIndices.end(), std::greater<size_t>());
				for (size_t index : sortedIndices)
				{
					m_Gramma.m_Productions[symbol].erase(m_Gramma.m_Productions[symbol].begin() + index);
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