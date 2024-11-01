#pragma once

namespace Firefly
{
	template<size_t num>
	concept Is_PowerOf2 = ((num != 0) && (num & (num - 1)) == 0);
}