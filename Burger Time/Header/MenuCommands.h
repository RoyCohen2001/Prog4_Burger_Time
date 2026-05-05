#pragma once
#include "StateCommands.h"
#include "MenuState.h"

namespace dae {

	class MenuNavigateCommand final : public Commands
	{
	public:
		MenuNavigateCommand(MenuState* menu, int delta) :
			m_menu(menu),
			m_delta(delta)
		{}

		void Execute() override
		{
			if (m_menu)
			{
				m_menu->MoveSelection(m_delta);
			}
		}

	private:
		MenuState* m_menu{};
		int m_delta{};
	};

	class MenuSelectCommand final : public Commands
	{
	public:
		explicit MenuSelectCommand(MenuState* menu) :
			m_menu(menu)
		{}

		void Execute() override
		{
			if (m_menu)
			{
				m_menu->SelectCurrent();
			}
		}

	private:
		MenuState* m_menu{};
	};
}