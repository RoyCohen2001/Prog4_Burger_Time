#include "HudObservers.h"
#include "TextComponent.h"
#include "GameObject.h"
#include "GameActor.h"
#include "GameStateManager.h"
#include "MenuState.h"

dae::LivesObserverComponent::LivesObserverComponent(GameObject* owner, TextComponent* text)
	: Component(owner), m_text(text)
{}

void dae::LivesObserverComponent::OnNotify(const std::string& event, GameObject* gameObject)
{
	if (!m_text || event != "PlayerDied" || !gameObject)
		return;

	if (auto* actor = gameObject->GetComponent<GameActor>())
	{
		m_text->SetText(std::to_string(actor->GetLives()));

		if (actor->GetLives() <= 0)
		{
			GameStateManager::GetInstance().ChangeState(std::make_unique<MenuState>());
		}
	}
}

dae::ScoreObserverComponent::ScoreObserverComponent(GameObject* owner, TextComponent* text)
	: Component(owner), m_text(text)
{}

void dae::ScoreObserverComponent::OnNotify(const std::string& event, GameObject* gameObject)
{
	if (!m_text || event != "ScoreChanged" || !gameObject)
		return;

	if (auto* actor = gameObject->GetComponent<GameActor>())
	{
		m_text->SetText(std::to_string(actor->GetScore()));
	}
}