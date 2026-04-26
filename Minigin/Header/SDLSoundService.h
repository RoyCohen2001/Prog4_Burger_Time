#pragma once
#include <memory>
#include "SoundService.h"

namespace dae
{
	class SDLSoundService final : public SoundService
	{
	public:
		SDLSoundService();
		~SDLSoundService() override;

		SDLSoundService(const SDLSoundService&) = delete;
		SDLSoundService(SDLSoundService&&) = delete;
		SDLSoundService& operator=(const SDLSoundService&) = delete;
		SDLSoundService& operator=(SDLSoundService&&) = delete;

		void QueueLoad(const std::string& soundId, const std::string& filePath) override;
		void QueuePlay(const std::string& soundId, int volume = 128, bool loop = false) override;
		void QueueStopAll() override;

	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl;
	};
}