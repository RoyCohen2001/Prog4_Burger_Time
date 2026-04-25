#include "GameObject.h"
#include "Component.h"

namespace dae
{
	void GameObject::Update()
	{
		for (const auto& component : m_components)
		{
			component->Update();
		}
		ProcessComponentRemoval();
		GetWorldPosition();
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
		m_transform.SetLocalPosition(glm::vec3{ x, y, 0.0f });
		SetPositionDirty();
	}

	GameObject::~GameObject() = default;

	void GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
	{
		if (IsChild(parent) || parent == this || m_parent == parent)
			return;

		if (parent == nullptr)
		{
			SetLocalPosition(GetWorldPosition());
			m_transform.SetParent(nullptr);
		}
		else
		{
			if (keepWorldPosition)
				SetLocalPosition(GetWorldPosition() - parent->GetWorldPosition());

			m_transform.SetParent(&parent->m_transform);
			SetPositionDirty();
		}

		// Not clear to me?
		if (m_parent) {
			m_parent->RemoveChild(this);
		}

		m_parent = parent;

		if (m_parent) {
			m_parent->AddChild(this);
		}
	}

	void GameObject::AddChild(GameObject* child)
	{
		m_children.push_back(child);
	}

	void GameObject::RemoveChild(GameObject* child)
	{
		m_children.erase(std::remove(m_children.begin(), m_children.end(), child), m_children.end());
	}

	void GameObject::SetLocalPosition(const glm::vec3& pos)
	{
		m_transform.SetLocalPosition(pos);
		SetPositionDirty();
	}

	glm::vec3 GameObject::GetWorldPosition()
	{
		return m_transform.GetWorldPosition();
	}

	void GameObject::SetPositionDirty()
	{
		m_transform.SetPositionDirty();
		for (auto* child : m_children)
		{
			child->SetPositionDirty();
		}
	}

	bool GameObject::IsChild(const GameObject* potentialChild) const
	{
		if (potentialChild == nullptr)
			return false;

		for (const auto* child : m_children)
		{
			if (child == potentialChild || child->IsChild(potentialChild))
				return true;
		}
		return false;
	}
}