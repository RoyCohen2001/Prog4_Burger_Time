#pragma once
#include "Singleton.h"
#include <chrono>

namespace dae
{
	using namespace std::chrono;

	class TimeManager final : public Singleton<TimeManager>
	{
	public:
		void Initialize() { 
			SetCurrentTime();
			CalculateDeltaTime();
			SetLastTime();
		}
		
		void SetLastTime() { m_lastTime = m_currentTime; }

		float GetDeltaTime() const { return m_deltaTime; }
		nanoseconds GetSleepTime() const
		{
			const auto target = m_currentTime + m_msPerFrame;
			const auto now = high_resolution_clock::now();
			if (now >= target)
				return nanoseconds{ 0 };
			return target - now;
		}
		
		void SetTargetFPS(int fps)
		{
			if (fps > 0)
				m_msPerFrame = milliseconds(1000 / fps);
		}
	private:
		void SetCurrentTime() { m_currentTime = high_resolution_clock::now(); }
		void CalculateDeltaTime() { m_deltaTime = duration<float>(m_currentTime - m_lastTime).count(); }

		high_resolution_clock::time_point m_lastTime{};
		high_resolution_clock::time_point m_currentTime{};
		milliseconds m_msPerFrame{16};

		float m_deltaTime{};
	};
}