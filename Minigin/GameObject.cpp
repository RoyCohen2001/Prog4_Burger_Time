#include "GameObject.h"
#include "Component.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Texture2D.h"

namespace dae
{
	void GameObject::Update(float deltaTime)
	{
		for (const auto& component : m_components)
		{
			component->Update(deltaTime);
		}
	}

	void GameObject::Render() const
	{
		if (m_texture != nullptr)
		{
			const auto& pos = m_transform.GetPosition();
			Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
		}

		for (const auto& component : m_components)
		{
			component->Render();
		}
	}

	void GameObject::SetTexture(const std::string& filename)
	{
		m_texture = ResourceManager::GetInstance().LoadTexture(filename);
	}

	void GameObject::SetPosition(float x, float y)
	{
		m_transform.SetPosition(x, y, 0.0f);
	}

	GameObject::~GameObject() = default;
}