#include "MovementComponent.h"
#include "GameObject.h"
#include "TimeManager.h"

#include <glm/gtc/constants.hpp> 
#include <glm/common.hpp>         

#ifndef __EMSCRIPTEN__
#include <Windows.h>
#include <sstream>
#endif

namespace dae
{
	MovementComponent::MovementComponent(GameObject* owner, float speed, float radius)
		: Component(owner), m_speed(speed), m_radius(radius)
	{
		if (GetOwner()->GetParent() == nullptr)
        {
            auto worldPos = GetOwner()->GetWorldPosition();
            m_center = glm::vec2{ worldPos.x, worldPos.y };
        }
	}

	void MovementComponent::Update()
	{
        const float dt = TimeManager::GetInstance().GetDeltaTime();

        // Update angle
        m_angle += m_speed * dt;

        // constexpr 2π to keep static analyzer happy
        constexpr float twoPi = glm::two_pi<float>();

        // Wrap angle into [0, 2π)
        if (m_angle >= twoPi || m_angle <= -twoPi)
        {
            m_angle = std::fmod(m_angle, twoPi);
            if (m_angle < 0.0f)
                m_angle += twoPi;
        }

        const float x = m_center.x + m_radius * glm::cos(m_angle);
        const float y = m_center.y + m_radius * glm::sin(m_angle);

        #ifndef __EMSCRIPTEN__
        std::ostringstream oss;
        oss << " angle=" << m_angle
            << " pos=(" << x << ", " << y << ")\n";
        OutputDebugStringA(oss.str().c_str());
        #endif

        GetOwner()->SetLocalPosition(glm::vec3{ x, y, 0.0f });
   
	}

	void MovementComponent::Move(const glm::vec2& direction)
	{
		m_velocity = direction;
	}
}