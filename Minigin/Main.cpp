#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#if USE_STEAMWORKS
#include "SteamManager.h"
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "Util.h"
#include "InputManager.h"

//Game loading
#include "LevelLoader.h"	

//components
#include "TextComponent.h"
#include "FPSComponent.h"
#include "RenderComponent.h"
#include "MovementComponent.h"
#include "TimeManager.h"
//#include "ImGuiComponent.h"
#include "GameActor.h"

#include <filesystem>
namespace fs = std::filesystem;
void SetInputMappingController(dae::GameActor* actor, int controller)
{
	// Movement
	dae::InputManager::GetInstance().BindCommandToGamepad(controller, dae::InputState::Pressed, dae::Button::DPAD_LEFT, std::make_unique<dae::Move>(actor, glm::vec2{ -1, 0 }));
	dae::InputManager::GetInstance().BindCommandToGamepad(controller, dae::InputState::Pressed, dae::Button::DPAD_RIGHT, std::make_unique<dae::Move>(actor, glm::vec2{ 1, 0 }));
	dae::InputManager::GetInstance().BindCommandToGamepad(controller, dae::InputState::Pressed, dae::Button::DPAD_UP, std::make_unique<dae::Move>(actor, glm::vec2{ 0, -1 }));
	dae::InputManager::GetInstance().BindCommandToGamepad(controller, dae::InputState::Pressed, dae::Button::DPAD_DOWN, std::make_unique<dae::Move>(actor, glm::vec2{ 0, 1 }));

	// Actions
	dae::InputManager::GetInstance().BindCommandToGamepad(controller, dae::InputState::DownThisFrame, dae::Button::X, std::make_unique<dae::Damage>(actor));
	dae::InputManager::GetInstance().BindCommandToGamepad(controller, dae::InputState::DownThisFrame, dae::Button::B, std::make_unique<dae::Pellets>(actor));
}

void SetInputMappingKeyboard(dae::GameActor* actor)
{
	// Movement
	dae::InputManager::GetInstance().BindCommandToKeyboard(SDL_SCANCODE_A, dae::InputState::Pressed, std::make_unique<dae::Move>(actor, glm::vec2{ -1, 0 }));
	dae::InputManager::GetInstance().BindCommandToKeyboard(SDL_SCANCODE_D, dae::InputState::Pressed, std::make_unique<dae::Move>(actor, glm::vec2{ 1, 0 }));
	dae::InputManager::GetInstance().BindCommandToKeyboard(SDL_SCANCODE_W, dae::InputState::Pressed, std::make_unique<dae::Move>(actor, glm::vec2{ 0, -1 }));
	dae::InputManager::GetInstance().BindCommandToKeyboard(SDL_SCANCODE_S, dae::InputState::Pressed, std::make_unique<dae::Move>(actor, glm::vec2{ 0, 1 }));

	// Actions
	dae::InputManager::GetInstance().BindCommandToKeyboard(SDL_SCANCODE_C, dae::InputState::DownThisFrame, std::make_unique<dae::Damage>(actor));
	dae::InputManager::GetInstance().BindCommandToKeyboard(SDL_SCANCODE_X, dae::InputState::DownThisFrame, std::make_unique<dae::Pellets>(actor));
}

