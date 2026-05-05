#pragma once
#include <functional>
#include <memory>
#include "Commands.h"
#include "GameStateManager.h"

namespace dae
{
	class ChangeStateCommand final : public Commands
	{
	public:
		explicit ChangeStateCommand(std::function<std::unique_ptr<GameState>()> factory)
			: m_factory(std::move(factory))
		{}

		void Execute() override
		{
			if (m_factory)
			{
				GameStateManager::GetInstance().ChangeState(m_factory());
			}
		}

	private:
		std::function<std::unique_ptr<GameState>()> m_factory{};
	};
}