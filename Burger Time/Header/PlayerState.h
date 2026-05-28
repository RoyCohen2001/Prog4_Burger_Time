#pragma once
#include <memory>

namespace dae
{
	class GameActor;

	class PlayerState
	{
	public:
		virtual ~PlayerState() = default;

		virtual void OnEnter(GameActor& actor) = 0;
		virtual void OnExit(GameActor& actor) = 0;
		virtual std::unique_ptr<PlayerState> HandleInput(GameActor& actor) = 0;
		virtual void Update(GameActor& actor) = 0;
	};

	class PlayerIdleState final : public PlayerState
	{
	public:
		void OnEnter(GameActor& actor) override;
		void OnExit(GameActor& actor) override;
		std::unique_ptr<PlayerState> HandleInput(GameActor& actor) override;
		void Update(GameActor& actor) override;
	};

	class PlayerMoveState final : public PlayerState
	{
	public:
		void OnEnter(GameActor& actor) override;
		void OnExit(GameActor& actor) override;
		std::unique_ptr<PlayerState> HandleInput(GameActor& actor) override;
		void Update(GameActor& actor) override;
	};

	class PlayerLadderState final : public PlayerState
	{
	public:
		void OnEnter(GameActor& actor) override;
		void OnExit(GameActor& actor) override;
		std::unique_ptr<PlayerState> HandleInput(GameActor& actor) override;
		void Update(GameActor& actor) override;
	};

	class PlayerDeadState final : public PlayerState
	{
	public:
		void OnEnter(GameActor& actor) override;
		void OnExit(GameActor& actor) override;
		std::unique_ptr<PlayerState> HandleInput(GameActor& actor) override;
		void Update(GameActor& actor) override;
	};

	class PlayerShootState final : public PlayerState
	{
	public:
		void OnEnter(GameActor& actor) override;
		void OnExit(GameActor& actor) override;
		std::unique_ptr<PlayerState> HandleInput(GameActor& actor) override;
		void Update(GameActor& actor) override;

	private:
		float m_elapsed{};
		static constexpr float kShootDuration{ 0.25f };
	};
}