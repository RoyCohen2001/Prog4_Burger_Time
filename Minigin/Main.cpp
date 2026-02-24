#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "Util.h"

//components
#include "TextComponent.h"
#include "FPSComponent.h"
#include "RenderComponent.h"
#include "MovementComponent.h"
#include "TimeManager.h"

#include <filesystem>
namespace fs = std::filesystem;

static void load()
{
	// FONT
	const auto MAIN_FONT = dae::ResourceManager::GetInstance().LoadFont("BurgerTime.otf", 36);

	auto& scene = dae::SceneManager::GetInstance().CreateScene();

	auto go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>("background.png");
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	go->SetPosition(358, 180);
	go->AddComponent<dae::RenderComponent>("logo.png");
	scene.Add(std::move(go));

	auto textObject = std::make_unique<dae::GameObject>();
	textObject->SetPosition(292, 20);
	auto textComponent = textObject->AddComponent<dae::TextComponent>("Programming 4 Assignment", MAIN_FONT);
	textComponent->SetColor({ 255, 255, 0, 255 });
	scene.Add(std::move(textObject));

	// Add FPS counter
	auto fpsObject = std::make_unique<dae::GameObject>();
	fpsObject->SetPosition(10.f, 10.f);
	fpsObject->AddComponent<dae::FPSComponent>();                 
	scene.Add(std::move(fpsObject));

	// ****************************************************************************
	//							CHILD PARENT EXAMPLE
	// ****************************************************************************
	// PARENT
	auto player = std::make_unique<dae::GameObject>();
	player->SetPosition(SCREEN_WIDTH/2, SCREEN_HEIGHT - SCREEN_HEIGHT/3);
	player->AddComponent<dae::RenderComponent>("peter.png");
	player->AddComponent<dae::MovementComponent>(5.f,50.0f);
	auto playerPtr = player.get();
	scene.Add(std::move(player));

	// CHILD
	auto satellite = std::make_unique<dae::GameObject>();
	satellite->AddComponent<dae::RenderComponent>("peter.png");
	satellite->AddComponent<dae::MovementComponent>(-10.0f, 30.0f);
	auto satellitePtr = satellite.get();
	scene.Add(std::move(satellite));

	// Setting parent
	satellitePtr->SetParent(playerPtr, true); // false = don't keep world position
}

int main(int, char*[]) {
#if __EMSCRIPTEN__
	fs::path data_location = "";
#else
	fs::path data_location = "./Data/";
	if(!fs::exists(data_location))
		data_location = "../Data/";
#endif
	dae::Minigin engine(data_location);
	engine.Run(load);
    return 0;
}
