#pragma once

#include "Core/ECB/Entity.h"

#include "Systems/GridCoordinate.h"

namespace Core::ECB
{
	class GridEntity : public ECB::Entity
	{
	public:
		GridEntity() = default;
		virtual ~GridEntity() override = default;

		void SetPosition(const sf::Vector2f& p_position) override;

		GridCoordinate& GetGridCoordinate() { return m_gridCoordinate; }

	private:
		GridCoordinate m_gridCoordinate;
	};
}
