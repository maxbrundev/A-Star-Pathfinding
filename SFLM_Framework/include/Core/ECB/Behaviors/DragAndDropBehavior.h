#pragma once

#include <functional>
#include <SFML/System/Vector2.hpp>

#include "Core/ECB/Behaviors/ABehavior.h"

namespace Core::ECB::Components
{
	class DrawableComponent;
	class TagHandlerComponent;
}

namespace Core::ECB::Behaviors
{
	using DragStartCallback = std::function<void(Entity&)>;
	using DraggedCallback   = std::function<void(Entity&, const sf::Vector2f&)>;
	using DropCallback      = std::function<void(Entity&, const sf::Vector2f&)>;

	class DragAndDropBehavior : public ABehavior
	{
	public:
		DragAndDropBehavior(Entity& p_owner);

		void Start() override;
		void Update(float p_deltaTime) override;

		void SetOnDragStartCallback(const DragStartCallback& p_callBack) { m_onDragStart = p_callBack; }
		void SetOnDraggedCallback(const DraggedCallback& p_callBack) { m_onDragged = p_callBack; }
		void SetOnDropCallback(const DropCallback& p_callBack) { m_onDrop = p_callBack; }

	private:
		void HandleDragAndDrop();

	private:
		bool m_isDragging;

		Components::DrawableComponent* m_drawableComponent;
		Components::TagHandlerComponent* m_tagHandlerComponent;
		DragStartCallback m_onDragStart;
		DraggedCallback m_onDragged;
		DropCallback m_onDrop;
	};
}
