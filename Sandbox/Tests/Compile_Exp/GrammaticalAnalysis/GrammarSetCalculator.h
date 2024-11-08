#pragma once

#include "Grammar.h"

namespace Firefly
{
	class GrammarSetCalculator
	{
	public:
		using left_type = Grammar::left_type;
		using member_type = Grammar::member_type;
		using right_type = Grammar::right_type;
		using set_type = Grammar::set_type;
	public:
		explicit GrammarSetCalculator(Grammar const& gramma) : m_Productions(gramma.GetProductions()), m_StartSymbol(gramma.GetStartSymbol())
		{
			CalculateNullableSet();
			CalculateFirstSet();
			CalculateFollowSet();
			CalculateSelectSet();
		}
		~GrammarSetCalculator() = default;

		[[nodiscard]] std::unordered_set<left_type> const& GetNullableSet() const{ return m_NullableSymbols; }
		[[nodiscard]] std::unordered_map<left_type, std::vector<set_type>> const& GetSelectSet() const { return m_SelectSet; }
		[[nodiscard]] std::unordered_map<left_type, set_type> const& GetFirstSet() const{ return m_FirstSet; }
		[[nodiscard]] std::unordered_map<left_type, set_type> const& GetFollowSet() const{ return m_FollowSet; }
	private:
		left_type const& m_StartSymbol;
		std::unordered_map<left_type, std::vector<right_type>> const& m_Productions;
		std::unordered_set<left_type> m_NullableSymbols;
		std::unordered_map<left_type, set_type> m_FirstSet;
		std::unordered_map<left_type, set_type> m_FollowSet;
		std::unordered_map<left_type, std::vector<set_type>> m_SelectSet;

	private:
		void CalculateNullableSet();
		void CalculateFirstSet();
		void CalculateFollowSet();
		void CalculateSelectSet();

		template<typename T>
		set_type FirstSet(T& value);

		set_type FirstSetForExpression(right_type const& expression);

		template<typename T>
		bool IsNullable(T& value);

		bool IsNullableForExpression(right_type const& expression);
	};

	template<typename T>
	inline GrammarSetCalculator::set_type GrammarSetCalculator::FirstSet(T& value)
	{
		using value_type = std::remove_const_t<std::remove_reference_t<decltype(value)>>;
		if constexpr (std::is_same_v<value_type, char>)
		{
			return set_type{ value };
		}
		else if constexpr (std::is_same_v<value_type, left_type>)
		{
			return m_FirstSet[value];
		}
		else
		{
			throw std::runtime_error("aaa");
		}
	}
	
	template<typename T>
	inline bool GrammarSetCalculator::IsNullable(T& value)
	{
		using value_type = std::remove_const_t<std::remove_reference_t<decltype(value)>>;
		if constexpr (std::is_same_v<value_type, char>)
		{
			return false;
		}
		else if constexpr (std::is_same_v<value_type, left_type>)
		{
			return (m_NullableSymbols.count(value) != 0);
		}
		else
		{
			throw std::runtime_error("aaa");
		}
	}
}