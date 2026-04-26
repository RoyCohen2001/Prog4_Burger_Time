#pragma once
#include "SoundService.h"

namespace dae
{
	class NullSoundService final : public SoundService
	{
	public:
		void QueueLoad(const std::string&, const std::string&) override {}
		void QueuePlay(const std::string&, int, bool) override {}
		void QueueStopAll() override {}
	};
}