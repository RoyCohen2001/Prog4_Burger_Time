#include "CoopState.h"
#include "SceneManager.h"
#include "Scene.h"
#include "LevelLoader.h"
#include "TextComponent.h"
#include "ResourceManager.h"
#include "GameActor.h"
#include "InputBindings.h"
#include "InputManager.h"
#include "Util.h"
#include "HudObservers.h"

namespace
{
	struct HudRefs final
	{
		// both
		dae::ScoreObserverComponent* score{};
		dae::LivesObserverComponent* lives{};

		// Ammo
		// player 1
		//dae::PepperObserverComponent* p1Ammo{};

		// player 2
		//dae::PepperObserverComponent* p2Ammo{};
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
		oneUpText->SetColor({ 0, 0, 255, 255 });
		scene.Add(std::move(oneUpLabel));

		auto hiScoreLabel = std::make_unique<dae::GameObject>();
		hiScoreLabel->SetPosition(110.f, 10.f);
		auto hiScoreText = hiScoreLabel->AddComponent<dae::TextComponent>("HI-SCORE", mainFont);
		hiScoreText->SetSize(TitleSize);
		hiScoreText->SetColor({ 0, 0, 255, 255 });
		scene.Add(std::move(hiScoreLabel));

		auto pepperLabelP1 = std::make_unique<dae::GameObject>();
		pepperLabelP1->SetPosition(SCREEN_WIDTH - 240.f, 10.f);
		auto pepperTextP1 = pepperLabelP1->AddComponent<dae::TextComponent>("PEPPER P1", mainFont);
		pepperTextP1->SetSize(TitleSize - 10);
		pepperTextP1->SetColor({ 0, 255, 0, 255 });
		scene.Add(std::move(pepperLabelP1));

		auto pepperLabelP2 = std::make_unique<dae::GameObject>();
		pepperLabelP2->SetPosition(SCREEN_WIDTH - 120.f, 10.f);
		auto pepperTextP2 = pepperLabelP2->AddComponent<dae::TextComponent>("PEPPER P2", mainFont);
		pepperTextP2->SetSize(TitleSize - 10);
		pepperTextP2->SetColor({ 255, 0, 0, 255 });
		scene.Add(std::move(pepperLabelP2));

		auto oneUpValue = std::make_unique<dae::GameObject>();
		oneUpValue->SetPosition(20.f, padding);
		auto oneUpValueText = oneUpValue->AddComponent<dae::TextComponent>("000", mainFont);
		oneUpValueText->SetSize(scoreSize);
		oneUpValueText->SetColor({ 255, 255, 0, 255 });
		auto p1LivesObserver = oneUpValue->AddComponent<dae::LivesObserverComponent>(oneUpValueText);
		scene.Add(std::move(oneUpValue));

		auto hiScoreValue = std::make_unique<dae::GameObject>();
		hiScoreValue->SetPosition(110.f, padding);
		auto hiScoreValueText = hiScoreValue->AddComponent<dae::TextComponent>("000", mainFont);
		hiScoreValueText->SetSize(scoreSize);
		hiScoreValueText->SetColor({ 255, 255, 0, 255 });
		auto hiScoreObserver = hiScoreValue->AddComponent<dae::ScoreObserverComponent>(hiScoreValueText);
		scene.Add(std::move(hiScoreValue));

		auto pepperValueP1 = std::make_unique<dae::GameObject>();
		pepperValueP1->SetPosition(SCREEN_WIDTH - 210.f, padding);
		auto pepperValueTextP1 = pepperValueP1->AddComponent<dae::TextComponent>("000", mainFont);
		pepperValueTextP1->SetSize(scoreSize);
		pepperValueTextP1->SetColor({ 255, 255, 0, 255 });
		// add observer for pepper 2 ammo here
		scene.Add(std::move(pepperValueP1));

		auto pepperValueP2 = std::make_unique<dae::GameObject>();
		pepperValueP2->SetPosition(SCREEN_WIDTH - 90.f, padding);
		auto pepperValueTextP2 = pepperValueP2->AddComponent<dae::TextComponent>("000", mainFont);
		pepperValueTextP2->SetSize(scoreSize);
		pepperValueTextP2->SetColor({ 255, 255, 0, 255 });
		// add observer for pepper 1 ammo here
		scene.Add(std::move(pepperValueP2));

		hudRefs.lives = p1LivesObserver;
		hudRefs.score = hiScoreObserver;
	}

	void PlayerLoader(dae::Scene& scene, const HudRefs& hudRefs)
	{
		float speed = 100.f;

		auto player1 = std::make_unique<dae::GameObject>();
		auto* player1Go = player1.get();
		player1->SetPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT - 160.f);
		auto player1Actor = player1->AddComponent<dae::GameActor>();
		player1Actor->SetTexture("Sprites/PeterPepper/peter.png");
		player1Actor->SetSpeed(speed);
		player1Actor->EnableGravity(true);
		scene.Add(std::move(player1));

		player1Actor->AddObserver(hudRefs.lives);
		player1Actor->AddObserver(hudRefs.score);

		auto player2 = std::make_unique<dae::GameObject>();
		auto* player2Go = player2.get();
		player2->SetPosition(SCREEN_WIDTH / 2.f - 50.f, SCREEN_HEIGHT - 160.f);
		auto player2Actor = player2->AddComponent<dae::GameActor>();
		player2Actor->SetTexture("Sprites/PeterPepper/peter.png");
		player2Actor->SetSpeed(speed);
		player2Actor->EnableGravity(true);
		scene.Add(std::move(player2));

		player2Actor->AddObserver(hudRefs.lives);
		player2Actor->AddObserver(hudRefs.score);

		// Indicator for player 1
		auto p1Indicator = std::make_unique<dae::GameObject>();
		auto* p1IndicatorRender = p1Indicator->AddComponent<dae::RenderComponent>();
		p1IndicatorRender->SetTexture("Player1Indicator.png");
		p1IndicatorRender->SetScale(0.02f);
		p1Indicator->SetParent(player1Go, true);
		p1Indicator->SetLocalPosition({ 10.f, -20.f, 0.f });
		scene.Add(std::move(p1Indicator));

		// Indicator for player 2
		auto p2Indicator = std::make_unique<dae::GameObject>();
		auto* p2IndicatorRender = p2Indicator->AddComponent<dae::RenderComponent>();
		p2IndicatorRender->SetTexture("Player2Indicator.png");
		p2IndicatorRender->SetScale(0.01f);
		p2Indicator->SetParent(player2Go, true);
		p2Indicator->SetLocalPosition({ 10.f, -20.f, 0.f});
		scene.Add(std::move(p2Indicator));

		dae::SetInputMappingController(player1Actor, 0);
		dae::SetInputMappingKeyboard(player1Actor);

		dae::SetInputMappingController(player2Actor, 1);
	}
}

void dae::CoopState::OnEnter()
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