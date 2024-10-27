#pragma once

#include <SFML/Graphics/Sprite.hpp>

#include "Core/ECB/Components/DrawableComponent.h"

namespace Core::ECB::Components
{
	class SpriteComponent : public DrawableComponent
	{
	public:
		SpriteComponent(Entity& p_owner);

		void SetTexture(const sf::Texture& p_texture);
		void SetPosition(const sf::Vector2f& p_position) override;
		const sf::Vector2f& GetPosition() const override;
		sf::FloatRect GetGlobalBounds() const override;

	private:
		void draw(sf::RenderTarget& p_target, sf::RenderStates p_states) const override;

	protected:
		sf::Sprite m_sprite;
	};
}
