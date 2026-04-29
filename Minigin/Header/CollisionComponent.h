#pragma once
#include "Component.h"
#include <glm/vec2.hpp>
#include <vector>

namespace dae
{
	enum class CollisionLayer
	{
		Default,
		Player,
		Enemy,
		Platform,
		Ladder,
		Trigger
	};

	struct AABB final
	{
		float left{};
		float top{};
		float right{};
		float bottom{};

		bool Intersects(const AABB& other) const
		{
			return !(right <= other.left || left >= other.right || bottom <= other.top || top >= other.bottom);
		}
	};

	class CollisionComponent final : public Component
	{
	public:
		CollisionComponent(
			GameObject* owner,
			const glm::vec2& size = glm::vec2{ 0.f, 0.f },
			const glm::vec2& offset = glm::vec2{ 0.f, 0.f },
			CollisionLayer layer = CollisionLayer::Default,
			bool isTrigger = false);

		~CollisionComponent() override;

		void SetSize(const glm::vec2& size) { m_size = size; }
		void SetOffset(const glm::vec2& offset) { m_offset = offset; }
		void SetTrigger(bool isTrigger) { m_isTrigger = isTrigger; }
		void SetLayer(CollisionLayer layer) { m_layer = layer; }

		const glm::vec2& GetSize() const { return m_size; }
		const glm::vec2& GetOffset() const { return m_offset; }
		bool IsTrigger() const { return m_isTrigger; }
		CollisionLayer GetLayer() const { return m_layer; }

		AABB GetAABB() const;

		float GetCenterX() const;
		float GetCenterY() const;
		float GetFeetY() const;

		bool IsOverlapping(const CollisionComponent& other) const;
		std::vector<CollisionComponent*> GetOverlaps(bool includeTriggers = true) const;
		std::vector<CollisionComponent*> GetOverlapsByLayer(CollisionLayer layer, bool includeTriggers = true) const;

		static const std::vector<CollisionComponent*>& GetAllColliders() { return s_AllColliders; }

	private:
		glm::vec2 m_size{};
		glm::vec2 m_offset{};
		bool m_isTrigger{ false };
		CollisionLayer m_layer{ CollisionLayer::Default };

		static std::vector<CollisionComponent*> s_AllColliders;
	};
}