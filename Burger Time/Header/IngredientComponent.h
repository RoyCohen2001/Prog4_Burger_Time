#pragma once
#include "Component.h"
#include "CollisionComponent.h"
#include <glm/vec3.hpp>
#include <vector>

namespace dae
{
	class IngredientComponent final : public Component
	{
	public:
		IngredientComponent(GameObject* owner);
		~IngredientComponent() override;

		size_t RegisterPiece(GameObject* piece);
		void OnPieceHit(size_t index);
		void Update() override;

		bool IsFalling() const { return m_isFalling; }
		bool IsOnPlate() const { return m_onPlate; }

	private:
		struct PieceInfo final
		{
			GameObject* object{};
			CollisionComponent* collider{};
			glm::vec3 baseLocalPos{};
			bool hit{};
		};

		void StartFall();
		void ResetPieces();
		bool AllPiecesHit() const;
		AABB GetBounds() const;
		void TriggerBelowFall();

		std::vector<PieceInfo> m_pieces{};
		bool m_isFalling{};
		bool m_onPlate{};
		bool m_targetIsPlate{};
		float m_targetTopY{};
		float m_fallSpeed{ 180.f };

		static std::vector<IngredientComponent*> s_ingredients;
	};

	class IngredientPieceComponent final : public Component
	{
	public:
		IngredientPieceComponent(GameObject* owner, IngredientComponent* parent, size_t index);
		void Update() override;
		void ResetHit() { m_hit = false; }

	private:
		IngredientComponent* m_parent{};
		size_t m_index{};
		CollisionComponent* m_collision{};
		bool m_hit{};
	};
}