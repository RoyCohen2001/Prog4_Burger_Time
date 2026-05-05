#pragma once
#include "GameState.h"
#include <array>
#include <glm/vec2.hpp>

namespace dae
{
	class GameObject;

	class MenuState final : public GameState
	{
	public:
		void OnEnter() override;

		void MoveSelection(int delta);
		void SelectCurrent();

	private:
		void UpdateSelectorPosition();

		std::array<glm::vec2, 4> m_optionPositions{};
		int m_selectedIndex{ 0 };

		

		GameObject* m_selectorObject{ nullptr };
		float m_selectorX{ 0.0f };
		glm::vec2 m_selectorSize{};
	};
}