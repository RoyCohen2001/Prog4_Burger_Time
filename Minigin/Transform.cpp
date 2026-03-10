#include "Transform.h"

namespace dae
{
	glm::vec3 Transform::GetWorldPosition()
	{
		if (m_positionIsDirty)
			UpdateWorldPosition();
		return m_worldPosition;
	}

	void Transform::SetLocalPosition(const glm::vec3& localPos)
	{
		m_localPosition = localPos;
		SetPositionDirty();
	}

	void Transform::SetPositionDirty()
	{
		m_positionIsDirty = true;
	}

	void Transform::UpdateWorldPosition()
	{
		if (m_parent == nullptr)
			m_worldPosition = m_localPosition;
		else
			m_worldPosition = m_parent->GetWorldPosition() + m_localPosition;

		m_positionIsDirty = false;
	}
}