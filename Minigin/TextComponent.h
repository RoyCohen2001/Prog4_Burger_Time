#pragma once
#include <string>
#include <memory>
#include "Component.h"
#include "Observer.h"
#include <SDL3/SDL.h>
#include "RenderComponent.h"

namespace dae
{
	class Font;
	class Texture2D;

	class TextComponent final : public Component, public Observer
	{
	public:

		enum class DisplayType { 
			Default, 
			Lives, 
			Score };

		TextComponent(GameObject* owner, const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color = { 255, 255, 255, 255 }, DisplayType type = DisplayType::Default);
		virtual ~TextComponent() = default;

		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;

		void OnNotify(const std::string& event, GameObject* gameObject) override;

		void Update() override;

		void SetSize(const unsigned int size);
		void SetText(const std::string& text);
		void SetColor(const SDL_Color& color);

	private:
		DisplayType m_displayType{ DisplayType::Default };

		bool m_needsUpdate{ true };
		std::string m_text;
		SDL_Color m_color{ 255, 255, 255, 255 };
		std::shared_ptr<Font> m_font;
		RenderComponent* m_renderComponent;
	};
}