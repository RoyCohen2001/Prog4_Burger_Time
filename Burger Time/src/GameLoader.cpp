#include "GameLoader.h"

#include "SceneManager.h"
#include "ServiceLocator.h"
#include <filesystem>
#include "GameStateManager.h"
#include "MenuState.h"

#if USE_STEAMWORKS
#include "SteamManager.h"
#endif

namespace fs = std::filesystem;

std::filesystem::path dae::ResolveDataPath()
{
#if __EMSCRIPTEN__
	return "";
#else
	fs::path dataLocation = "./Data/";
	if (!fs::exists(dataLocation))
		dataLocation = "../Data/";
	return dataLocation;
#endif
}

void dae::LoadGame()
{
	ServiceLocator::GetSoundService().QueueLoad("music", "Data/Sounds/music.wav");
	ServiceLocator::GetSoundService().QueuePlay("music", 64, true);

	GameStateManager::GetInstance().ChangeState(std::make_unique<MenuState>());
}