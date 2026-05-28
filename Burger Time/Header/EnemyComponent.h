#pragma once
#include "Component.h"
#include <glm/vec2.hpp>
#include <string>

namespace dae
{
	class RenderComponent;
	class MovementComponent;
	class CollisionComponent;
	class AnimationComponent;

	class EnemyComponent final : public Component
	{
	public:
		explicit EnemyComponent(GameObject* owner);

		void Update() override;

		void SetSpeed(float speed);
		void SetScale(float scale);
		void SetSpriteSheet(const std::string& path, const glm::vec2& frameSize, int columns, const glm::vec2& originOffset = { 0.f, 0.f });
		void SetAnimationClips(int walkLeftStart, int walkRightStart, int idleStart, int climbStart, int stunnedStart, int framesPerClip);
		void SetAnimationFps(float walkFps, float idleFps, float climbFps, float stunnedFps);

	private:
		enum class Mode { Horizontal, Climb };

		void EnsureComponents();
		void ChooseNewHorizontalDirection();
		bool CanExitLadder() const;
		void UpdateAnimation(bool onLadder);

		void ApplyAnimationSetup();
		void UpdateCollisionSize();

		RenderComponent* m_renderComponent{};
		MovementComponent* m_movementComponent{};
		CollisionComponent* m_collisionComponent{};
		AnimationComponent* m_animationComponent{};

		Mode m_mode{ Mode::Horizontal };
		glm::vec2 m_direction{ 1.f, 0.f };

		float m_decisionTimer{};
		float m_climbTimer{};
		bool m_climbUp{ true };

		float m_speed{ 80.f };
		bool m_isStunned{ false };

		// animation config
		std::string m_spritePath{ "Sprites/Enemies/dog.png" };
		glm::vec2 m_frameSize{ 15.f, 15.f };
		glm::vec2 m_originOffset{ 0.f, 0.f };
		int m_columns{ 2 };
		int m_framesPerClip{ 2 };

		int m_walkLeftStart{ 0 };
		int m_walkRightStart{ 2 };
		int m_idleStart{ 4 };
		int m_climbStart{ 6 };
		int m_stunnedStart{ 8 };

		float m_walkFps{ 6.f };
		float m_idleFps{ 3.f };
		float m_climbFps{ 6.f };
		float m_stunnedFps{ 3.f };

		float m_scale{ 2.f };
	};
}