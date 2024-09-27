#pragma once
namespace Firefly
{
	class Time
	{
	public:
		static void Update()
		{
			float time = static_cast<float>(glfwGetTime());
			s_DeltaTime = time - s_Time;
			s_Time = time;
		}
		static float GetTime() { return s_Time; }
		static float GetDeltaTime() { return s_DeltaTime; }
	private:
		static float s_Time;
		static float s_DeltaTime;
	};
}