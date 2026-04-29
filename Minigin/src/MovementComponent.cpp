#include "MovementComponent.h"
#include "GameObject.h"
#include "TimeManager.h"
#include "CollisionComponent.h"

#include <glm/gtc/constants.hpp>
#include <glm/common.hpp>
#include <algorithm>
#include <limits>

namespace dae
{
	MovementComponent::MovementComponent(GameObject* owner, float speed, float radius)
		: Component(owner), m_speed(speed), m_radius(radius)
	{}

	void MovementComponent::Update()
	{
		const float dt = TimeManager::GetInstance().GetDeltaTime();
		auto localPos = GetOwner()->GetTransform().GetLocalPosition();

		if (!m_pCollisionComponent)
		{
			m_pCollisionComponent = GetOwner()->GetComponent<CollisionComponent>();
		}

		auto* collision = m_pCollisionComponent;

		bool fullyInLadder = false;
		bool canExitLadder = false;

		if (collision)
		{
			const auto& myAABB = collision->GetAABB();

			for (auto* other : CollisionComponent::GetAllColliders())
			{
				if (other == collision) continue;

				if (other->GetLayer() == CollisionLayer::Ladder)
				{
					const auto& lAABB = other->GetAABB();

					//extend ladder a bit upwards so we can reach the top 
					const float ladderTopExtended = lAABB.top - 15.0f;

					bool intersects = !(myAABB.right <= lAABB.left || myAABB.left >= lAABB.right ||
						myAABB.bottom <= ladderTopExtended || myAABB.top >= lAABB.bottom);

					if (intersects)
					{
						if (myAABB.left >= lAABB.left - 1.f && myAABB.right <= lAABB.right + 1.f)
						{
							fullyInLadder = true;
							break;
						}
					}
				}
			}

			// If inside a ladder, check if we're alongside a platform to permit horizontally exiting
			if (fullyInLadder)
			{
				const float feetY = collision->GetFeetY();
				for (auto* other : CollisionComponent::GetAllColliders())
				{
					if (other == collision) continue;

					if (other->GetLayer() == CollisionLayer::Platform)
					{
						const auto& pAABB = other->GetAABB();

						if (myAABB.right > pAABB.left && myAABB.left < pAABB.right)
						{
							if (std::abs(feetY - pAABB.top) <= 8.0f)
							{
								canExitLadder = true;
								break;
							}
						}
					}
				}
			}
		}

		const bool hasVerticalInput = (m_velocity.y != 0.0f);

		float deltaX = m_velocity.x * m_speed * dt;
		float deltaY = 0.0f;

		// Prevent leaving the ladder horizontally if there's no platform to walk onto
		if (fullyInLadder && !canExitLadder)
		{
			deltaX = 0.0f;
		}

		if (hasVerticalInput && fullyInLadder)
		{
			deltaY = m_velocity.y * m_speed * dt;
			m_fallSpeed = 0.0f;
			m_isGrounded = false;
		}
		else if (m_useGravity && !fullyInLadder)
		{
			m_fallSpeed = std::min(m_fallSpeed + m_gravity * dt, m_maxFallSpeed);
			deltaY += m_fallSpeed * dt;
		}

		if (deltaX != 0.0f || deltaY != 0.0f || (m_useGravity && !fullyInLadder))
		{
			localPos += glm::vec3{ deltaX, deltaY, 0.0f };

			if (m_useGravity && !fullyInLadder && collision)
			{
				GetOwner()->SetLocalPosition(localPos);

				const float feetY = collision->GetFeetY();
				const float prevFeetY = feetY - deltaY;

				m_isGrounded = false;
				float bestGroundY = std::numeric_limits<float>::infinity();

				for (auto* other : CollisionComponent::GetAllColliders())
				{
					if (other == collision) continue;

					if (other->GetLayer() == CollisionLayer::Platform)
					{
						const auto& pAABB = other->GetAABB();
						const auto& myAABB = collision->GetAABB();

						// Determine if standing over this platform horizontally
						if (myAABB.right > pAABB.left && myAABB.left < pAABB.right)
						{
							if (m_fallSpeed >= 0.0f)
							{
								// Allow a small margin so we don't fall through due to gravity pulses
								if (feetY + 2.f >= pAABB.top && prevFeetY <= pAABB.top + 2.f)
								{
									if (pAABB.top < bestGroundY)
									{
										bestGroundY = pAABB.top;
									}
								}
							}
						}
					}
				}

				if (bestGroundY != std::numeric_limits<float>::infinity())
				{
					localPos.y = bestGroundY - collision->GetOffset().y - collision->GetSize().y;
					m_fallSpeed = 0.0f;
					m_isGrounded = true;
					GetOwner()->SetLocalPosition(localPos);
				}
			}
			else
			{
				GetOwner()->SetLocalPosition(localPos);
			}
		}

		Stop();
	}

	void MovementComponent::Move(const glm::vec2& direction)
	{
		m_velocity = direction;
	}

	void MovementComponent::SetGrounded(bool grounded)
	{
		m_isGrounded = grounded;
		if (grounded)
		{
			m_fallSpeed = 0.0f;
		}
	}

	void MovementComponent::GoInCircles()
	{
		const float dt = TimeManager::GetInstance().GetDeltaTime();

		m_angle += m_speed * dt;

		constexpr float twoPi = glm::two_pi<float>();

		if (m_angle >= twoPi || m_angle <= -twoPi)
		{
			m_angle = std::fmod(m_angle, twoPi);
			if (m_angle < 0.0f)
				m_angle += twoPi;
		}

		const float x = m_radius * glm::cos(m_angle);
		const float y = m_radius * glm::sin(m_angle);

		GetOwner()->SetLocalPosition(glm::vec3{ x, y, 0.0f });
	}
}