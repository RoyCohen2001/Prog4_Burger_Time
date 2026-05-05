#pragma once

namespace dae
{
	class GameActor;
	class MenuState;

	void SetInputMappingController(GameActor* actor, int controller);
	void SetInputMappingKeyboard(GameActor* actor);

	void SetMenuInputMappingController(MenuState* menu, int controller);
	void SetMenuInputMappingKeyboard(MenuState* menu);

	void SetBackToMenuInputMappingController(int controller);
	void SetBackToMenuInputMappingKeyboard();
}