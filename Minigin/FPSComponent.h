#pragma once
#include "Component.h"

namespace dae
{
	class FPSComponent final : public Component
	{
		public:
			explicit FPSComponent(GameObject* owner);
			void Update() override;
	};
}