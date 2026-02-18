#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"

//components
#include "TextComponent.h"
#include "FPSComponent.h"
#include "RenderComponent.h"

#include <filesystem>
namespace fs = std::filesystem;

static void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();
	
	// FONT
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	// Replace SetTexture() with AddComponent
	auto go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>("background.png");
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	go->SetPosition(358, 180);
	go->AddComponent<dae::RenderComponent>("logo.png");
	scene.Add(std::move(go));

	auto textObject = std::make_unique<dae::GameObject>();
	textObject->SetPosition(292, 20);
	auto textComponent = textObject->AddComponent<dae::TextComponent>("Programming 4 Assignment", font);
	textComponent->SetColor({ 255, 255, 0, 255 });
	scene.Add(std::move(textObject));

	// Add FPS counter
	auto fpsObject = std::make_unique<dae::GameObject>();
	fpsObject->SetPosition(10.f, 10.f);
	fpsObject->AddComponent<dae::TextComponent>("FPS: 0", font); 
	fpsObject->AddComponent<dae::FPSComponent>();                 
	scene.Add(std::move(fpsObject));
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
