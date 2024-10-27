#pragma once

#include <concepts>

#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "Core/ECB/Components/DrawableComponent.h"

namespace Core::ECB::Components
{
	template <typename T>
	concept IsShape = std::derived_from<T, sf::Shape>;

	//template <IsShape ShapeType>
	template <typename ShapeType>
	requires IsShape<ShapeType>
	class ShapeComponent : public DrawableComponent
	{
	public:
		ShapeComponent(Entity& p_owner) : DrawableComponent(p_owner)
		{
			m_shape.setPosition(m_owner.GetPosition());
		}

		ShapeType& GetShape()
		{
			return m_shape;
		}

		void SetPosition(const sf::Vector2f& p_position) override
		{
			m_shape.setPosition(p_position);
		}

		const sf::Vector2f& GetPosition() const override
		{
			return m_shape.getPosition();
		}

		sf::FloatRect GetGlobalBounds() const override
		{
			return m_shape.getGlobalBounds();
		}

	private:
		void draw(sf::RenderTarget& p_target, sf::RenderStates p_states) const override
		{
			p_target.draw(m_shape, p_states);
		}

	protected:
		ShapeType m_shape;
	};
}
