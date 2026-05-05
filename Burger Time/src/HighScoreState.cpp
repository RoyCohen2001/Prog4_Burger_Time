#include "HighscoreState.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "InputBindings.h"

void dae::HighscoreState::OnEnter()
{
	InputManager::GetInstance().ClearBindings();
	SceneManager::GetInstance().ClearScenes();

	auto& scene = SceneManager::GetInstance().CreateScene();
	const auto font = ResourceManager::GetInstance().LoadFont("BurgerTime.otf", 36);

	auto title = std::make_unique<GameObject>();
	title->SetPosition(140.f, 120.f);
	auto* titleText = title->AddComponent<TextComponent>("HIGHSCORES", font);
	titleText->SetSize(36);
	scene.Add(std::move(title));

	auto line1 = std::make_unique<GameObject>();
	line1->SetPosition(160.f, 200.f);
	auto* line1Text = line1->AddComponent<TextComponent>("00000", font);
	line1Text->SetSize(24);
	scene.Add(std::move(line1));

	auto line2 = std::make_unique<GameObject>();
	line2->SetPosition(160.f, 240.f);
	auto* line2Text = line2->AddComponent<TextComponent>("00000", font);
	line2Text->SetSize(24);
	scene.Add(std::move(line2));

	auto line3 = std::make_unique<GameObject>();
	line3->SetPosition(160.f, 280.f);
	auto* line3Text = line3->AddComponent<TextComponent>("00000", font);
	line3Text->SetSize(24);
	scene.Add(std::move(line3));

	SetBackToMenuInputMappingKeyboard();
	SetBackToMenuInputMappingController(0);
}