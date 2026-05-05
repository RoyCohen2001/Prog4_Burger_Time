#pragma once
#include "GameState.h"

namespace dae
{
	class VersusState final : public GameState
	{
	public:
		void OnEnter() override;
	};
}