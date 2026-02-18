#pragma once
#include "Component.h"

namespace dae
{
	class TextComponent;

	class FPSComponent final : public Component
	{
	public:
		explicit FPSComponent(GameObject* owner);
		void Update() override;

	private:
		int static constexpr MAX_SAMPLES{};
		float m_frames{};
		float m_fps{};
		float m_accumulatedTime{};
		float m_updateInterval{}; 
		float m_timeSinceLastUpdate{};

		TextComponent* m_textComponent{ nullptr };
	};
}