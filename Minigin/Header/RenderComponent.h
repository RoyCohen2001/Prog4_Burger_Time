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
		RenderComponent(GameObject* owner);

		void Render() const override;

		void SetTexture(const std::string& filename);
		void SetTexture(const std::shared_ptr<Texture2D> texture);

	private:
		std::shared_ptr<Texture2D> m_texture{ nullptr };
	};
}