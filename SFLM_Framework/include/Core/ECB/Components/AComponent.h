#pragma once

namespace Core::ECB
{
	class Entity;
}

namespace Core::ECB::Components
{
	class AComponent
	{
	public:
		AComponent(Entity& p_owner) : m_owner(p_owner)
		{
		}

		virtual ~AComponent() = default;

	protected:
		Entity& m_owner;
	};
}
