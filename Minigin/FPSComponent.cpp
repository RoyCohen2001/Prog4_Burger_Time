#include "FPSComponent.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "TimeManager.h"
#include <numeric>

dae::FPSComponent::FPSComponent(GameObject* owner)
	: Component(owner),
	m_frames(0),
	m_fps(0),
	m_accumulatedTime(0.0f),
	m_updateInterval(0.5f),
	m_timeSinceLastUpdate(0.0f)
{
	m_textComponent = owner->GetComponent<TextComponent>();
}

void dae::FPSComponent::Update()
{
	m_accumulatedTime += TimeManager::GetInstance().GetDeltaTime();
	++m_frames;

	if (m_frames >= MAX_SAMPLES)
	{
		m_fps = m_frames / m_accumulatedTime;
		m_frames = 0;
		m_accumulatedTime = 0.0f;
	}

	m_timeSinceLastUpdate += TimeManager::GetInstance().GetDeltaTime();
	if (m_timeSinceLastUpdate >= m_updateInterval && m_textComponent)
	{
		m_textComponent->SetText(std::to_string(static_cast<int>(m_fps)));
		m_timeSinceLastUpdate = 0.0f;
	}
}
