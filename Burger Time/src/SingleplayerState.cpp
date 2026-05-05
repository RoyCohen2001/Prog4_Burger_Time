#include "SingleplayerState.h"
#include "SceneManager.h"
#include "Scene.h"
#include "LevelLoader.h"
#include "TextComponent.h"
#include "ResourceManager.h"
#include "GameActor.h"
#include "InputBindings.h"
#include "InputManager.h"
#include "Util.h"

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
	}

	void PlayerLoader(dae::Scene& scene, const HudRefs& hudRefs)
	{
		auto player1 = std::make_unique<dae::GameObject>();
		player1->SetPosition(SCREEN_WIDTH/2.f, SCREEN_HEIGHT - 160.f);
		auto player1Actor = player1->AddComponent<dae::GameActor>();
		player1Actor->SetTexture("Sprites/PeterPepper/peter.png");

		player1Actor->SetSpeed(100.f);
		player1Actor->EnableGravity(true);
		scene.Add(std::move(player1));

		player1Actor->AddObserver(hudRefs.p1Lives);
		player1Actor->AddObserver(hudRefs.p1Score);

		dae::SetInputMappingController(player1Actor, 0);
		dae::SetInputMappingKeyboard(player1Actor);
	}
}

void dae::SingleplayerState::OnEnter()
{
	InputManager::GetInstance().ClearBindings();
	SceneManager::GetInstance().ClearScenes();

	const auto mainFont = ResourceManager::GetInstance().LoadFont("BurgerTime.otf", 36);
	auto& scene = SceneManager::GetInstance().CreateScene();

	LevelLoader::LoadLevelFromJson("Data/Levels/level1.json", scene, glm::vec2{ -50, 50 });

	HudRefs hudRefs{};
	TextLoader(scene, mainFont, hudRefs);
	PlayerLoader(scene, hudRefs);
}