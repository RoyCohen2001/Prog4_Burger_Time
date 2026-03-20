#pragma once
#include <cstdint>
#ifndef __EMSCRIPTEN__
#include <windows.h>
#include <Xinput.h>
#endif
#include <memory>

namespace dae
{
	enum class Button : std::uint16_t {
		A = 0x1000,
		B = 0x2000,
		X = 0x4000,
		Y = 0x8000,

		DPAD_UP = 0x0001,
		DPAD_DOWN = 0x0002,
		DPAD_LEFT = 0x0004,
		DPAD_RIGHT = 0x0008,

		START = 0x0010,
		BACK = 0x0020,

		LEFT_THUMB = 0x0040,
		RIGHT_THUMB = 0x0080,

		LEFT_SHOULDER = 0x0100,
		RIGHT_SHOULDER = 0x0200
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