#include "MenuState.h"
#include <SDL3/SDL.h>
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "RenderComponent.h"
#include "Util.h"
#include "GameStateManager.h"
#include "InputBindings.h"


#include "SingleplayerState.h"
#include "CoopState.h"
#include "VersusState.h"
#include "HighscoreState.h"

void dae::MenuState::OnEnter()
{
	const float OPTIONS_X = 160.f;
	const float ARR_OPTIONS_Y[] = { 350.f, 390.f, 430.f, 470.f };

	InputManager::GetInstance().ClearBindings();
	SceneManager::GetInstance().ClearScenes();

	auto& scene = SceneManager::GetInstance().CreateScene();
	const auto font = ResourceManager::GetInstance().LoadFont("BurgerTime.otf", 36);

	auto logoObject = std::make_unique<GameObject>();
	auto* logoRender = logoObject->AddComponent<RenderComponent>();
	logoRender->SetTexture("BurgerTimeLogo.png");
	logoRender->SetScale(0.4f);
	const auto logoSize = logoRender->GetScaledSize();
	const float logoX = (SCREEN_WIDTH - logoSize.x) * 0.5f;
	const float logoY = 60.f;
	logoObject->SetPosition(logoX, logoY);
	scene.Add(std::move(logoObject));

	auto optionsTitle = std::make_unique<GameObject>();
	optionsTitle->SetPosition(OPTIONS_X, ARR_OPTIONS_Y[0]);
	auto* optionsTitleText = optionsTitle->AddComponent<TextComponent>("SINGLE PLAYER", font);
	optionsTitleText->SetSize(28);
	scene.Add(std::move(optionsTitle));

	auto coop = std::make_unique<GameObject>();
	coop->SetPosition(OPTIONS_X, ARR_OPTIONS_Y[1]);
	auto* coopText = coop->AddComponent<TextComponent>("COOP", font);
	coopText->SetSize(28);
	scene.Add(std::move(coop));

	auto versus = std::make_unique<GameObject>();
	versus->SetPosition(OPTIONS_X, ARR_OPTIONS_Y[2]);
	auto* versusText = versus->AddComponent<TextComponent>("VERSUS", font);
	versusText->SetSize(28);
	scene.Add(std::move(versus));

	auto highscores = std::make_unique<GameObject>();
	highscores->SetPosition(OPTIONS_X, ARR_OPTIONS_Y[3]);
	auto* highscoresText = highscores->AddComponent<TextComponent>("HIGHSCORES", font);
	highscoresText->SetSize(28);
	scene.Add(std::move(highscores));

	const float selectorOffsetY = 10.f;
	m_optionPositions = {
		glm::vec2{ OPTIONS_X,  ARR_OPTIONS_Y[0] - selectorOffsetY },
		glm::vec2{ OPTIONS_X,  ARR_OPTIONS_Y[1] - selectorOffsetY },
		glm::vec2{ OPTIONS_X,  ARR_OPTIONS_Y[2] - selectorOffsetY },
		glm::vec2{ OPTIONS_X,  ARR_OPTIONS_Y[3] - selectorOffsetY }
	};
	m_selectedIndex = 0;

	auto selectorObject = std::make_unique<GameObject>();
	auto* selectorRender = selectorObject->AddComponent<RenderComponent>();
	selectorRender->SetTexture("burgertimeselector.png");
	selectorRender->SetScale(0.2f);
	m_selectorSize = selectorRender->GetScaledSize();
	m_selectorX = OPTIONS_X - m_selectorSize.x - 20.f;
	m_selectorObject = selectorObject.get();
	UpdateSelectorPosition();
	scene.Add(std::move(selectorObject));

	auto credit = std::make_unique<GameObject>();
	credit->SetPosition(150.f, SCREEN_HEIGHT - 40.f);
	auto* creditText = credit->AddComponent<TextComponent>("Made by Roy Cohen", font);
	creditText->SetSize(18);
	scene.Add(std::move(credit));

	SetMenuInputMappingController(this, 0);
	SetMenuInputMappingKeyboard(this);
}

void dae::MenuState::MoveSelection(int delta)
{
	const int count = static_cast<int>(m_optionPositions.size());
	m_selectedIndex = (m_selectedIndex + delta + count) % count;
	UpdateSelectorPosition();
}

void dae::MenuState::SelectCurrent()
{
	switch (m_selectedIndex)
	{
	case 0:
		GameStateManager::GetInstance().ChangeState(std::make_unique<SingleplayerState>());
		break;
	case 1:
		GameStateManager::GetInstance().ChangeState(std::make_unique<CoopState>());
		break;
	case 2:
		GameStateManager::GetInstance().ChangeState(std::make_unique<VersusState>());
		break;
	case 3:
		GameStateManager::GetInstance().ChangeState(std::make_unique<HighscoreState>());
		break;
	default:
		break;
	}
}

void dae::MenuState::UpdateSelectorPosition()
{
	if (!m_selectorObject)
	{
		return;
	}

	const auto& pos = m_optionPositions[m_selectedIndex];
	m_selectorObject->SetPosition(m_selectorX, pos.y);
}