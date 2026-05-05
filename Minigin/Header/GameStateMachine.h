#pragma once
#include <memory>
#include "GameState.h"

namespace dae
{
	class GameStateMachine
	{
	public:
		void ChangeState(std::unique_ptr<GameState> state);
		GameState* GetCurrentState() const;

		void Update();
		void Render() const;

	private:
		std::unique_ptr<GameState> m_current{};
		std::unique_ptr<GameState> m_pending{};
	};
}