static void load()
{
	// FONT
	const auto MAIN_FONT = dae::ResourceManager::GetInstance().LoadFont("BurgerTime.otf", 36);

	auto& scene = dae::SceneManager::GetInstance().CreateScene();

	dae::LevelLoader::LoadLevelFromJson("Data/Levels/level1.json", scene, glm::vec2{-50, 50});

	auto textPlayer1 = std::make_unique<dae::GameObject>();
	textPlayer1->SetPosition(70, 10);
	auto textComponent1 = textPlayer1->AddComponent<dae::TextComponent>("Use the d-pad to move player 1 (A - damage) -- (B - pellets)", MAIN_FONT);
	textComponent1->SetSize(14);
	textComponent1->SetColor({ 255, 0, 0, 255 });
	scene.Add(std::move(textPlayer1));

	auto textPLayer1Lives = std::make_unique<dae::GameObject>();
	textPLayer1Lives->SetPosition(70, 30);
	auto textlives1 = textPLayer1Lives->AddComponent<dae::TextComponent>("Lives: ", MAIN_FONT);
	textlives1->SetSize(10);
	textlives1->SetColor({ 255, 255, 0, 255 });
	scene.Add(std::move(textPLayer1Lives));

	auto textPLayer1Score = std::make_unique<dae::GameObject>();
	textPLayer1Score->SetPosition(70, 40);
	auto textscore1 = textPLayer1Score->AddComponent<dae::TextComponent>("Score: ", MAIN_FONT);
	textscore1->SetSize(10);
	textscore1->SetColor({ 255, 255, 0, 255 });
	scene.Add(std::move(textPLayer1Score));

	auto textPlayer2 = std::make_unique<dae::GameObject>();
	textPlayer2->SetPosition(70, 60);
	auto textComponent2 = textPlayer2->AddComponent<dae::TextComponent>("Use WASD to move player 2 (C - damage) -- (X - pellets)", MAIN_FONT);
	textComponent2->SetSize(14);
	textComponent2->SetColor({ 255, 0, 0, 255 });
	scene.Add(std::move(textPlayer2));

	auto textPLayer2Lives = std::make_unique<dae::GameObject>();
	textPLayer2Lives->SetPosition(70, 80);
	auto textlives2 = textPLayer2Lives->AddComponent<dae::TextComponent>("Lives: ", MAIN_FONT, SDL_Color{ 255,255,0,255 }, dae::TextComponent::DisplayType::Lives);
	textlives2->SetSize(10);
	textlives2->SetColor({ 255, 255, 0, 255 });
	scene.Add(std::move(textPLayer2Lives));

	auto textPLayer2Score = std::make_unique<dae::GameObject>();
	textPLayer2Score->SetPosition(70, 90);
	auto textscore2 = textPLayer2Score->AddComponent<dae::TextComponent>("Score: ", MAIN_FONT, SDL_Color{ 255,255,0,255 }, dae::TextComponent::DisplayType::Score);
	textscore2->SetSize(10);
	textscore2->SetColor({ 255, 255, 0, 255 });
	scene.Add(std::move(textPLayer2Score));

	// Add FPS counter
	auto fpsObject = std::make_unique<dae::GameObject>();
	fpsObject->SetPosition(10.f, 10.f);
	fpsObject->AddComponent<dae::FPSComponent>();                 
	scene.Add(std::move(fpsObject));

	// ****************************************************************************
	//									PLAYERS
	// ****************************************************************************
	auto player1 = std::make_unique<dae::GameObject>();
	player1->SetPosition(200.f, 300.f);
	auto player1Actor = player1->AddComponent<dae::GameActor>();
	player1Actor->SetTexture("Sprites/PeterPepper/peter.png");
	player1Actor->SetSpeed(60.f);
	scene.Add(std::move(player1));

	player1Actor->AddObserver(textlives1);
	player1Actor->AddObserver(textscore1);

	auto player2 = std::make_unique<dae::GameObject>();
	player2->SetPosition(200.f, 350.f);
	auto player2Actor = player2->AddComponent<dae::GameActor>();
	player2Actor->SetTexture("Sprites/PeterPepper/peter.png");
	player2Actor->SetSpeed(120.f);
	scene.Add(std::move(player2));

	player2Actor->AddObserver(textlives2);
	player2Actor->AddObserver(textscore2);

	SetInputMappingController(player1Actor, 0);
	SetInputMappingKeyboard(player2Actor);

#if USE_STEAMWORKS
	player2Actor->AddObserver(&dae::SteamManager::GetInstance());
#endif
}

int main(int, char*[]) {
#if __EMSCRIPTEN__
	fs::path data_location = "";
#else
	fs::path data_location = "./Data/";
	if(!fs::exists(data_location))
		data_location = "../Data/";
#endif

#if USE_STEAMWORKS
	dae::SteamManager::GetInstance().Initialize();
#endif

	dae::Minigin engine(data_location);
	engine.Run(load);

#if USE_STEAMWORKS
	dae::SteamManager::GetInstance().Shutdown();
#endif

    return 0;
}
