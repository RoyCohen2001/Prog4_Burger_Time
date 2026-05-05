#include "GameStateMachine.h"

void dae::GameStateMachine::ChangeState(std::unique_ptr<GameState> state)
{
	m_pending = std::move(state);
}

dae::GameState* dae::GameStateMachine::GetCurrentState() const
{
	return m_current.get();
}

void dae::GameStateMachine::Update()
{
	if (m_pending)
	{
		if (m_current)
		{
			m_current->OnExit();
		}

		m_current = std::move(m_pending);

		if (m_current)
		{
			m_current->OnEnter();
		}
	}

	if (m_current)
	{
		m_current->Update();
	}
}

void dae::GameStateMachine::Render() const
{
	if (m_current)
	{
		m_current->Render();
	}
}