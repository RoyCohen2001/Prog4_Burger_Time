#include <SDL3/SDL.h>
#include <backends/imgui_impl_sdl3.h>
#include "InputManager.h"

namespace dae {

	bool InputManager::ProcessInput()
	{
		ProcessControllerInput();

		if (!ProcessKeyboardInput()) {
			return false;
		}
		return true;
	}

	InputManager::InputManager()
	{
		m_pGamepads.emplace_back(std::make_unique<Gamepad>(0)); // Player 1
		m_pGamepads.emplace_back(std::make_unique<Gamepad>(1)); // Player 2
	}

	void InputManager::ClearBindings()
	{
		for (auto& gamepadCommands : m_GamepadCommands)
		{
			gamepadCommands.clear();
		}
		m_KeyboardCommands.clear();
	}

	void InputManager::BindCommandToGamepad(int controllerIdx, InputState state, Button button, std::unique_ptr<Commands> command)
	{
		m_GamepadCommands[controllerIdx][button] = std::make_pair(std::move(command), state);
	}

	void InputManager::BindCommandToKeyboard(unsigned int key, InputState state, std::unique_ptr<Commands> command)
	{
		m_KeyboardCommands[key] = std::make_pair(std::move(command), state);
	}

	bool InputManager::ProcessKeyboardInput()
	{
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_EVENT_QUIT) {
				return false;
			}
			ImGui_ImplSDL3_ProcessEvent(&e);
		}

		m_PrevKeyStates = m_CurrKeyStates;
		const bool* keystate = SDL_GetKeyboardState(&m_NumKeys);
		m_CurrKeyStates.assign(keystate, keystate + m_NumKeys);
		m_PrevKeyStates.resize(m_NumKeys, false);

		for (const auto& [key, pair] : m_KeyboardCommands) {
			if (key >= m_CurrKeyStates.size()) continue;

			Commands* command = pair.first.get();
			InputState state = pair.second;

			bool execute = false;
			bool isDown = m_CurrKeyStates[key];
			bool wasDown = m_PrevKeyStates[key];

			switch (state) {
			case InputState::Pressed:
				execute = isDown;
				break;
			case InputState::DownThisFrame:
				execute = isDown && !wasDown;
				break;
			case InputState::UpThisFrame:
				execute = !isDown && wasDown;
				break;
			}

			if (execute && command)
				command->Execute();
		}

		return true;
	}

	void InputManager::ProcessControllerInput()
	{
		for (size_t i = 0; i < m_pGamepads.size(); ++i)
		{
			m_pGamepads[i]->Update();
			for (const auto& [button, commandPair] : m_GamepadCommands[i])
			{
				Commands* command = commandPair.first.get();
				InputState state = commandPair.second;

				bool execute = false;
				switch (state)
				{
				case InputState::Pressed:
					execute = m_pGamepads[i]->IsPressed(button);
					break;
				case InputState::DownThisFrame:
					execute = m_pGamepads[i]->IsDownThisFrame(button);
					break;
				case InputState::UpThisFrame:
					execute = m_pGamepads[i]->IsUpThisFrame(button);
					break;
				}
				if (execute && command)
					command->Execute();
			}
		}
	}

}