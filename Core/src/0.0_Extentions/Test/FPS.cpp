#include "FPS.h"
#include "0.0_Extentions/Updater/Time.h"

#include <iostream>

namespace Firefly
{
	FrameRatio::FrameRatio(int fpsInterval)
		:m_Counter(0), m_fpsInterval(fpsInterval), m_fps(0.0f), m_AccumulativeTime(0.0f)
	{
	}

	void FrameRatio::Update()
	{
		m_AccumulativeTime += Time::GetInstance().GetDeltaTime();
		++m_Counter;
		if (m_Counter >= m_fpsInterval)
		{
			m_fps = m_fpsInterval / m_AccumulativeTime;
			m_AccumulativeTime = 0.0f;
			m_Counter = 0;
			std::cout << m_fps << "\n";
		}
	}

}