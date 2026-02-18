#pragma once
#include "Component.h"
#include <memory>
#include <string>

namespace dae
{
	class Texture2D;

	class RenderComponent final : public Component
	{
	public:
		RenderComponent(GameObject* owner, const std::string& filename);

		void Render() const override;

		void SetTexture(const std::string& filename);

	private:
		std::shared_ptr<Texture2D> m_texture{ nullptr };
	};
}