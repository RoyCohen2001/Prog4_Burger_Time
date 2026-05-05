#pragma once
#include "Singleton.h"
#include "GameStateMachine.h"

namespace dae
{
	class GameStateManager final : public Singleton<GameStateManager>
	{
	public:
		void ChangeState(std::unique_ptr<GameState> state);
		GameState* GetCurrentState() const;

		void Update();
		void Render() const;

	private:
		friend class Singleton<GameStateManager>;
		GameStateManager() = default;

		GameStateMachine m_machine{};
	};
}