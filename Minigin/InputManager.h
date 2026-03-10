#pragma once
#include <map>
#include "Singleton.h"
#include "Gamepad.h"
#include <vector>
#include "Commands.h"

namespace dae
{
	enum class InputState
	{
		Pressed,
		DownThisFrame,
		UpThisFrame
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager(const InputManager& other) = delete;
		InputManager(InputManager&& other) = delete;
		InputManager& operator=(const InputManager& other) = delete;
		InputManager& operator=(InputManager&& other) = delete;

		bool ProcessInput();
		void ClearBindings();

		void BindCommandToGamepad(int controllerIdx, InputState state, Button button, Commands* command);
		void BindCommandToKeyboard(unsigned int key, InputState state, Commands* command);

	private:
		friend class Singleton<InputManager>;

		InputManager();
		~InputManager() = default;

		bool ProcessKeyboardInput();
		void ProcessControllerInput();

		std::vector<std::unique_ptr<Gamepad>> m_pGamepads;

		std::vector<std::map<Button, std::pair<Commands*, InputState>>> m_GamepadCommands;
		std::vector<std::map<unsigned int, std::pair<Commands*, InputState>>> m_KeyboardCommands;

		std::vector<bool> m_PrevKeyStates;
		std::vector<bool> m_CurrKeyStates;
		int m_NumKeys = 0;

	};
}