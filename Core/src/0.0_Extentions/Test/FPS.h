#pragma once

namespace Firefly
{
	class FrameRatio
	{
	public:
		FrameRatio(int fpsInterval = 50);
		void Update();
		float PrintFrameRatio() { return m_fps; }
	private:
		int m_fpsInterval;
		int m_Counter;
		float m_AccumulativeTime;
		float m_fps;
	};
}