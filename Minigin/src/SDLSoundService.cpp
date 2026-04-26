#include "SDLSoundService.h"

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

#include <algorithm>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

namespace dae
{
	enum class SoundRequestType
	{
		Load,
		Play,
		StopAll,
		Shutdown
	};

	struct SoundRequest
	{
		SoundRequestType Type{};
		std::string SoundId{};
		std::string FilePath{};
		int Volume{ 128 };
		bool Loop{ false };
	};

	class SDLSoundService::Impl final
	{
	public:
		bool m_MixInitialized{ false };

		Impl() {
			m_MixInitialized = MIX_Init();
			if (!m_MixInitialized) {
				SDL_Log("MIX_Init failed: %s", SDL_GetError());
			}
			else {
				m_Mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
				if (!m_Mixer) SDL_Log("MIX_CreateMixerDevice failed: %s", SDL_GetError());
			}
			m_Worker = std::jthread([this] { ProcessRequests(); });
		}

		~Impl() {
			Enqueue({ SoundRequestType::Shutdown });
			if (m_Worker.joinable()) m_Worker.join();

			for (auto& [_, audio] : m_Sounds) {
				if (audio) MIX_DestroyAudio(audio);
			}
			m_Sounds.clear();

			m_ActiveTracks.clear(); 

			if (m_Mixer) {
				MIX_DestroyMixer(m_Mixer);
				m_Mixer = nullptr;
			}

			if (m_MixInitialized) {
				MIX_Quit();
			}
		}

		void Enqueue(const SoundRequest& req)
		{
			{
				std::lock_guard lock(m_QueueMutex);
				m_RequestQueue.push(req);
			}
			m_Cv.notify_one();
		}

	private:
		void ProcessRequests()
		{
			bool running = true;

			while (running)
			{
				SoundRequest req{};
				{
					std::unique_lock lock(m_QueueMutex);
					m_Cv.wait(lock, [this] { return !m_RequestQueue.empty(); });

					req = m_RequestQueue.front();
					m_RequestQueue.pop();
				}

				CleanupFinishedTracks();

				switch (req.Type)
				{
				case SoundRequestType::Load:
					HandleLoad(req.SoundId, req.FilePath);
					break;
				case SoundRequestType::Play:
					HandlePlay(req.SoundId, req.Volume, req.Loop);
					break;
				case SoundRequestType::StopAll:
					HandleStopAll();
					break;
				case SoundRequestType::Shutdown:
					running = false;
					break;
				}
			}
		}

		void HandleLoad(const std::string& id, const std::string& filePath)
		{
			if (!m_Mixer || id.empty() || filePath.empty() || m_Sounds.contains(id))
			{
				return;
			}

			MIX_Audio* audio = MIX_LoadAudio(m_Mixer, filePath.c_str(), false);
			if (!audio)
			{
				SDL_Log("MIX_LoadAudio failed for '%s': %s", filePath.c_str(), SDL_GetError());
				return;
			}

			m_Sounds.emplace(id, audio);
		}

		void HandlePlay(const std::string& id, int volume, bool loop)
		{
			if (!m_Mixer)
			{
				return;
			}

			const auto it = m_Sounds.find(id);
			if (it == m_Sounds.end() || !it->second)
			{
				SDL_Log("Sound '%s' not loaded", id.c_str());
				return;
			}

			MIX_Track* track = MIX_CreateTrack(m_Mixer);
			if (!track)
			{
				SDL_Log("MIX_CreateTrack failed: %s", SDL_GetError());
				return;
			}

			if (!MIX_SetTrackAudio(track, it->second))
			{
				SDL_Log("MIX_SetTrackAudio failed: %s", SDL_GetError());
				MIX_DestroyTrack(track);
				return;
			}

			volume = std::clamp(volume, 0, 128);
			const float gain = static_cast<float>(volume) / 128.0f;
			if (!MIX_SetTrackGain(track, gain))
			{
				SDL_Log("MIX_SetTrackGain failed: %s", SDL_GetError());
			}

			SDL_PropertiesID options = 0;
			if (loop)
			{
				options = SDL_CreateProperties();
				if (options)
				{
					SDL_SetNumberProperty(options, MIX_PROP_PLAY_LOOPS_NUMBER, -1);
				}
			}

			if (!MIX_PlayTrack(track, options))
			{
				SDL_Log("MIX_PlayTrack failed: %s", SDL_GetError());
				if (options)
				{
					SDL_DestroyProperties(options);
				}
				MIX_DestroyTrack(track);
				return;
			}

			if (options)
			{
				SDL_DestroyProperties(options);
			}

			m_ActiveTracks.push_back(track);
		}

		void HandleStopAll()
		{
			if (!m_Mixer)
			{
				return;
			}

			if (!MIX_StopAllTracks(m_Mixer, 0))
			{
				SDL_Log("MIX_StopAllTracks failed: %s", SDL_GetError());
			}

			CleanupFinishedTracks();
		}

		void CleanupFinishedTracks()
		{
			m_ActiveTracks.erase(
				std::remove_if(
					m_ActiveTracks.begin(),
					m_ActiveTracks.end(),
					[](MIX_Track* track)
					{
						if (!track)
						{
							return true;
						}

						if (MIX_TrackPlaying(track) || MIX_TrackPaused(track))
						{
							return false;
						}

						MIX_DestroyTrack(track);
						return true;
					}),
				m_ActiveTracks.end());
		}

		void CleanupAllTracks()
		{
			for (MIX_Track* track : m_ActiveTracks)
			{
				if (track)
				{
					MIX_DestroyTrack(track);
				}
			}
			m_ActiveTracks.clear();
		}

		MIX_Mixer* m_Mixer{};
		std::unordered_map<std::string, MIX_Audio*> m_Sounds{};
		std::vector<MIX_Track*> m_ActiveTracks{};

		std::queue<SoundRequest> m_RequestQueue{};
		std::mutex m_QueueMutex{};
		std::condition_variable m_Cv{};
		std::jthread m_Worker{};
	};
}

dae::SDLSoundService::SDLSoundService()
	: m_pImpl(std::make_unique<Impl>())
{}

dae::SDLSoundService::~SDLSoundService() = default;

void dae::SDLSoundService::QueueLoad(const std::string& soundId, const std::string& filePath)
{
	m_pImpl->Enqueue({ SoundRequestType::Load, soundId, filePath, 128, false });
}

void dae::SDLSoundService::QueuePlay(const std::string& soundId, int volume, bool loop)
{
	m_pImpl->Enqueue({ SoundRequestType::Play, soundId, {}, volume, loop });
}

void dae::SDLSoundService::QueueStopAll()
{
	m_pImpl->Enqueue({ SoundRequestType::StopAll });
}