#pragma once
#include "Commands.h"
#include "GameActor.h"

namespace dae {
	class GameCommands : public Commands
	{
		GameActor* m_actor{};
	protected:
		GameActor* GetGameActor() const { return m_actor; }
	public:
		explicit GameCommands(GameActor* actor)
			: m_actor(actor)
		{}
	};

	class Move : public GameCommands
	{
	public:
		Move(GameActor* actor, const glm::vec2& direction) :
			GameCommands(actor),
			m_Direction(direction)
		{}

		void Execute() override
		{
			GetGameActor()->Move(m_Direction);
		}
	private:
		glm::vec2 m_Direction;
	};

	class Damage : public GameCommands
	{
	public:
		Damage(GameActor* actor) :
			GameCommands(actor)
		{}
		void Execute() override
		{
			GetGameActor()->OnDeath();
		}
	};

	class Pellets : public GameCommands
	{
	public:
		Pellets(GameActor* actor) :
			GameCommands(actor)
		{}
		void Execute() override
		{
			GetGameActor()->AddScore(10);
		}
	};
}

