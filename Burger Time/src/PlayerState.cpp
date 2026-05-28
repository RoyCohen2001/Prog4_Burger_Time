#include "playerstate.h"
#include "GameActor.h"
#include "TimeManager.h"

// -----------------
// PlayerMoveState
// -----------------
void dae::PlayerMoveState::OnEnter(GameActor& actor)
{
	actor.PlayAnimation("walk");
}

void dae::PlayerMoveState::OnExit(GameActor&)
{}

std::unique_ptr<dae::PlayerState> dae::PlayerMoveState::HandleInput(GameActor& actor)
{
	if (actor.GetLives() <= 0)
	{
		return std::make_unique<PlayerDeadState>();
	}

	const auto& dir = actor.GetLastMoveDir();
	if (actor.IsOnLadder() && std::abs(dir.y) > 0.f)
	{
		return std::make_unique<PlayerLadderState>();
	}

	if (actor.HasShootInput())
	{
		return std::make_unique<PlayerShootState>();
	}

	if (!actor.HasMoveInput())
	{
		return std::make_unique<PlayerIdleState>();
	}

	return nullptr;
}
void dae::PlayerMoveState::Update(GameActor& actor)
{
	const auto& dir = actor.GetLastMoveDir();

	if (auto* movement = actor.GetMovementComponent())
	{
		movement->Move(dir);
	}

	if (dir.x < 0.f)
	{
		actor.PlayAnimation("walk_left");
	}
	else if (dir.x > 0.f)
	{
		actor.PlayAnimation("walk_right");
	}
	else
	{
		actor.PlayAnimation("idle");
	}
}

// -----------------
// PlayerIdleState
// -----------------
void dae::PlayerIdleState::OnEnter(GameActor& actor)
{
	actor.PlayAnimation("idle");
}

void dae::PlayerIdleState::OnExit(GameActor&)
{}

std::unique_ptr<dae::PlayerState> dae::PlayerIdleState::HandleInput(GameActor& actor)
{
	if (actor.GetLives() <= 0)
	{
		return std::make_unique<PlayerDeadState>();
	}

	const auto& dir = actor.GetLastMoveDir();
	if (actor.IsOnLadder() && std::abs(dir.y) > 0.f)
	{
		return std::make_unique<PlayerLadderState>();
	}

	if (actor.HasShootInput())
	{
		return std::make_unique<PlayerShootState>();
	}

	if (actor.HasMoveInput())
	{
		return std::make_unique<PlayerMoveState>();
	}

	return nullptr;
}

void dae::PlayerIdleState::Update(GameActor& actor)
{
	actor.PlayAnimation("idle");
}

// -----------------
// PlayerLadderState
// -----------------
void dae::PlayerLadderState::OnEnter(GameActor& actor)
{
	actor.PlayAnimation("ladder");
}

void dae::PlayerLadderState::OnExit(GameActor&)
{}

std::unique_ptr<dae::PlayerState> dae::PlayerLadderState::HandleInput(GameActor& actor)
{
	if (actor.GetLives() <= 0)
	{
		return std::make_unique<PlayerDeadState>();
	}

	if (!actor.IsOnLadder())
	{
		if (actor.HasMoveInput())
		{
			return std::make_unique<PlayerMoveState>();
		}
		else
		{
			return std::make_unique<PlayerIdleState>();
		}
	}

	return nullptr;
}

void dae::PlayerLadderState::Update(GameActor& actor)
{
	const auto& dir = actor.GetLastMoveDir();

	if (auto* movement = actor.GetMovementComponent())
	{
		movement->Move(dir);
	}

	if (std::abs(dir.y) > 0.f)
	{
		actor.PlayAnimation("ladder");
	}
	else
	{
		actor.PlayAnimation("idle");
	}
}

// -----------------
// PlayerDeadState
// -----------------
void dae::PlayerDeadState::OnEnter(GameActor& actor)
{
	if (auto* movement = actor.GetMovementComponent())
	{
		movement->Stop();
	}
	actor.PlayAnimation("Dead");
}

void dae::PlayerDeadState::OnExit(GameActor&)
{}

std::unique_ptr<dae::PlayerState> dae::PlayerDeadState::HandleInput(GameActor&)
{
	return nullptr;
}

void dae::PlayerDeadState::Update(GameActor& actor)
{
	if (auto* movement = actor.GetMovementComponent())
	{
		movement->Stop();
	}
	actor.PlayAnimation("Dead");
}

// -----------------
// PlayerShootState
// -----------------
void dae::PlayerShootState::OnEnter(GameActor& actor)
{
	m_elapsed = 0.f;
	if (auto* movement = actor.GetMovementComponent())
	{
		movement->Stop();
	}
	actor.PlayAnimation("idle");
}

void dae::PlayerShootState::OnExit(GameActor&)
{}

std::unique_ptr<dae::PlayerState> dae::PlayerShootState::HandleInput(GameActor& actor)
{
	if (actor.GetLives() <= 0)
	{
		return std::make_unique<PlayerDeadState>();
	}

	if (m_elapsed < kShootDuration)
	{
		return nullptr;
	}

	if (actor.IsOnLadder())
	{
		return std::make_unique<PlayerLadderState>();
	}

	if (actor.HasMoveInput())
	{
		return std::make_unique<PlayerMoveState>();
	}

	return std::make_unique<PlayerIdleState>();
}

void dae::PlayerShootState::Update(GameActor& actor)
{
	m_elapsed += TimeManager::GetInstance().GetDeltaTime();
	actor.PlayAnimation("idle");
}