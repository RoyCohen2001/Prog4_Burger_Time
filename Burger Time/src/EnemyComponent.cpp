#include "EnemyComponent.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "MovementComponent.h"
#include "CollisionComponent.h"
#include "AnimationComponent.h"
#include "TimeManager.h"
#include "LevelLoader.h"

#include <random>
#include <cmath>

namespace
{
	float RandomRange(float minValue, float maxValue)
	{
		static std::mt19937 rng{ std::random_device{}() };
		std::uniform_real_distribution<float> dist(minValue, maxValue);
		return dist(rng);
	}
}

dae::EnemyComponent::EnemyComponent(GameObject* owner)
	: Component(owner)
{
	m_renderComponent = owner->AddComponent<RenderComponent>();
	m_renderComponent->SetScale(m_scale);

	m_animationComponent = owner->AddComponent<AnimationComponent>();
	ApplyAnimationSetup();

	m_collisionComponent = owner->AddComponent<CollisionComponent>(
		glm::vec2{ 0.f, 0.f },
		glm::vec2{ 0.f, 0.f },
		CollisionLayer::Enemy);

	UpdateCollisionSize();

	m_movementComponent = owner->AddComponent<MovementComponent>();
	m_movementComponent->SetSpeed(m_speed);
	m_movementComponent->SetGravityEnabled(true);

	ChooseNewHorizontalDirection();
	m_decisionTimer = RandomRange(0.5f, 1.5f);
}

void dae::EnemyComponent::SetSpeed(float speed)
{
	m_speed = speed;
	if (m_movementComponent)
	{
		m_movementComponent->SetSpeed(speed);
	}
}

void dae::EnemyComponent::SetScale(float scale)
{
	m_scale = scale;
	if (m_renderComponent)
	{
		m_renderComponent->SetScale(scale);
	}
	UpdateCollisionSize();
}

void dae::EnemyComponent::SetSpriteSheet(const std::string& path, const glm::vec2& frameSize, int columns, const glm::vec2& originOffset)
{
	m_spritePath = path;
	m_frameSize = frameSize;
	m_columns = columns;
	m_originOffset = originOffset;
	ApplyAnimationSetup();
	UpdateCollisionSize();
}

void dae::EnemyComponent::SetAnimationClips(int walkLeftStart, int walkRightStart, int idleStart, int climbStart, int stunnedStart, int framesPerClip)
{
	m_walkLeftStart = walkLeftStart;
	m_walkRightStart = walkRightStart;
	m_idleStart = idleStart;
	m_climbStart = climbStart;
	m_stunnedStart = stunnedStart;
	m_framesPerClip = framesPerClip;
	ApplyAnimationSetup();
}

void dae::EnemyComponent::SetAnimationFps(float walkFps, float idleFps, float climbFps, float stunnedFps)
{
	m_walkFps = walkFps;
	m_idleFps = idleFps;
	m_climbFps = climbFps;
	m_stunnedFps = stunnedFps;
	ApplyAnimationSetup();
}

void dae::EnemyComponent::ApplyAnimationSetup()
{
	if (!m_animationComponent || !m_renderComponent)
		return;

	m_animationComponent->SetSpriteSheet(m_spritePath, m_frameSize, m_columns, m_originOffset);

	m_animationComponent->AddClip("walk_left", m_walkLeftStart, m_framesPerClip, m_walkFps);
	m_animationComponent->AddClip("walk_right", m_walkRightStart, m_framesPerClip, m_walkFps);
	m_animationComponent->AddClip("idle", m_idleStart, m_framesPerClip, m_idleFps);
	m_animationComponent->AddClip("climb", m_climbStart, m_framesPerClip, m_climbFps);
	m_animationComponent->AddClip("stunned", m_stunnedStart, m_framesPerClip, m_stunnedFps);
	m_animationComponent->Play("idle");
}

void dae::EnemyComponent::UpdateCollisionSize()
{
	if (!m_collisionComponent)
		return;

	const auto size = m_frameSize * m_scale;
	m_collisionComponent->SetSize(size);
}

