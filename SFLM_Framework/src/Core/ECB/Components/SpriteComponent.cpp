#include "Core/ECB/Components/SpriteComponent.h"

#include "Core/ECB/Entity.h"

#include "SFML/Graphics/RenderTarget.hpp"

Core::ECB::Components::SpriteComponent::SpriteComponent(Entity& p_owner) : DrawableComponent(p_owner)
{
	m_sprite.setPosition(m_owner.GetPosition());
}

void Core::ECB::Components::SpriteComponent::SetTexture(const sf::Texture& p_texture)
{
	m_sprite.setTexture(p_texture);
}

void Core::ECB::Components::SpriteComponent::SetPosition(const sf::Vector2f& p_position)
{
	m_sprite.setPosition(p_position);
}

const sf::Vector2f& Core::ECB::Components::SpriteComponent::GetPosition() const
{
	return m_sprite.getPosition();
}

sf::FloatRect Core::ECB::Components::SpriteComponent::GetGlobalBounds() const
{
	return m_sprite.getGlobalBounds();
}

void Core::ECB::Components::SpriteComponent::draw(sf::RenderTarget& p_target, sf::RenderStates p_states) const
{
	p_target.draw(m_sprite, p_states);
}
