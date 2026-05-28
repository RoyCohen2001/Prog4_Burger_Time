#pragma once
#include "Component.h"
#include "Transform.h"
#include "Subject.h"
#include <glm/glm.hpp>
#include "CollisionComponent.h"
#include "RenderComponent.h"
#include "MovementComponent.h"
#include "PlayerState.h"
#include <memory>

namespace dae
{
	class AnimationComponent;

	class GameActor final : public Component, public Subject
	{
	public:
		GameActor(GameObject* owner);

		virtual ~GameActor() = default;
		GameActor(const GameActor& other) = delete;
		GameActor(GameActor&& other) = delete;
		GameActor& operator=(const GameActor& other) = delete;
		GameActor& operator=(GameActor&& other) = delete;

		void Update() override;
		void Move(const glm::vec2& direction);

		void SetSpeed(float speed) { m_movementComponent->SetSpeed(speed); }
		void SetTexture(const std::string& texturePath) {
			m_renderComponent->SetTexture(texturePath);
			m_collisionComponent->SetSize(m_renderComponent->GetScaledSize());
		}

		void SetupAnimations(const std::string& spriteSheetPath, const glm::vec2& frameSize, const glm::vec2& originOffset);

		void EnableGravity(bool enabled) { m_movementComponent->SetGravityEnabled(enabled); }
		void SetGroundY(float y) { m_movementComponent->SetGroundY(y); }

		void OnDeath();
		int GetLives() const { return m_lives; }

		void AddScore(int points);
		int GetScore() const { return m_score; }

		void ChangeState(std::unique_ptr<PlayerState> state);
		const glm::vec2& GetLastMoveDir() const { return m_lastMoveDir; }
		bool HasMoveInput() const { return m_hasMoveInput; }
		void PlayAnimation(const std::string& name);

		MovementComponent* GetMovementComponent() const { return m_movementComponent; }
		bool IsOnLadder() const;

		void Shoot();
		bool HasShootInput() const { return m_hasShootInput; }

	private:
		void Respawn();

		RenderComponent* m_renderComponent{};
		MovementComponent* m_movementComponent{};
		CollisionComponent* m_collisionComponent{};
		AnimationComponent* m_animationComponent{};

		std::unique_ptr<PlayerState> m_state{};

		glm::vec2 m_lastMoveDir{ 0.f, 0.f };
		bool m_hasMoveInput{ false };
		bool m_hasShootInput{ false };

		glm::vec3 m_spawnPosition{ 0.f, 0.f, 0.f };

		int m_lives{};
		int m_score{};
	};
}