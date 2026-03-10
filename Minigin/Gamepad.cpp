#include "Gamepad.h"
#include <XInput.h>

namespace dae {
    class Gamepad::Impl {
    public:
        Impl(int idx) :
            m_ControllerIndex(idx)
        {
        }

        void Update()
        {
            CopyMemory(&previousState, &currentState, sizeof(XINPUT_STATE));
            ZeroMemory(&currentState, sizeof(XINPUT_STATE));
            XInputGetState(m_ControllerIndex, &currentState);

            auto buttonChanges = currentState.Gamepad.wButtons ^ previousState.Gamepad.wButtons;
            buttonsPressedThisFrame = buttonChanges & currentState.Gamepad.wButtons;
            buttonsReleasedThisFrame = buttonChanges & (~currentState.Gamepad.wButtons);
        }

        bool IsPressed(Button button) const
        {
            return currentState.Gamepad.wButtons & static_cast<WORD>(button);
        }

        bool IsDownThisFrame(Button button) const
        {
			return buttonsPressedThisFrame & static_cast<WORD>(button);
        }

        bool IsUpThisFrame(Button button) const
        {
			return buttonsReleasedThisFrame & static_cast<WORD>(button);
        }
    private:
        int m_ControllerIndex;
        XINPUT_STATE previousState{};
        XINPUT_STATE currentState{};

		unsigned int buttonsPressedThisFrame{};
		unsigned int buttonsReleasedThisFrame{};
    };


    // PIMPL
    Gamepad::Gamepad(int controllerIndex) :
        m_pImpl(std::make_unique<Impl>(controllerIndex))
    {
    }

    void Gamepad::Update()
    {
        m_pImpl->Update();
    }
    bool Gamepad::IsPressed(Button button) const
    {
        return m_pImpl->IsPressed(button);
    }
    bool Gamepad::IsDownThisFrame(Button button) const
    {
        return m_pImpl->IsDownThisFrame(button);
    }
    bool Gamepad::IsUpThisFrame(Button button) const
    {
        return m_pImpl->IsUpThisFrame(button);
    }
}