void dae::EnemyComponent::EnsureComponents()
{
	if (!m_renderComponent)
		m_renderComponent = GetOwner()->GetComponent<RenderComponent>();
	if (!m_collisionComponent)
		m_collisionComponent = GetOwner()->GetComponent<CollisionComponent>();
	if (!m_movementComponent)
		m_movementComponent = GetOwner()->GetComponent<MovementComponent>();
	if (!m_animationComponent)
		m_animationComponent = GetOwner()->GetComponent<AnimationComponent>();
}

void dae::EnemyComponent::ChooseNewHorizontalDirection()
{
	m_direction.x = (RandomRange(0.f, 1.f) < 0.5f) ? -1.f : 1.f;
	m_direction.y = 0.f;
}

bool dae::EnemyComponent::CanExitLadder() const
{
	if (!m_collisionComponent)
		return false;

	const auto& myAABB = m_collisionComponent->GetAABB();
	const float feetY = m_collisionComponent->GetFeetY();

	for (auto* other : CollisionComponent::GetAllColliders())
	{
		if (!other || other == m_collisionComponent)
			continue;

		if (other->GetLayer() != CollisionLayer::Platform)
			continue;

		const auto& pAABB = other->GetAABB();
		if (myAABB.right > pAABB.left && myAABB.left < pAABB.right)
		{
			if (std::abs(feetY - pAABB.top) <= 8.f)
				return true;
		}
	}

	return false;
}

void dae::EnemyComponent::UpdateAnimation(bool onLadder)
{
	if (!m_animationComponent)
		return;

	if (m_isStunned)
		m_animationComponent->Play("stunned");
	else if (m_mode == Mode::Climb && onLadder)
		m_animationComponent->Play("climb");
	else if (m_direction.x < 0.f)
		m_animationComponent->Play("walk_left");
	else if (m_direction.x > 0.f)
		m_animationComponent->Play("walk_right");
	else
		m_animationComponent->Play("idle");
}

void dae::EnemyComponent::Update()
{
	EnsureComponents();
	if (!m_movementComponent || !m_collisionComponent)
		return;

	const float dt = TimeManager::GetInstance().GetDeltaTime();
	m_decisionTimer -= dt;

	const float centerX = m_collisionComponent->GetCenterX();
	const float centerY = m_collisionComponent->GetCenterY();
	const bool onLadder = LevelLoader::IsOnLadder(centerX, centerY);

	if (m_mode == Mode::Horizontal)
	{
		if (onLadder && m_decisionTimer <= 0.f && RandomRange(0.f, 1.f) < 0.3f)
		{
			m_mode = Mode::Climb;
			m_climbUp = (RandomRange(0.f, 1.f) < 0.5f);
			m_climbTimer = RandomRange(0.8f, 1.6f);
		}

		const float feetY = m_collisionComponent->GetFeetY();
		const float currentTop = LevelLoader::GetGroundYAt(centerX, feetY - 2.f);
		const float nextX = centerX + m_direction.x * m_speed * dt * 1.5f;
		const float nextTop = LevelLoader::GetGroundYAt(nextX, feetY - 2.f);

		if (!std::isfinite(currentTop) || !std::isfinite(nextTop) || std::abs(nextTop - currentTop) > 6.f)
		{
			m_direction.x *= -1.f;
		}

		m_movementComponent->Move({ m_direction.x, 0.f });
	}
	else
	{
		m_climbTimer -= dt;

		if (!onLadder)
		{
			m_mode = Mode::Horizontal;
			m_decisionTimer = RandomRange(0.5f, 1.5f);
			ChooseNewHorizontalDirection();
		}
		else if (m_climbTimer <= 0.f && CanExitLadder())
		{
			m_mode = Mode::Horizontal;
			m_decisionTimer = RandomRange(0.5f, 1.5f);
			ChooseNewHorizontalDirection();
		}
		else
		{
			m_movementComponent->Move({ 0.f, m_climbUp ? -1.f : 1.f });
		}
	}

	UpdateAnimation(onLadder);
}