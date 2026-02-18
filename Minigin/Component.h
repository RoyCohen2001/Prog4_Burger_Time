#pragma once

namespace dae
{
	class GameObject;

	class Component
	{
	public:
		Component(GameObject* pOwner) : m_pOwner(pOwner) {}
		GameObject* GetOwner() const { return m_pOwner; }

		virtual void Update() {}
		virtual void Render() const {}

		virtual void SetDelete() { m_delete = true; }
		virtual bool ShouldBeDeleted() const { return m_delete; }

		virtual ~Component() = default;
		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

	protected:
		GameObject* m_pOwner;

		bool m_delete{ false };
	};
}