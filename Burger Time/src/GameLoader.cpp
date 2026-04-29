#include "GameLoader.h"

#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "LevelLoader.h"
#include "TextComponent.h"
#include "FPSComponent.h"
#include "GameActor.h"
#include "InputBindings.h"
#include "ServiceLocator.h"
#include <filesystem>
#include "Util.h"

#if USE_STEAMWORKS
#include "SteamManager.h"
#endif

namespace fs = std::filesystem;

namespace
{
	struct HudRefs final
	{
		dae::TextComponent* p1Lives{};
		dae::TextComponent* p1Score{};
		dae::TextComponent* p2Lives{};
		dae::TextComponent* p2Score{};
	};

	void TextLoader(dae::Scene& scene, const std::shared_ptr<dae::Font>& mainFont, HudRefs& hudRefs)
	{
		int TitleSize = 25;
		int scoreSize = 20;

		float padding = 40.f;

		// Top row labels (left side)
		auto oneUpLabel = std::make_unique<dae::GameObject>();
		oneUpLabel->SetPosition(20.f, 10.f);
		auto oneUpText = oneUpLabel->AddComponent<dae::TextComponent>("1UP", mainFont);
		oneUpText->SetSize(TitleSize);
		oneUpText->SetColor({ 255, 0, 0, 255 });
		scene.Add(std::move(oneUpLabel));

		auto hiScoreLabel = std::make_unique<dae::GameObject>();
		hiScoreLabel->SetPosition(110.f, 10.f);
		auto hiScoreText = hiScoreLabel->AddComponent<dae::TextComponent>("HI-SCORE", mainFont);
		hiScoreText->SetSize(TitleSize);
		hiScoreText->SetColor({ 255, 0, 0, 255 });
		scene.Add(std::move(hiScoreLabel));

		auto pepperLabel = std::make_unique<dae::GameObject>();
		pepperLabel->SetPosition(SCREEN_WIDTH - 150.f, 10.f);
		auto pepperText = pepperLabel->AddComponent<dae::TextComponent>("PEPPER", mainFont);
		pepperText->SetSize(TitleSize);
		pepperText->SetColor({ 0, 255, 0, 255 });
		scene.Add(std::move(pepperLabel));

		// Second row values
		auto oneUpValue = std::make_unique<dae::GameObject>();
		oneUpValue->SetPosition(20.f, padding);
		auto oneUpValueText = oneUpValue->AddComponent<dae::TextComponent>("000", mainFont);
		oneUpValueText->SetSize(scoreSize);
		oneUpValueText->SetColor({ 255, 255, 0, 255 });
		scene.Add(std::move(oneUpValue));

		auto hiScoreValue = std::make_unique<dae::GameObject>();
		hiScoreValue->SetPosition(110.f, padding);
		auto hiScoreValueText = hiScoreValue->AddComponent<dae::TextComponent>("000", mainFont);
		hiScoreValueText->SetSize(scoreSize);
		hiScoreValueText->SetColor({ 255, 255, 0, 255 });
		scene.Add(std::move(hiScoreValue));

		auto pepperValue = std::make_unique<dae::GameObject>();
		pepperValue->SetPosition(SCREEN_WIDTH - 150.f, padding);
		auto pepperValueText = pepperValue->AddComponent<dae::TextComponent>("000", mainFont);
		pepperValueText->SetSize(scoreSize);
		pepperValueText->SetColor({ 255, 255, 0, 255 });
		scene.Add(std::move(pepperValue));

		hudRefs.p1Lives = oneUpValueText;
		hudRefs.p1Score = oneUpValueText;
		hudRefs.p2Lives = hiScoreValueText;
		hudRefs.p2Score = hiScoreValueText;

		//auto fpsObject = std::make_unique<dae::GameObject>();
		//fpsObject->SetPosition(10.f, 10.f);
		//fpsObject->AddComponent<dae::FPSComponent>();
		//scene.Add(std::move(fpsObject));
	}

	void PlayerLoader(dae::Scene& scene, const HudRefs& hudRefs)
	{
		auto player1 = std::make_unique<dae::GameObject>();
		player1->SetPosition(200.f, 300.f);
		auto player1Actor = player1->AddComponent<dae::GameActor>();
		player1Actor->SetTexture("Sprites/PeterPepper/peter.png");
		
		player1Actor->SetSpeed(60.f);
		player1Actor->EnableGravity(true);
		scene.Add(std::move(player1));

		player1Actor->AddObserver(hudRefs.p1Lives);
		player1Actor->AddObserver(hudRefs.p1Score);

		auto player2 = std::make_unique<dae::GameObject>();
		player2->SetPosition(200.f, 350.f);
		auto player2Actor = player2->AddComponent<dae::GameActor>();
		player2Actor->SetTexture("Sprites/PeterPepper/peter.png");
		player2Actor->SetSpeed(120.f);
		player2Actor->EnableGravity(true);
		scene.Add(std::move(player2));

		player2Actor->AddObserver(hudRefs.p2Lives);
		player2Actor->AddObserver(hudRefs.p2Score);

		dae::SetInputMappingController(player1Actor, 0);
		dae::SetInputMappingKeyboard(player2Actor);

#if USE_STEAMWORKS
		player2Actor->AddObserver(&dae::SteamManager::GetInstance());
#endif
	}
}

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

	const auto mainFont = ResourceManager::GetInstance().LoadFont("BurgerTime.otf", 36);
	auto& scene = SceneManager::GetInstance().CreateScene();

	LevelLoader::LoadLevelFromJson("Data/Levels/level1.json", scene, glm::vec2{ -50, 50 });

	HudRefs hudRefs{};
	TextLoader(scene, mainFont, hudRefs);
	PlayerLoader(scene, hudRefs);
}