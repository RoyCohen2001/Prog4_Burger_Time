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
{
}


void dae::RenderComponent::Render() const
{
	if (m_texture)
	{
		const auto pos = GetOwner()->GetWorldPosition();
		Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
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
