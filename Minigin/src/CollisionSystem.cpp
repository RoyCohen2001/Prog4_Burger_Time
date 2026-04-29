#include "CollisionSystem.h"
#include "CollisionComponent.h"
#include "Renderer.h"

#include <SDL3/SDL.h>

namespace dae
{
	void CollisionSystem::Update()
	{
		const auto& colliders = CollisionComponent::GetAllColliders();

		for (size_t i = 0; i < colliders.size(); ++i)
		{
			for (size_t j = i + 1; j < colliders.size(); ++j)
			{
				auto* a = colliders[i];
				auto* b = colliders[j];
				if (!a || !b)
					continue;

				[[maybe_unused]] const bool overlapping = a->IsOverlapping(*b);
			}
		}
	}

	void CollisionSystem::RenderDebug()
	{
		auto* renderer = Renderer::GetInstance().GetSDLRenderer();
		if (!renderer)
			return;

		const auto& colliders = CollisionComponent::GetAllColliders();

		for (const auto* collider : colliders)
		{
			if (!collider)
				continue;

			const auto box = collider->GetAABB();

			SDL_FRect rect{};
			rect.x = box.left;
			rect.y = box.top;
			rect.w = box.right - box.left;
			rect.h = box.bottom - box.top;

			// Green = solid, Yellow = trigger
			if (collider->IsTrigger())
				SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
			else
				SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

			SDL_RenderRect(renderer, &rect);
		}
	}
}