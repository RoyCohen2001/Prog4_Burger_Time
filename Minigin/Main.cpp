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
#include "InputManager.h"

//components
#include "TextComponent.h"
#include "FPSComponent.h"
#include "RenderComponent.h"
#include "MovementComponent.h"
#include "TimeManager.h"
#include "ImGuiComponent.h"
#include "GameActor.h"

#include <filesystem>
namespace fs = std::filesystem;
void SetInputMappingController(dae::GameActor* actor, int controller)
{
	// Movement
	dae::InputManager::GetInstance().BindCommandToGamepad(controller, dae::InputState::Pressed, dae::Button::DPAD_LEFT, new dae::Move(actor, glm::vec2{ -1, 0 }));
	dae::InputManager::GetInstance().BindCommandToGamepad(controller, dae::InputState::Pressed, dae::Button::DPAD_RIGHT, new dae::Move(actor, glm::vec2{ 1, 0 }));
	dae::InputManager::GetInstance().BindCommandToGamepad(controller, dae::InputState::Pressed, dae::Button::DPAD_UP, new dae::Move(actor, glm::vec2{ 0, -1 }));
	dae::InputManager::GetInstance().BindCommandToGamepad(controller, dae::InputState::Pressed, dae::Button::DPAD_DOWN, new dae::Move(actor, glm::vec2{ 0, 1 }));
}

void SetInputMappingKeyboard(dae::GameActor* actor)
{
	// Movement
	dae::InputManager::GetInstance().BindCommandToKeyboard(SDL_SCANCODE_A, dae::InputState::Pressed, new dae::Move(actor, glm::vec2{ -1, 0 }));
	dae::InputManager::GetInstance().BindCommandToKeyboard(SDL_SCANCODE_D, dae::InputState::Pressed, new dae::Move(actor, glm::vec2{ 1, 0 }));
	dae::InputManager::GetInstance().BindCommandToKeyboard(SDL_SCANCODE_W, dae::InputState::Pressed, new dae::Move(actor, glm::vec2{ 0, -1 }));
	dae::InputManager::GetInstance().BindCommandToKeyboard(SDL_SCANCODE_S, dae::InputState::Pressed, new dae::Move(actor, glm::vec2{ 0, 1 }));
}

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

	auto textPlayer1 = std::make_unique<dae::GameObject>();
	textPlayer1->SetPosition(20, 120);
	auto textComponent1 = textPlayer1->AddComponent<dae::TextComponent>("Use the d-pad to move player 1", MAIN_FONT);
	textComponent1->SetSize(18);
	textComponent1->SetColor({ 255, 255, 255, 255 });
	scene.Add(std::move(textPlayer1));

	auto textPlayer2 = std::make_unique<dae::GameObject>();
	textPlayer2->SetPosition(20, 150);
	auto textComponent2 = textPlayer2->AddComponent<dae::TextComponent>("Use WASD to move player 2", MAIN_FONT);
	textComponent2->SetSize(18);
	textComponent2->SetColor({ 255, 255, 255, 255 });
	scene.Add(std::move(textPlayer2));

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

	auto player2 = std::make_unique<dae::GameObject>();
	player2->SetPosition(200.f, 350.f);
	auto player2Actor = player2->AddComponent<dae::GameActor>();
	player2Actor->SetTexture("Sprites/PeterPepper/peter.png");
	player2Actor->SetSpeed(120.f);
	scene.Add(std::move(player2));

	SetInputMappingController(player1Actor, 0);
	SetInputMappingKeyboard(player2Actor);

	//// ****************************************************************************
	////							CHILD PARENT EXAMPLE
	//// ****************************************************************************
	//// 
	//// Location 
	//auto location = std::make_unique<dae::GameObject>();
	//location->SetPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT - SCREEN_HEIGHT / 3);
	//auto locationPtr = location.get();
	//scene.Add(std::move(location));
	//
	//// Player
	//auto player = std::make_unique<dae::GameObject>();
	//player->AddComponent<dae::RenderComponent>("Sprites/PeterPepper/peter.png");
	//player->AddComponent<dae::MovementComponent>(5.f,50.0f);
	//auto playerPtr = player.get();
	//scene.Add(std::move(player));
	//
	//// Orbiting satellite
	//auto satellite = std::make_unique<dae::GameObject>();
	//satellite->AddComponent<dae::RenderComponent>("Sprites/PeterPepper/peter.png");
	//satellite->AddComponent<dae::MovementComponent>(-10.0f, 30.0f);
	//auto satellitePtr = satellite.get();
	//scene.Add(std::move(satellite));
	//
	//// Setting parent
	//playerPtr->SetParent(locationPtr, true); // false = don't keep world position
	//satellitePtr->SetParent(playerPtr, true); // false = don't keep world position
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
