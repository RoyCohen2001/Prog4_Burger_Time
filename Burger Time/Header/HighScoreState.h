#pragma once
#include "GameState.h"

namespace dae
{
	class HighscoreState final : public GameState
	{
	public:
		void OnEnter() override;
	};
}