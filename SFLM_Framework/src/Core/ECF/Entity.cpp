#include "Core/ECB/Entity.h"

#include "Core/ECB/Components/DrawableComponent.h"

Core::ECB::Entity::Entity() : m_position(0.f, 0.f)
{
}

void Core::ECB::Entity::SetPosition(const sf::Vector2f& p_position)
{
	m_position = p_position;

	auto drawableComponent = GetComponent<Components::DrawableComponent>();

	if(drawableComponent!= nullptr)
	{
		drawableComponent->SetPosition(p_position);
	}
}

sf::Vector2f Core::ECB::Entity::GetPosition() const
{
	return m_position;
}
