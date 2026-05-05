#pragma once
#include "GameState.h"

namespace dae
{
	class CoopState final : public GameState
	{
	public:
		void OnEnter() override;
	};
}