#pragma once
#include "Component.h"
#include <string>
#include <unordered_map>
#include <glm/vec2.hpp>
#include <memory>

namespace dae
{
	class RenderComponent;
	class Texture2D;

	class AnimationComponent final : public Component
	{
	public:
		explicit AnimationComponent(GameObject* owner);

		void Update() override;

		void SetSpriteSheet(const std::string& texturePath, const glm::vec2& frameSize, int columns);
		void SetSpriteSheet(const std::string& texturePath, const glm::vec2& frameSize, int columns, const glm::vec2& originOffset);
		void AddClip(const std::string& name, int startFrame, int frameCount, float fps, bool loop = true);
		void Play(const std::string& name);

		const std::string& GetCurrentClip() const { return m_currentClip; }

	private:
		struct Clip final
		{
			int startFrame{};
			int frameCount{};
			float fps{};
			bool loop{};
		};

		void ApplyFrame(int frameIndex);

		RenderComponent* m_renderComponent{};
		std::shared_ptr<Texture2D> m_texture{};
		glm::vec2 m_frameSize{ 0.f, 0.f };
		glm::vec2 m_originOffset{ 0.f, 0.f };
		int m_columns{ 1 };

		std::unordered_map<std::string, Clip> m_clips{};
		std::string m_currentClip{};
		int m_currentFrame{ 0 };
		float m_time{ 0.f };
	};
}