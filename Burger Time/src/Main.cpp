#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#if USE_STEAMWORKS
#include "SteamManager.h"
#endif

#include "Minigin.h"
#include "GameLoader.h"

#include "ServiceLocator.h"
#include "SDLSoundService.h"

int main(int, char* [])
{
#if USE_STEAMWORKS
	dae::SteamManager::GetInstance().Initialize();
#endif

	dae::Minigin engine(dae::ResolveDataPath());

	dae::SDLSoundService soundService{};
	dae::ServiceLocator::Provide(&soundService);

	engine.Run(dae::LoadGame);

	dae::ServiceLocator::Provide(nullptr);

#if USE_STEAMWORKS
	dae::SteamManager::GetInstance().Shutdown();
#endif

	return 0;
}