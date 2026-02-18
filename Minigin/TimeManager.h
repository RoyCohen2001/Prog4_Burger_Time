#pragma once
#include "Singleton.h"
#include <chrono>

namespace dae
{
	using namespace std::chrono;

	class TimeManager final : public Singleton<TimeManager>
	{
	public:
		void SetCurrentTime() { m_currentTime = high_resolution_clock::now(); }
		void SetLastTime() { m_lastTime = m_currentTime; }

		void CalculateDeltaTime() { m_deltaTime = duration<float>(m_currentTime - m_lastTime).count(); }

		float GetDeltaTime() const { return m_deltaTime; }
		nanoseconds GetSleepTime() const { return m_currentTime + m_msPerFrame - high_resolution_clock::now(); }
		
		void SetTargetFPS(int fps)
		{
			if (fps > 0)
				m_msPerFrame = milliseconds(1000 / fps);
		}
	private:
		high_resolution_clock::time_point m_lastTime{};
		high_resolution_clock::time_point m_currentTime{};
		milliseconds m_msPerFrame{};

		float m_deltaTime{16};
	};
}