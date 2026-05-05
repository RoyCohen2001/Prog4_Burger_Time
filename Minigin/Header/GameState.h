#pragma once
#include "InputManager.h"

namespace dae
{
	class GameState
	{
	public:
		virtual ~GameState() = default;

		virtual void OnEnter() {}
		virtual void OnExit() {
			InputManager::GetInstance().ClearBindings();
		}
		virtual void Update() {}
		virtual void Render() const {}
	};
}