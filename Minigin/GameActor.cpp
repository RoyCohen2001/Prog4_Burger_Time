#include "GameActor.h"
#include "GameObject.h"

dae::GameActor::GameActor(GameObject* owner):
	Component(owner)
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
