#include "RenderComponent.h"
#include "Texture2D.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "GameObject.h"

dae::RenderComponent::RenderComponent(GameObject* owner, const std::string& filename)
	: Component(owner)
{
	SetTexture(filename);
}

dae::RenderComponent::RenderComponent(GameObject* owner)
	: Component(owner)
{}

void dae::RenderComponent::Render() const
{
	if (m_texture)
	{
		const auto pos = GetOwner()->GetWorldPosition();
		const auto size = m_texture->GetSize();

		Renderer::GetInstance().RenderTexture(
			*m_texture,
			pos.x,
			pos.y,
			size.x * m_scale.x,
			size.y * m_scale.y
		);
	}
}

void dae::RenderComponent::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::RenderComponent::SetTexture(const std::shared_ptr<Texture2D> texture)
{
	m_texture = texture;
}

glm::vec2 dae::RenderComponent::GetScaledSize() const
{
	if (!m_texture)
		return { 0.f, 0.f };

	const auto size = m_texture->GetSize();
	return { size.x * m_scale.x, size.y * m_scale.y };
}