#pragma once
#include "Component.h"
#include <glm/glm.hpp>

namespace dae
{
	class CollisionComponent;

	class MovementComponent final : public Component
	{
	public:
		MovementComponent(GameObject* owner, float speed = 100.0f, float radius = 50.0f);

		void Update() override;
		void Move(const glm::vec2& direction);
		void Stop() { m_velocity = glm::vec2{ 0.0f, 0.0f }; }

		void GoInCircles();

		void SetSpeed(float speed) { m_speed = speed; }
		float GetSpeed() const { return m_speed; }
		void SetRadius(float radius) { m_radius = radius; }

		void SetGravityEnabled(bool enabled) { m_useGravity = enabled; }
		void SetGravity(float gravity) { m_gravity = gravity; }
		void SetMaxFallSpeed(float maxFallSpeed) { m_maxFallSpeed = maxFallSpeed; }
		void SetGroundY(float groundY) { m_groundY = groundY; }
		void SetGrounded(bool grounded);

	private:
		CollisionComponent* m_pCollisionComponent{ nullptr };

		float m_speed;
		glm::vec2 m_velocity{ 0.0f, 0.0f };

		// Gravity
		bool m_useGravity{ false };
		bool m_isGrounded{ false };
		float m_gravity{ 1200.0f };
		float m_fallSpeed{ 0.0f };
		float m_maxFallSpeed{ 900.0f };
		float m_groundY{ 10000.0f };

		// Circular motion
		float m_angle{ 0.0f };
		float m_radius;
	};
}