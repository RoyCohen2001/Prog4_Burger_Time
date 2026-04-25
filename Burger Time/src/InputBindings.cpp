#include "InputBindings.h"

#include <SDL3/SDL.h>
#include "InputManager.h"
#include "GameCommands.h"
#include "GameActor.h"

void dae::SetInputMappingController(GameActor* actor, int controller)
{
	InputManager::GetInstance().BindCommandToGamepad(controller, InputState::Pressed, Button::DPAD_LEFT, std::make_unique<Move>(actor, glm::vec2{ -1, 0 }));
	InputManager::GetInstance().BindCommandToGamepad(controller, InputState::Pressed, Button::DPAD_RIGHT, std::make_unique<Move>(actor, glm::vec2{ 1, 0 }));
	InputManager::GetInstance().BindCommandToGamepad(controller, InputState::Pressed, Button::DPAD_UP, std::make_unique<Move>(actor, glm::vec2{ 0, -1 }));
	InputManager::GetInstance().BindCommandToGamepad(controller, InputState::Pressed, Button::DPAD_DOWN, std::make_unique<Move>(actor, glm::vec2{ 0, 1 }));

	InputManager::GetInstance().BindCommandToGamepad(controller, InputState::DownThisFrame, Button::X, std::make_unique<Damage>(actor));
	InputManager::GetInstance().BindCommandToGamepad(controller, InputState::DownThisFrame, Button::B, std::make_unique<Pellets>(actor));
}

void dae::SetInputMappingKeyboard(GameActor* actor)
{
	InputManager::GetInstance().BindCommandToKeyboard(SDL_SCANCODE_A, InputState::Pressed, std::make_unique<Move>(actor, glm::vec2{ -1, 0 }));
	InputManager::GetInstance().BindCommandToKeyboard(SDL_SCANCODE_D, InputState::Pressed, std::make_unique<Move>(actor, glm::vec2{ 1, 0 }));
	InputManager::GetInstance().BindCommandToKeyboard(SDL_SCANCODE_W, InputState::Pressed, std::make_unique<Move>(actor, glm::vec2{ 0, -1 }));
	InputManager::GetInstance().BindCommandToKeyboard(SDL_SCANCODE_S, InputState::Pressed, std::make_unique<Move>(actor, glm::vec2{ 0, 1 }));

	InputManager::GetInstance().BindCommandToKeyboard(SDL_SCANCODE_C, InputState::DownThisFrame, std::make_unique<Damage>(actor));
	InputManager::GetInstance().BindCommandToKeyboard(SDL_SCANCODE_X, InputState::DownThisFrame, std::make_unique<Pellets>(actor));
}