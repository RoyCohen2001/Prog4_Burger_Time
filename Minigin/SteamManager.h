#pragma once
#include <stdexcept>
#include <string>

#include "Observer.h"
#include "GameActor.h"
#include "GameObject.h"
#include "Singleton.h"

#if USE_STEAMWORKS
#pragma warning (push)
#pragma warning (disable:4996)
#include <steam_api.h>
#pragma warning (pop)
#endif

namespace dae
{
	class SteamManager final : public Singleton<SteamManager>,  public Observer
	{
	public:
		SteamManager() = default;
		~SteamManager() = default;

		SteamManager(const SteamManager& other) = delete;
		SteamManager(SteamManager&& other) noexcept = delete;
		SteamManager& operator=(const SteamManager& other) = delete;
		SteamManager& operator=(SteamManager&& other) noexcept = delete;

		void Initialize()
		{
			if (!SteamAPI_Init())
				throw std::runtime_error(std::string("Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed)."));
		}

		void Shutdown()
		{
			SteamAPI_Shutdown();
		}

		void OnNotify(const std::string& event, GameObject* gameActor) override
		{
			auto actor = dynamic_cast<GameActor*>(gameActor->GetComponent<GameActor>());
			if (event == "ScoreChanged")
			{
				if (actor)
				{
					if (actor->GetScore() >= 50) {
						UnlockAchievement("ACH_WIN_ONE_GAME");
					}
				}
			}
		}

	private:
		void UnlockAchievement(const char* achievementID)
		{
			if (SteamUserStats())
			{
				SteamUserStats()->SetAchievement(achievementID);
				SteamUserStats()->StoreStats();
			}
		}
	};
}