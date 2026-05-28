#include "AnimationComponent.h"
#include "RenderComponent.h"
#include "ResourceManager.h"
#include "Texture2D.h"
#include "TimeManager.h"
#include "GameObject.h"

#include <SDL3/SDL.h>
#include <algorithm>

dae::AnimationComponent::AnimationComponent(GameObject* owner)
	: Component(owner)
{
	m_renderComponent = owner->GetComponent<RenderComponent>();
}

void dae::AnimationComponent::SetSpriteSheet(const std::string& texturePath, const glm::vec2& frameSize, int columns)
{
	SetSpriteSheet(texturePath, frameSize, columns, { 0.f, 0.f });
}

void dae::AnimationComponent::SetSpriteSheet(const std::string& texturePath, const glm::vec2& frameSize, int columns, const glm::vec2& originOffset)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(texturePath);
	m_frameSize = frameSize;
	m_originOffset = originOffset;
	m_columns = std::max(1, columns);

	if (!m_renderComponent)
	{
		m_renderComponent = GetOwner()->AddComponent<RenderComponent>();
	}

	m_renderComponent->SetTexture(m_texture);
}

void dae::AnimationComponent::AddClip(const std::string& name, int startFrame, int frameCount, float fps, bool loop)
{
	m_clips[name] = Clip{ startFrame, frameCount, fps, loop };
}

void dae::AnimationComponent::Play(const std::string& name)
{
	if (m_currentClip == name)
		return;

	auto it = m_clips.find(name);
	if (it == m_clips.end())
		return;

	m_currentClip = name;
	m_currentFrame = 0;
	m_time = 0.f;

	ApplyFrame(it->second.startFrame);
}

void dae::AnimationComponent::Update()
{
	if (m_currentClip.empty() || !m_renderComponent)
		return;

	const auto it = m_clips.find(m_currentClip);
	if (it == m_clips.end())
		return;

	const Clip& clip = it->second;
	const float dt = TimeManager::GetInstance().GetDeltaTime();

	m_time += dt;

	const float frameTime = 1.f / std::max(clip.fps, 0.0001f);
	while (m_time >= frameTime)
	{
		m_time -= frameTime;
		m_currentFrame++;

		if (m_currentFrame >= clip.frameCount)
		{
			if (clip.loop)
				m_currentFrame = 0;
			else
				m_currentFrame = clip.frameCount - 1;
		}
	}

	ApplyFrame(clip.startFrame + m_currentFrame);
}

void dae::AnimationComponent::ApplyFrame(int frameIndex)
{
	const int col = frameIndex % m_columns;
	const int row = frameIndex / m_columns;

	SDL_FRect src{};
	src.x = m_originOffset.x + m_frameSize.x * col;
	src.y = m_originOffset.y + m_frameSize.y * row;
	src.w = m_frameSize.x;
	src.h = m_frameSize.y;

	m_renderComponent->SetSourceRect(src);
}