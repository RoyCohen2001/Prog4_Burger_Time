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
		m_transform.SetPosition(x, y, 0.0f);
		m_localPosition = glm::vec3{ x, y, 0.0f };
		SetPositionDirty();
	}

	GameObject::~GameObject() = default;

	void GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
	{
		if (IsChild(parent) || parent == this || m_parent == parent)
			return;
		if (parent == nullptr) {
			SetLocalPosition(GetWorldPosition());
		}
		else
		{
			if (keepWorldPosition) {
				SetLocalPosition(GetWorldPosition() - parent->GetWorldPosition());
			}
			SetPositionDirty();
		}

		m_parent = parent; 
	}


	void GameObject::SetLocalPosition(const glm::vec3& pos)
	{
		m_localPosition = pos;
		SetPositionDirty();
	}

	glm::vec3 GameObject::GetWorldPosition() 
	{
		if (m_positionIsDirty) {
			UpdateWorldPosition();
		}
		return m_worldPosition;
	}

	void GameObject::UpdateWorldPosition()
	{
		if (m_positionIsDirty)
		{
			if (m_parent == nullptr)
				m_worldPosition = m_localPosition;
			else
				m_worldPosition = m_parent->GetWorldPosition() + m_localPosition;
		}
		m_positionIsDirty = false;
	}
	void GameObject::SetPositionDirty()
	{
		m_positionIsDirty = true;
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