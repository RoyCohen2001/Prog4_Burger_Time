#pragma once
#include "Component.h"
#include "Transform.h"
#include "Subject.h"
#include <glm/glm.hpp>
#include "CollisionComponent.h"
#include "RenderComponent.h"
#include "MovementComponent.h"

namespace dae
{
	class GameActor final : public Component, public Subject
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
		void SetTexture(const std::string& texturePath) { 
			m_renderComponent->SetTexture(texturePath); 
			m_collisionComponent->SetSize(m_renderComponent->GetScaledSize());
		}

		void EnableGravity(bool enabled) { m_movementComponent->SetGravityEnabled(enabled); }
		void SetGroundY(float y) { m_movementComponent->SetGroundY(y); }

		void OnDeath();
		int GetLives() const { return m_lives; }

		void AddScore(int points);
		int GetScore() const { return m_score; }

	private:
		RenderComponent* m_renderComponent;
		MovementComponent* m_movementComponent;
		CollisionComponent* m_collisionComponent;

		int m_lives;
		int m_score;
	};
}