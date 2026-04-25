#include "GameLoader.h"

#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "LevelLoader.h"
#include "TextComponent.h"
#include "FPSComponent.h"
#include "GameActor.h"
#include "InputBindings.h"

#include <filesystem>

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
	const auto mainFont = ResourceManager::GetInstance().LoadFont("BurgerTime.otf", 36);
	auto& scene = SceneManager::GetInstance().CreateScene();

	LevelLoader::LoadLevelFromJson("Data/Levels/level1.json", scene, glm::vec2{ -50, 50 });

	auto textPlayer1 = std::make_unique<GameObject>();
	textPlayer1->SetPosition(70, 10);
	auto textComponent1 = textPlayer1->AddComponent<TextComponent>("Use the d-pad to move player 1 (A - damage) -- (B - pellets)", mainFont);
	textComponent1->SetSize(14);
	textComponent1->SetColor({ 255, 0, 0, 255 });
	scene.Add(std::move(textPlayer1));

	auto textPlayer1Lives = std::make_unique<GameObject>();
	textPlayer1Lives->SetPosition(70, 30);
	auto textLives1 = textPlayer1Lives->AddComponent<TextComponent>("Lives: ", mainFont);
	textLives1->SetSize(10);
	textLives1->SetColor({ 255, 255, 0, 255 });
	scene.Add(std::move(textPlayer1Lives));

	auto textPlayer1Score = std::make_unique<GameObject>();
	textPlayer1Score->SetPosition(70, 40);
	auto textScore1 = textPlayer1Score->AddComponent<TextComponent>("Score: ", mainFont);
	textScore1->SetSize(10);
	textScore1->SetColor({ 255, 255, 0, 255 });
	scene.Add(std::move(textPlayer1Score));

	auto textPlayer2 = std::make_unique<GameObject>();
	textPlayer2->SetPosition(70, 60);
	auto textComponent2 = textPlayer2->AddComponent<TextComponent>("Use WASD to move player 2 (C - damage) -- (X - pellets)", mainFont);
	textComponent2->SetSize(14);
	textComponent2->SetColor({ 255, 0, 0, 255 });
	scene.Add(std::move(textPlayer2));

	auto textPlayer2Lives = std::make_unique<GameObject>();
	textPlayer2Lives->SetPosition(70, 80);
	auto textLives2 = textPlayer2Lives->AddComponent<TextComponent>("Lives: ", mainFont, SDL_Color{ 255,255,0,255 }, TextComponent::DisplayType::Lives);
	textLives2->SetSize(10);
	textLives2->SetColor({ 255, 255, 0, 255 });
	scene.Add(std::move(textPlayer2Lives));

	auto textPlayer2Score = std::make_unique<GameObject>();
	textPlayer2Score->SetPosition(70, 90);
	auto textScore2 = textPlayer2Score->AddComponent<TextComponent>("Score: ", mainFont, SDL_Color{ 255,255,0,255 }, TextComponent::DisplayType::Score);
	textScore2->SetSize(10);
	textScore2->SetColor({ 255, 255, 0, 255 });
	scene.Add(std::move(textPlayer2Score));

	auto fpsObject = std::make_unique<GameObject>();
	fpsObject->SetPosition(10.f, 10.f);
	fpsObject->AddComponent<FPSComponent>();
	scene.Add(std::move(fpsObject));

	auto player1 = std::make_unique<GameObject>();
	player1->SetPosition(200.f, 300.f);
	auto player1Actor = player1->AddComponent<GameActor>();
	player1Actor->SetTexture("Sprites/PeterPepper/peter.png");
	player1Actor->SetSpeed(60.f);
	scene.Add(std::move(player1));

	player1Actor->AddObserver(textLives1);
	player1Actor->AddObserver(textScore1);

	auto player2 = std::make_unique<GameObject>();
	player2->SetPosition(200.f, 350.f);
	auto player2Actor = player2->AddComponent<GameActor>();
	player2Actor->SetTexture("Sprites/PeterPepper/peter.png");
	player2Actor->SetSpeed(120.f);
	scene.Add(std::move(player2));

	player2Actor->AddObserver(textLives2);
	player2Actor->AddObserver(textScore2);

	SetInputMappingController(player1Actor, 0);
	SetInputMappingKeyboard(player2Actor);

#if USE_STEAMWORKS
#include "SteamManager.h"
	player2Actor->AddObserver(&SteamManager::GetInstance());
#endif
}