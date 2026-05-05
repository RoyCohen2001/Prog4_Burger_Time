#include "InputBindings.h"

#include <SDL3/SDL.h>
#include "InputManager.h"

// Game related includes
#include "GameCommands.h"
#include "GameActor.h"

// Menu related includes
#include "MenuState.h"
#include "MenuCommands.h"

void dae::SetInputMappingController(GameActor* actor, int controller)
{
	InputManager::GetInstance().BindCommandToGamepad(controller, InputState::Pressed, Button::DPAD_LEFT, std::make_unique<Move>(actor, glm::vec2{ -1, 0 }));
	InputManager::GetInstance().BindCommandToGamepad(controller, InputState::Pressed, Button::DPAD_RIGHT, std::make_unique<Move>(actor, glm::vec2{ 1, 0 }));
	InputManager::GetInstance().BindCommandToGamepad(controller, InputState::Pressed, Button::DPAD_UP, std::make_unique<Move>(actor, glm::vec2{ 0, -1 }));
	InputManager::GetInstance().BindCommandToGamepad(controller, InputState::Pressed, Button::DPAD_DOWN, std::make_unique<Move>(actor, glm::vec2{ 0, 1 }));

	InputManager::GetInstance().BindCommandToGamepad(controller, InputState::DownThisFrame, Button::X, std::make_unique<Damage>(actor));
	InputManager::GetInstance().BindCommandToGamepad(controller, InputState::DownThisFrame, Button::B, std::make_unique<Pellets>(actor));

	SetBackToMenuInputMappingController(controller);
}

void dae::SetInputMappingKeyboard(GameActor* actor)
{
	InputManager::GetInstance().BindCommandToKeyboard(SDL_SCANCODE_A, InputState::Pressed, std::make_unique<Move>(actor, glm::vec2{ -1, 0 }));
	InputManager::GetInstance().BindCommandToKeyboard(SDL_SCANCODE_D, InputState::Pressed, std::make_unique<Move>(actor, glm::vec2{ 1, 0 }));
	InputManager::GetInstance().BindCommandToKeyboard(SDL_SCANCODE_W, InputState::Pressed, std::make_unique<Move>(actor, glm::vec2{ 0, -1 }));
	InputManager::GetInstance().BindCommandToKeyboard(SDL_SCANCODE_S, InputState::Pressed, std::make_unique<Move>(actor, glm::vec2{ 0, 1 }));

	InputManager::GetInstance().BindCommandToKeyboard(SDL_SCANCODE_C, InputState::DownThisFrame, std::make_unique<Damage>(actor));
	InputManager::GetInstance().BindCommandToKeyboard(SDL_SCANCODE_X, InputState::DownThisFrame, std::make_unique<Pellets>(actor));

	SetBackToMenuInputMappingKeyboard();
}

void dae::SetMenuInputMappingController(MenuState* menu, int controller) {
	InputManager::GetInstance().BindCommandToGamepad(
		controller,
		InputState::DownThisFrame,
		Button::DPAD_UP,
		std::make_unique<MenuNavigateCommand>(menu, -1)
	);
	InputManager::GetInstance().BindCommandToGamepad(
		controller,
		InputState::DownThisFrame,
		Button::DPAD_DOWN,
		std::make_unique<MenuNavigateCommand>(menu, 1)
	);
	InputManager::GetInstance().BindCommandToGamepad(
		controller,
		InputState::DownThisFrame,
		Button::A,
		std::make_unique<MenuSelectCommand>(menu)
	);
	InputManager::GetInstance().BindCommandToGamepad(
		controller,
		InputState::DownThisFrame,
		Button::START,
		std::make_unique<MenuSelectCommand>(menu)
	);
}

void dae::SetMenuInputMappingKeyboard(MenuState* menu) {
	InputManager::GetInstance().BindCommandToKeyboard(
		SDL_SCANCODE_UP,
		InputState::DownThisFrame,
		std::make_unique<MenuNavigateCommand>(menu, -1)
	);
	InputManager::GetInstance().BindCommandToKeyboard(
		SDL_SCANCODE_DOWN,
		InputState::DownThisFrame,
		std::make_unique<MenuNavigateCommand>(menu, 1)
	);
	InputManager::GetInstance().BindCommandToKeyboard(
		SDL_SCANCODE_RETURN,
		InputState::DownThisFrame,
		std::make_unique<MenuSelectCommand>(menu)
	);
	InputManager::GetInstance().BindCommandToKeyboard(
		SDL_SCANCODE_SPACE,
		InputState::DownThisFrame,
		std::make_unique<MenuSelectCommand>(menu)
	);
}

void dae::SetBackToMenuInputMappingController(int controller)
{
	InputManager::GetInstance().BindCommandToGamepad(
		controller,
		InputState::DownThisFrame,
		Button::BACK,
		std::make_unique<ChangeStateCommand>([] { return std::make_unique<MenuState>(); })
	);
	InputManager::GetInstance().BindCommandToGamepad(
		controller,
		InputState::DownThisFrame,
		Button::START,
		std::make_unique<ChangeStateCommand>([] { return std::make_unique<MenuState>(); })
	);
}

void dae::SetBackToMenuInputMappingKeyboard()
{
	InputManager::GetInstance().BindCommandToKeyboard(
		SDL_SCANCODE_ESCAPE,
		InputState::DownThisFrame,
		std::make_unique<ChangeStateCommand>([] { return std::make_unique<MenuState>(); })
	);
	InputManager::GetInstance().BindCommandToKeyboard(
		SDL_SCANCODE_RETURN,
		InputState::DownThisFrame,
		std::make_unique<ChangeStateCommand>([] { return std::make_unique<MenuState>(); })
	);
}