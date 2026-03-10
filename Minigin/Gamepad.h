#pragma once
#include <windows.h>
#include <Xinput.h>
#include <memory>

namespace dae
{
	enum class Button {
		A = XINPUT_GAMEPAD_A,
		B = XINPUT_GAMEPAD_B,
		X = XINPUT_GAMEPAD_X,
		Y = XINPUT_GAMEPAD_Y,

		DPAD_UP = XINPUT_GAMEPAD_DPAD_UP,
		DPAD_DOWN = XINPUT_GAMEPAD_DPAD_DOWN,
		DPAD_LEFT = XINPUT_GAMEPAD_DPAD_LEFT,
		DPAD_RIGHT = XINPUT_GAMEPAD_DPAD_RIGHT,

		RIGHT_SHOULDER = XINPUT_GAMEPAD_RIGHT_SHOULDER,
		LEFT_SHOULDER = XINPUT_GAMEPAD_LEFT_SHOULDER,

		RIGHT_THUMB = XINPUT_GAMEPAD_RIGHT_THUMB,
		LEFT_THUMB = XINPUT_GAMEPAD_LEFT_THUMB,

		START = XINPUT_GAMEPAD_START,
		BACK = XINPUT_GAMEPAD_BACK
	};

	class Gamepad
	{

	public:
		Gamepad(int controllerIndex = 0);
		virtual ~Gamepad() = default;

		virtual bool IsPressed(Button button) const;
		virtual bool IsDownThisFrame(Button button) const;
		virtual bool IsUpThisFrame(Button button) const;

		virtual void Update();

	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl;


	};
}