#pragma once

namespace dae
{
	class GameObject;

	class Component
	{
	public:
		Component(GameObject* pOwner) : m_pOwner(pOwner) {}
		virtual ~Component() = default;

		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

		virtual void Update(float deltaTime) { (void)deltaTime; }
		virtual void Render() const {}

		GameObject* GetOwner() const { return m_pOwner; }

	protected:
		GameObject* m_pOwner;
	};
}