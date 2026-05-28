#include "SingleplayerState.h"
#include "SceneManager.h"
#include "Scene.h"
#include "TextComponent.h"
#include "GameActor.h"
#include "InputBindings.h"
#include "InputManager.h"
#include "Util.h"
#include "HudObservers.h"
#include "LevelLoader.h"
#include "ResourceManager.h"
#include "EnemyComponent.h"


namespace
{
	struct HudRefs final
	{
		dae::LivesObserverComponent* p1Lives{};
		dae::ScoreObserverComponent* p1Score{};
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
		auto LivesObserver = oneUpValue->AddComponent<dae::LivesObserverComponent>(oneUpValueText);
		scene.Add(std::move(oneUpValue));

		auto hiScoreValue = std::make_unique<dae::GameObject>();
		hiScoreValue->SetPosition(110.f, padding);
		auto hiScoreValueText = hiScoreValue->AddComponent<dae::TextComponent>("000", mainFont);
		hiScoreValueText->SetSize(scoreSize);
		hiScoreValueText->SetColor({ 255, 255, 0, 255 });
		auto hiScoreScoreObserver = hiScoreValue->AddComponent<dae::ScoreObserverComponent>(hiScoreValueText);
		scene.Add(std::move(hiScoreValue));

		auto pepperValue = std::make_unique<dae::GameObject>();
		pepperValue->SetPosition(SCREEN_WIDTH - 150.f, padding);
		auto pepperValueText = pepperValue->AddComponent<dae::TextComponent>("000", mainFont);
		pepperValueText->SetSize(scoreSize);
		pepperValueText->SetColor({ 255, 255, 0, 255 });
		scene.Add(std::move(pepperValue));

		hudRefs.p1Lives = LivesObserver;
		hudRefs.p1Score = hiScoreScoreObserver;
	}

	void PlayerLoader(dae::Scene& scene, const HudRefs& hudRefs)
	{
		auto player1 = std::make_unique<dae::GameObject>();
		player1->SetPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT - 160.f);
		auto player1Actor = player1->AddComponent<dae::GameActor>();
		//player1Actor->SetTexture("Sprites/PeterPepper/peter.png");
		player1Actor->SetupAnimations("Sprites/PeterPepper/sprites.png", { 15.f, 15.f }, { 0.f, 0.f });
		player1Actor->SetSpeed(100.f);
		player1Actor->EnableGravity(true);
		scene.Add(std::move(player1));

		player1Actor->AddObserver(hudRefs.p1Lives);
		player1Actor->AddObserver(hudRefs.p1Score);

		dae::SetInputMappingController(player1Actor, 0);
		dae::SetInputMappingKeyboard(player1Actor);
	}

	void SpawnEnemies(dae::Scene& scene)
	{
		const auto bounds = dae::LevelLoader::GetLevelBounds();
		if (bounds.z <= bounds.x || bounds.w <= bounds.y)
			return;

		const float leftX = bounds.x;
		const float rightX = bounds.z;
		const float topLeftY = dae::LevelLoader::GetHighestGroundYAt(leftX + 1.f);
		const float topRightY = dae::LevelLoader::GetHighestGroundYAt(rightX - 1.f);
		const float bottomLeftY = dae::LevelLoader::GetLowestGroundYAt(leftX + 1.f);
		const float bottomRightY = dae::LevelLoader::GetLowestGroundYAt(rightX - 1.f);

		auto spawnEnemyAt = [&](float x, float y, bool alignRight)
			{
				if (!std::isfinite(y))
					return;

				auto enemy = std::make_unique<dae::GameObject>();
				enemy->SetPosition(x, y);

				enemy->AddComponent<dae::EnemyComponent>();
				auto* collision = enemy->GetComponent<dae::CollisionComponent>();
				const auto size = collision->GetSize();

				const float spawnX = alignRight ? (x - size.x) : x;
				enemy->SetPosition(spawnX, y - size.y);

				scene.Add(std::move(enemy));
			};

		spawnEnemyAt(leftX, topLeftY, false);
		spawnEnemyAt(rightX, topRightY, true);
		spawnEnemyAt(leftX, bottomLeftY, false);
		spawnEnemyAt(rightX, bottomRightY, true);
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
	SpawnEnemies(scene);
}