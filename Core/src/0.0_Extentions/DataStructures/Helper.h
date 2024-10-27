#pragma once

namespace Firefly
{
	inline constexpr bool IsPowerOf2(const size_t num)
	{
		return (num != 0) && ((num & (num - 1)) == 0);
	}
}