#include "GrammarSetCalculator.h"

namespace Firefly
{
	void GrammarSetCalculator::CalculateFirstSet()
	{
		bool changed = true;
		while (changed)
		{
			changed = false;
			for (auto const& [left, rights] : m_Productions)
			{
				for (right_type const& right : rights)
				{
					for (int i = 0; i < right.size(); ++i)
					{
						set_type firstSet = std::visit([this](auto& value) {
							return FirstSet(value);
							}, right[i]);
						for (member_type f : firstSet)
						{
							if (m_FirstSet[left].insert(f).second)
							{
								changed = true;
							}
						}

						if (!std::visit([this](auto& value) {
							return IsNullable(value);
							}, right[i]))
						{
							break;
						}
					}
				}
			}
		}

		for (left_type const& nullableSymbol : m_NullableSymbols)
		{
			m_FirstSet[nullableSymbol].insert('\0');
		}
	}

	void GrammarSetCalculator::CalculateFollowSet()
	{
		m_FollowSet[m_StartSymbol].insert('$');

		bool changed = true;
		while (changed)
		{
			changed = false;
			for (auto const& [left, rights] : m_Productions)
			{
				for (right_type const& right : rights)
				{
					for (size_t i = 0; i < right.size(); ++i)
					{
						std::visit([i, &right, this, &left, &changed](auto const& value) {
							using value_type = std::remove_const_t<std::remove_reference_t<decltype(value)>>;
							if constexpr (std::is_same_v<value_type, left_type>)
							{
								if (i == (right.size() - 1))
								{
									for (member_type const& member : m_FollowSet[left])
									{
										if (m_FollowSet[value].insert(member).second)
										{
											changed = true;
										}
									}
								}
								else
								{
									right_type temp(right.begin() + i + 1, right.end());
									for (member_type const& member : FirstSetForExpression(temp))
									{
										if (std::visit([](auto const& value) {
											using value_type = std::remove_const_t<std::remove_reference_t<decltype(value)>>;
											if constexpr (std::is_same_v<value_type, char>)
											{
												return value != '\0';
											}
											else
											{
												return true;
											}
											}, member))
										{
											if (m_FollowSet[value].insert(member).second)
											{
												changed = true;
											}
										}
									}
									if (IsNullableForExpression(temp))
									{
										for (member_type const& member : m_FollowSet[left])
										{
											if (m_FollowSet[value].insert(member).second)
											{
												changed = true;
											}
										}
									}
								}
							}
							}, right[i]);
					}
				}
			}
		}
	}
	
	void GrammarSetCalculator::CalculateSelectSet()
	{
		for (auto const& [left, rights] : m_Productions)
		{
			for (right_type const& right : rights)
			{
				set_type temp;
				for (member_type member : FirstSetForExpression(right))
				{
					if (std::visit([](auto const& value) {
						using value_type = std::remove_const_t<std::remove_reference_t<decltype(value)>>;
						if constexpr (std::is_same_v<value_type, char>)
						{
							return value != '\0';
						}
						else
						{
							return true;
						}
						}, member))
					{
						temp.insert(member);
					}
				}
				if (IsNullableForExpression(right))
				{
					// 能推出空字符串
					for (member_type _member : m_FollowSet[left])
					{
						temp.insert(_member);
					}
				}
				m_SelectSet[left].push_back(temp);
			}
		}
	}

	void GrammarSetCalculator::CalculateNullableSet()
	{
		std::unordered_map<left_type, bool> record;
		for (auto const& [left, rights] : m_Productions)
		{
			record[left] = false;
			for (right_type const& right : rights)
			{
				if (right.size() == 0)
				{
					record[left] = true;
					break;
				}
			}
		}
		auto judgeIsNullable = [&record](auto const& value) {
			using value_type = std::remove_const_t<std::remove_reference_t<decltype(value)>>;
			if constexpr (std::is_same_v<value_type, char>)
			{
				return false;
			}
			else if constexpr (std::is_same_v<value_type, std::string>)
			{
				return record[value];
			}
			};

		bool changed = true;
		while (changed)
		{
			changed = false;
			for (auto const& [left, rights] : m_Productions)
			{
				if (!record[left]) // 对所有还不是可推导出空字符串的非终结符进行判断
				{
					bool isnullable = false;
					for (right_type const& right : rights)
					{
						bool _isnullable = true;
						for (member_type const& member : right)
						{
							if (!std::visit(judgeIsNullable, member))
							{
								_isnullable = false;
								break;
							}
						}
						if (_isnullable)
						{
							isnullable = true;
							break;
						}
					}
					if (isnullable)
					{
						changed = true;
						record[left] = true;
					}
				}
			}
		}

		for (auto const& pair : record)
		{
			if (pair.second)
			{
				m_NullableSymbols.insert(pair.first);
			}
		}
	}

	GrammarSetCalculator::set_type GrammarSetCalculator::FirstSetForExpression(right_type const& expression)
	{
		set_type result;
		if (expression.size() == 0)
		{
			return result;
		}
		for (member_type const& member : expression)
		{
			for (member_type const& f : std::visit([this](auto const& value) {
				return FirstSet(value);
				}, member))
			{
				result.insert(f);
			}

			if (!std::visit([this](auto const& value) {
				return IsNullable(value);
				}, member))
			{
				break;
			}
		}
		return result;
	}

	bool GrammarSetCalculator::IsNullableForExpression(right_type const& expression)
	{
		for (member_type const& e : expression)
		{
			if (!std::visit([this](auto const& value) {
				return IsNullable(value);
				}, e))
			{
				return false;
			}
		}
		return true;
	}

}