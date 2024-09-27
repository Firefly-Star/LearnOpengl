#include "MousePos.h"
#include "Time.h"

namespace Firefly
{
	float MousePos::s_X = 0.0f;
	float MousePos::s_Y = 0.0f;
	float MousePos::s_DeltaX = 0.0f;
	float MousePos::s_DeltaY = 0.0f;
	bool MousePos::s_FirstUpdate = true;

	float Time::s_Time = 0.0f;
	float Time::s_DeltaTime = 1 / 60.0f;
}
