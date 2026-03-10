#pragma once
#include "GameActor.h"


namespace dae {
	class Commands
	{
	public:
		virtual ~Commands() = default;
		virtual void Execute() = 0;
	};

	class GameCommands : public Commands
	{
		GameActor* m_actor;
	protected:
		GameActor* GetGameActor() const { return m_actor; }
	public:
		GameCommands(GameActor* actor):
			m_actor(actor)
		{
		}
		virtual ~GameCommands() {
			delete m_actor;
			m_actor = nullptr;
		}
	};

	class Move : public GameCommands
	{
	public:
		Move(GameActor* actor, const glm::vec2& direction) :
			GameCommands(actor),
			m_Direction(direction)
		{
		}

		void Execute() override
		{
			GetGameActor()->Move(m_Direction);
		}
	private:
		glm::vec2 m_Direction;
	};

}