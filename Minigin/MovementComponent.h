#pragma once
#include "Component.h"
#include <glm/glm.hpp>

namespace dae
{
	class MovementComponent final : public Component
	{
	public:
		MovementComponent(GameObject* owner, float speed = 100.0f, float radius = 50.0f);

		void Update() override;
		void Move(const glm::vec2& direction);

		void SetSpeed(float speed) { m_speed = speed; }
		float GetSpeed() const { return m_speed; }
		void SetRadius(float radius) { m_radius = radius; }
		void SetOrbitCenter(const glm::vec2& center) { m_center = center; }

	private:
		float m_speed;
		glm::vec2 m_velocity{ 0.0f, 0.0f };

		// Circular motion
		float m_angle{ 0.0f };
		float m_radius;
		glm::vec2 m_center{ 0.0f, 0.0f }; // Relative center (0,0 for children orbiting parent)
	};
}