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
		glm::vec2 size{};

		if (m_useSourceRect)
		{
			size = { m_sourceRect.w * m_scale.x, m_sourceRect.h * m_scale.y };
			SDL_FRect dst{ pos.x, pos.y, size.x, size.y };
			SDL_RenderTexture(Renderer::GetInstance().GetSDLRenderer(), m_texture->GetSDLTexture(), &m_sourceRect, &dst);
		}
		else
		{
			const auto texSize = m_texture->GetSize();
			size = { texSize.x * m_scale.x, texSize.y * m_scale.y };
			Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y, size.x, size.y);
		}
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

	if (m_useSourceRect)
	{
		return { m_sourceRect.w * m_scale.x, m_sourceRect.h * m_scale.y };
	}

	const auto size = m_texture->GetSize();
	return { size.x * m_scale.x, size.y * m_scale.y };
}