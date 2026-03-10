#pragma once
#include <glm/glm.hpp>

namespace dae
{
	class Transform final
	{
	public:
		const glm::vec3& GetLocalPosition() const { return m_localPosition; }
		glm::vec3 GetWorldPosition();

		void SetLocalPosition(const glm::vec3& localPos);
		void SetParent(Transform* parent) { m_parent = parent; }
		void SetPositionDirty();

	private:
		void UpdateWorldPosition();

		Transform* m_parent{ nullptr };
		glm::vec3 m_localPosition{};
		glm::vec3 m_worldPosition{};
		bool m_positionIsDirty{ true };
	};
}