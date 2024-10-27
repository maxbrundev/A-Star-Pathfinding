#include "Core/ECB/Behaviors/DragAndDropBehavior.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>

#include "Context/Window.h"
#include "Core/TagHandlerComponent.h"
#include "Core/ECB/Entity.h"
#include "Core/ECB/Components/DrawableComponent.h"
#include "Inputs/EMouseButton.h"
#include "Inputs/InputManager.h"
#include "Tools/Global/ServiceLocator.h"

Core::ECB::Behaviors::DragAndDropBehavior::DragAndDropBehavior(Entity& p_owner) : ABehavior(p_owner),
	m_isDragging(false), m_drawableComponent(nullptr), m_tagHandlerComponent(nullptr)
{
}

void Core::ECB::Behaviors::DragAndDropBehavior::Start()
{
	m_drawableComponent = m_owner.GetComponent<Components::DrawableComponent>();
	m_tagHandlerComponent = m_owner.GetComponent<Components::TagHandlerComponent>();
}

void Core::ECB::Behaviors::DragAndDropBehavior::Update(float p_deltaTime)
{
	HandleDragAndDrop();
}

void Core::ECB::Behaviors::DragAndDropBehavior::HandleDragAndDrop()
{
	if (m_drawableComponent == nullptr || m_tagHandlerComponent == nullptr)
		return;

	auto& window = Tools::Global::ServiceLocator::Get<Context::Window>();
	auto& inputManager = Tools::Global::ServiceLocator::Get<Inputs::InputManager>();
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*window.GetWindow());
	sf::Vector2f worldMousePosition = window.GetWindow()->mapPixelToCoords(mousePosition);
	

	if (inputManager.IsMouseButtonPressed(Inputs::EMouseButton::LEFT))
	{
		if (m_drawableComponent->GetGlobalBounds().contains(worldMousePosition) && !m_isDragging)
		{
			m_tagHandlerComponent->AddTags(EGridEntityState::DRAGGING);

			if (m_onDragStart)
			{
				m_onDragStart(m_owner);
			}

			m_isDragging = true;
		}
	}

	if (m_isDragging)
	{
		m_owner.SetPosition(worldMousePosition);
	}

	if (inputManager.IsMouseButtonReleased(Inputs::EMouseButton::LEFT))
	{
		if (m_isDragging)
		{
			m_isDragging = false;
			
			m_tagHandlerComponent->RemoveTags(EGridEntityState::DRAGGING);

			if (m_onDrop)
			{
				m_onDrop(m_owner, m_owner.GetPosition());
			}
		}
	}
}
