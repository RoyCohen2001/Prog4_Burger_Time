#include "GameObject.h"
#include "Component.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Texture2D.h"

namespace dae
{
	void GameObject::Update()
	{
		for (const auto& component : m_components)
		{
			component->Update();
		}

	   ProcessComponentRemoval();
		
	}

	void GameObject::Render() const
	{
		for (const auto& component : m_components)
		{
			component->Render();
		}
	}

	void GameObject::SetPosition(float x, float y)
	{
		m_transform.SetPosition(x, y, 0.0f);
	}

	GameObject::~GameObject() = default;
}