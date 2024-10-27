#pragma once

#include "Core/ECB/Components/AComponent.h"

namespace Core::ECB::Components
{
	class DrawableComponent : public AComponent, public sf::Drawable
	{
	public:
		DrawableComponent(Entity& p_owner) : AComponent(p_owner)
		{
		}

		virtual ~DrawableComponent() override = default;

		virtual void SetPosition(const sf::Vector2f& p_position) = 0;
		virtual const sf::Vector2f& GetPosition() const          = 0;
		virtual sf::FloatRect GetGlobalBounds() const            = 0;
		
	};
}
