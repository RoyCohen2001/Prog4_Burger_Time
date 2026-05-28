#pragma once
#include "Component.h"
#include <memory>
#include <string>
#include <glm/vec2.hpp>
#include <SDL3/SDL.h>

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

		void SetScale(float uniformScale) { m_scale = { uniformScale, uniformScale }; }
		void SetScale(const glm::vec2& scale) { m_scale = scale; }

		void SetSourceRect(const SDL_FRect& rect) { m_sourceRect = rect; m_useSourceRect = true; }
		void ClearSourceRect() { m_useSourceRect = false; }
		bool HasSourceRect() const { return m_useSourceRect; }

		glm::vec2 GetScaledSize() const;

	private:
		std::shared_ptr<Texture2D> m_texture{ nullptr };
		glm::vec2 m_scale{ 1.f, 1.f };

		bool m_useSourceRect{ false };
		SDL_FRect m_sourceRect{};
	};
}