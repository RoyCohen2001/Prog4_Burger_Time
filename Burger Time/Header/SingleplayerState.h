#pragma once
#include "GameState.h"

namespace dae
{
	class SingleplayerState final : public GameState
	{
	public:
		void OnEnter() override;
	};
}