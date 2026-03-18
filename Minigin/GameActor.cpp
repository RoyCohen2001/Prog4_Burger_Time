#include "GameActor.h"
#include "GameObject.h"
#include "Observer.h"

dae::GameActor::GameActor(GameObject* owner):
	Component(owner), m_lives(3), m_score(0)
{
	m_renderComponent = owner->AddComponent<RenderComponent>();
	m_movementComponent = owner->AddComponent<MovementComponent>();
}


void dae::GameActor::Move(const glm::vec2& direction)
{
	if (m_movementComponent)
	{
		m_movementComponent->Move(direction);
	}
}

void dae::GameActor::OnDeath()
{
	if (m_lives <= 0)
		return;

	m_lives -= 1;
	Notify("PlayerDied", GetOwner());
}

void dae::GameActor::AddScore(int points)
{
	m_score += points;
	Notify("ScoreChanged", GetOwner());
}
