#include "Core/ECB/GridEntity.h"

#include "Systems/GridSystem.h"

void Core::ECB::GridEntity::SetPosition(const sf::Vector2f& p_position)
{
	Entity::SetPosition(p_position);
	m_gridCoordinate = Systems::GridSystem::WorldToGrid(p_position);
}
