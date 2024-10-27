#pragma once

namespace Core::ECB
{
	class Entity;
}

namespace Core::ECB::Behaviors
{
	class ABehavior
	{
	public:
		ABehavior(Entity& p_owner) : m_owner(p_owner), m_isStarted(false)
		{
		}

		virtual ~ABehavior() = default;

		virtual void Start()
		{
		}

		virtual void Update(float p_deltaTime)
		{
		}

		virtual void OnDestroy()
		{
		}

		bool IsStarted() const { return m_isStarted; }
		void SetStarted(bool p_value) { m_isStarted = p_value; }

	protected:
		Entity& m_owner;
		bool m_isStarted;
	};
}
