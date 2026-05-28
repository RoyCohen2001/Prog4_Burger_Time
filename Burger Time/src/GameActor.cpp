#include "GameActor.h"
#include "GameObject.h"
#include "Observer.h"
#include "AnimationComponent.h"
#include "LevelLoader.h"
#include "PlayerState.h"

namespace
{
	constexpr int kPeterColumns = 3;
}

dae::GameActor::GameActor(GameObject* owner) :
	Component(owner), m_lives(3), m_score(0)
{
	m_renderComponent = owner->AddComponent<RenderComponent>();
	m_renderComponent->SetScale(2.f);

	m_collisionComponent = owner->AddComponent<CollisionComponent>();
	m_movementComponent = owner->AddComponent<MovementComponent>();

	ChangeState(std::make_unique<PlayerIdleState>());
}

void dae::GameActor::SetupAnimations(const std::string& spriteSheetPath, const glm::vec2& frameSize, const glm::vec2& originOffset)
{
	if (!m_animationComponent)
	{
		m_animationComponent = GetOwner()->AddComponent<AnimationComponent>();
	}

	m_animationComponent->SetSpriteSheet(spriteSheetPath, frameSize, kPeterColumns, originOffset);

	// rows: idle(0,1), walk_left(1,3), walk_right(2,3), ladder(3,3)
	m_animationComponent->AddClip("idle", 0, 1, 1.f, true);
	m_animationComponent->AddClip("walk_left", 3, 3, 6.f, true);
	m_animationComponent->AddClip("walk_right", 6, 3, 6.f, true);
	m_animationComponent->AddClip("ladder", 9, 3, 6.f, true);
	m_animationComponent->Play("idle");

	m_collisionComponent->SetSize({ frameSize.x * 2.f, frameSize.y * 2.f });
}

void dae::GameActor::Update()
{
	if (m_state)
	{
		auto nextState = m_state->HandleInput(*this);
		if (nextState)
		{
			ChangeState(std::move(nextState));
		}

		m_state->Update(*this);
	}

	m_hasMoveInput = false;
	m_hasShootInput = false;
	m_lastMoveDir = { 0.f, 0.f };
}

void dae::GameActor::ChangeState(std::unique_ptr<PlayerState> state)
{
	if (m_state)
	{
		m_state->OnExit(*this);
	}

	m_state = std::move(state);

	if (m_state)
	{
		m_state->OnEnter(*this);
	}
}

void dae::GameActor::PlayAnimation(const std::string& name)
{
	if (m_animationComponent)
	{
		m_animationComponent->Play(name);
	}
}

void dae::GameActor::Move(const glm::vec2& direction)
{
	m_lastMoveDir = direction;
	m_hasMoveInput = (direction.x != 0.f || direction.y != 0.f);
}

bool dae::GameActor::IsOnLadder() const
{
	if (!m_collisionComponent)
	{
		return false;
	}

	return LevelLoader::IsOnLadder(m_collisionComponent->GetCenterX(), m_collisionComponent->GetCenterY());
}

void dae::GameActor::Shoot()
{
	m_hasShootInput = true;
}

void dae::GameActor::OnDeath()
{
	if (m_lives <= 0)
		return;

	m_lives -= 1;
	Notify("PlayerDied", GetOwner());
}

void dae::GameActor::AddScore(int points)
{
	m_score += points;
	Notify("ScoreChanged", GetOwner());
}