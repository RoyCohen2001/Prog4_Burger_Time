#include "GameStateManager.h"

void dae::GameStateManager::ChangeState(std::unique_ptr<GameState> state)
{
	m_machine.ChangeState(std::move(state));
}

dae::GameState* dae::GameStateManager::GetCurrentState() const
{
	return m_machine.GetCurrentState();
}

void dae::GameStateManager::Update()
{
	m_machine.Update();
}

void dae::GameStateManager::Render() const
{
	m_machine.Render();
}