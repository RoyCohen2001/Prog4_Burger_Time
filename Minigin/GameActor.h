#pragma once
#include "Component.h"
#include "RenderComponent.h"
#include "MovementComponent.h"
#include "Transform.h"
#include "Subject.h"
#include <glm/glm.hpp>

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
		void SetTexture(const std::string& texturePath) { m_renderComponent->SetTexture(texturePath); }

		void OnDeath();
		int GetLives() const { return m_lives; }

		void AddScore(int points);
		int GetScore() const { return m_score; }

	private:		
		RenderComponent* m_renderComponent;
		MovementComponent* m_movementComponent;

		int m_lives;
		int m_score;
	};
}