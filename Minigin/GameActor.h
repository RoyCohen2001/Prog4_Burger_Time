#pragma once
#include "Component.h"
#include "RenderComponent.h"
#include "MovementComponent.h"
#include "Transform.h"
#include <glm/glm.hpp>

namespace dae
{
	class GameActor final : public Component
	{
	public:
		GameActor(GameObject* owner);

		virtual ~GameActor() = default;
		GameActor(const GameActor& other) = delete;
		GameActor(GameActor&& other) = delete;
		GameActor& operator=(const GameActor& other) = delete;
		GameActor& operator=(GameActor&& other) = delete;
		
		void Move(const glm::vec2& direction);

		void SetSpeed(float speed) { m_movementComponent->SetSpeed(speed); }
		void SetTexture(const std::string& texturePath) { m_renderComponent->SetTexture(texturePath); }

	private:
		RenderComponent* m_renderComponent;
		MovementComponent* m_movementComponent;
	};
}