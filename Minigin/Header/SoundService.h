#pragma once
#include <string>

namespace dae
{
	class SoundService
	{
	public:
		virtual ~SoundService() = default;

		virtual void QueueLoad(const std::string& soundId, const std::string& filePath) = 0;
		virtual void QueuePlay(const std::string& soundId, int volume = 128, bool loop = false) = 0;
		virtual void QueueStopAll() = 0;
	};
}