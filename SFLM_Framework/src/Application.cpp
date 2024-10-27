#include "Application.h"

#include "Core/ECB/Behaviors/DragAndDropBehavior.h"
#include "Core/ECB/Components/ShapeComponent.h"

#include "Core/TagHandlerComponent.h"

#include "Tools/Global/ServiceLocator.h"

//TODO: Scene, Game, EntityManager classes need to be implemented to clean this messy Application.
Core::Application::Application(Context::Window& p_window, Inputs::InputManager& p_inputManager) :
m_window(p_window),
m_inputManager(p_inputManager)
{
	Tools::Global::ServiceLocator::Provide(m_window);
	Tools::Global::ServiceLocator::Provide(m_inputManager);

	m_gridSettings.Width  = 30;
	m_gridSettings.Height = 30;
	m_gridSystem = new Systems::GridSystem(p_window, m_gridSettings);

	m_startEntity = std::make_unique<ECB::GridEntity>();
	m_endEntity   = std::make_unique<ECB::GridEntity>();

	m_startEntity->SetPosition(Systems::GridSystem::GridToWorld(0, 0));
	m_endEntity->SetPosition(Systems::GridSystem::GridToWorld(m_gridSettings.Width - 1, m_gridSettings.Height - 1));

	auto startEntityShapeComponent = m_startEntity->AddComponent<ECB::Components::ShapeComponent<sf::RectangleShape>>();
	startEntityShapeComponent->GetShape().setSize(sf::Vector2f(Systems::GridSystem::CELL_SIZE, Systems::GridSystem::CELL_SIZE));
	startEntityShapeComponent->GetShape().setFillColor(sf::Color::Green);
	startEntityShapeComponent->GetShape().setOutlineThickness(1.0f);
	startEntityShapeComponent->GetShape().setOutlineColor(sf::Color::White);
	startEntityShapeComponent->GetShape().setOrigin(Systems::GridSystem::CELL_SIZE / 2.0f, Systems::GridSystem::CELL_SIZE / 2.0f);

	auto startEntityTagComponent = m_startEntity->AddComponent<ECB::Components::TagHandlerComponent>();
	startEntityTagComponent->InitTagSet<EGridEntityState>();

	auto endEntityShapeComponent = m_endEntity->AddComponent<ECB::Components::ShapeComponent<sf::RectangleShape>>();
	endEntityShapeComponent->GetShape().setSize(sf::Vector2f(Systems::GridSystem::CELL_SIZE, Systems::GridSystem::CELL_SIZE));
	endEntityShapeComponent->GetShape().setFillColor(sf::Color::Blue);
	endEntityShapeComponent->GetShape().setOutlineThickness(1.0f);
	endEntityShapeComponent->GetShape().setOutlineColor(sf::Color::White);
	endEntityShapeComponent->GetShape().setOrigin(Systems::GridSystem::CELL_SIZE / 2.0f, Systems::GridSystem::CELL_SIZE / 2.0f);

	auto endEntityTagComponent = m_endEntity->AddComponent<ECB::Components::TagHandlerComponent>();
	endEntityTagComponent->InitTagSet<EGridEntityState>();

	auto& drag = m_startEntity->AddBehavior<ECB::Behaviors::DragAndDropBehavior>();
	drag.SetOnDropCallback([&](ECB::Entity& entity, const sf::Vector2<float>& vector2)
	{
		OnDropEntity(entity, vector2);
	});

	drag.SetOnDragStartCallback([&](ECB::Entity& entity)
	{
		ECB::GridEntity* test = dynamic_cast<ECB::GridEntity*>(&entity);
		Cell* cell = m_gridSystem->GetCell(test->GetGridCoordinate().X, test->GetGridCoordinate().Y);
		cell->CurrentEntity = nullptr;
	});

	auto& drag2 = m_endEntity->AddBehavior<ECB::Behaviors::DragAndDropBehavior>();
	drag2.SetOnDropCallback([&](ECB::Entity& entity, const sf::Vector2<float>& vector2)
	{
		OnDropEntity(entity, vector2);
	});

	drag2.SetOnDragStartCallback([&](ECB::Entity& entity)
	{
		ECB::GridEntity* test = dynamic_cast<ECB::GridEntity*>(&entity);
		Cell* cell = m_gridSystem->GetCell(test->GetGridCoordinate().X, test->GetGridCoordinate().Y);
		cell->CurrentEntity = nullptr;
	});

	m_pathFinding = new Systems::AStar(m_gridSettings);

	m_path.reserve(static_cast<size_t>(m_gridSettings.Width * m_gridSettings.Height));

	SetAnimationDelay(0.01f);

	m_startEntity->StartBehaviors();
	m_endEntity->StartBehaviors();
}

void Core::Application::Update(float p_deltaTime)
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*m_window.GetWindow());
	m_worldMousePosition = m_window.GetWindow()->mapPixelToCoords(mousePosition);
	m_hoveredGridCoordinate = Systems::GridSystem::WorldToGrid(m_worldMousePosition);

	HandleWallInputs();
	HandlePathInput();
	HandleMoveInput();

	MoveEntityAlongPath(p_deltaTime);

	m_startEntity->Update(p_deltaTime);
	m_endEntity->Update(p_deltaTime);
	
	if (m_isPathfinding && m_pathfindingClock.getElapsedTime().asSeconds() >= m_pathAnimationDelay)
	{
		m_pathfindingClock.restart();
		if (m_pathFinding->Step(m_obstacles))
		{
			m_isPathfinding = false;
			m_path = m_pathFinding->GetPath();
		}
	}

	const auto& checkedTiles = m_pathFinding->GetCheckedTiles();
	const auto& openTiles = m_pathFinding->GetOpenTiles();

	for (int i = 0; i < m_gridSettings.Width; i++)
	{
		for (int j = 0; j < m_gridSettings.Height; j++)
		{
			Cell* cell = m_gridSystem->GetCell(i, j);

			if (cell != nullptr)
			{
				if (m_obstacles.find(cell->GridCoordinate) != m_obstacles.end())
				{
					cell->Tile.setFillColor(sf::Color(128, 128, 128));
				}
				else if (std::find(checkedTiles.begin(), checkedTiles.end(), cell->GridCoordinate) != checkedTiles.end())
				{
					cell->Tile.setFillColor(sf::Color(100, 100, 255));
				}
				else
				{
					cell->Tile.setFillColor(sf::Color(200, 200, 180));
				}
			}

		}
	}

	for (const GridCoordinate& gridCoordinate : openTiles)
	{
		m_gridSystem->GetCell(gridCoordinate.X, gridCoordinate.Y)->Tile.setFillColor(sf::Color(255, 165, 0));
	}

	for (const GridCoordinate& gridCoordinate : m_path)
	{
		m_gridSystem->GetCell(gridCoordinate.X, gridCoordinate.Y)->Tile.setFillColor(sf::Color(0, 155, 0));
	}
}

void Core::Application::Render() const
{
	for (int i = 0; i < m_gridSettings.Width; i++)
	{
		for (int j = 0; j < m_gridSettings.Height; j++)
		{
			auto& cell = *m_gridSystem->GetCell(i, j);

			m_window.Draw(cell.Tile);
		}
	}

	m_window.Draw(*m_startEntity->GetComponent<ECB::Components::DrawableComponent>());
	m_window.Draw(*m_endEntity->GetComponent<ECB::Components::DrawableComponent>());
}

void Core::Application::SetAnimationDelay(float p_speed)
{
	if (p_speed < 0.0f)
	{
		p_speed = 0.0f;
	}

	m_pathAnimationDelay = p_speed;
}

void Core::Application::SetWall(const GridCoordinate& p_gridCoordinate)
{
	if (p_gridCoordinate.X >= 0 && p_gridCoordinate.X < m_gridSettings.Width && p_gridCoordinate.Y >= 0 && p_gridCoordinate.Y < m_gridSettings.Height)
	{
		if (m_obstacles.find(p_gridCoordinate) == m_obstacles.end())
		{
			m_path.clear();
			m_pathFinding->Clear();
			m_obstacles.insert(p_gridCoordinate);

			auto tagComponent = m_startEntity->GetComponent<ECB::Components::TagHandlerComponent>();

			if (tagComponent->HasAllTags(EGridEntityState::MOVING))
			{
				tagComponent->ClearTags(EGridEntityState::MOVING);
				m_startEntity->SetPosition(Systems::GridSystem::GridToWorld(m_previousStartCoordinate.X, m_previousStartCoordinate.Y));
			}
		}
	}
}

void Core::Application::RemoveWall(const GridCoordinate& p_gridCoordinate)
{
	if (p_gridCoordinate.X >= 0 && p_gridCoordinate.X < m_gridSettings.Width && p_gridCoordinate.Y >= 0 && p_gridCoordinate.Y < m_gridSettings.Height)
	{
		if (m_obstacles.find(p_gridCoordinate) != m_obstacles.end())
		{
			m_path.clear();
			m_pathFinding->Clear();
			m_obstacles.erase(p_gridCoordinate);

			auto tagComponent = m_startEntity->GetComponent<ECB::Components::TagHandlerComponent>();

			if (tagComponent->HasAllTags(EGridEntityState::MOVING))
			{
				tagComponent->ClearTags(EGridEntityState::MOVING);
				m_startEntity->SetPosition(Systems::GridSystem::GridToWorld(m_previousStartCoordinate.X, m_previousStartCoordinate.Y));
			}
		}
	}
}

void Core::Application::HandleWallInputs()
{
	if (m_inputManager.GetMouseButtonState(Inputs::EMouseButton::RIGHT) == Inputs::EMouseButtonState::MOUSE_DOWN)
	{
		SetWall(m_hoveredGridCoordinate);
	}
	else if (m_inputManager.GetMouseButtonState(Inputs::EMouseButton::LEFT) == Inputs::EMouseButtonState::MOUSE_DOWN)
	{
		auto tagComponent = m_startEntity->GetComponent<ECB::Components::TagHandlerComponent>();
		auto tagComponentEndEnity = m_endEntity->GetComponent<ECB::Components::TagHandlerComponent>();

		if (tagComponent->HasAllTags(EGridEntityState::DRAGGING) || tagComponentEndEnity->HasAllTags(EGridEntityState::DRAGGING))
			return;
		
		RemoveWall(m_hoveredGridCoordinate);
	}
}

void Core::Application::RetrieveAvailableNeighbor(Cell* p_cell, GridCoordinate& p_gridCoordinate, std::unordered_set<GridCoordinate, GridCoordinateHash>& p_visitedGridCoordinates)
{
	//Depth-First Search
	/*if (p_cell == nullptr)
		return;

	if (p_visitedGridCoordinates.find(p_cell->GridCoordinate) != p_visitedGridCoordinates.end())
		return;

	p_visitedGridCoordinates.insert(p_cell->GridCoordinate);

	if (m_walls.find(p_cell->GridCoordinate) != m_walls.end())
	{
		RetrieveAvailableNeighbor(p_cell->Neighbors[LEFT_NEIGHBOR], p_gridCoordinate, p_visitedGridCoordinates);
		RetrieveAvailableNeighbor(p_cell->Neighbors[RIGHT_NEIGHBOR], p_gridCoordinate, p_visitedGridCoordinates);
		RetrieveAvailableNeighbor(p_cell->Neighbors[TOP_NEIGHBOR], p_gridCoordinate, p_visitedGridCoordinates);
		RetrieveAvailableNeighbor(p_cell->Neighbors[BOTTOM_NEIGHBOR], p_gridCoordinate, p_visitedGridCoordinates);
	}
	else
	{
		p_gridCoordinate = p_cell->GridCoordinate;
	}*/

	//Breadth-First Search
	if (p_cell == nullptr)
		return;

	std::queue<Cell*> cellQueue;
	cellQueue.push(p_cell);

	while (!cellQueue.empty())
	{
		Cell* currentCell = cellQueue.front();
		cellQueue.pop();

		if (currentCell == nullptr || p_visitedGridCoordinates.find(currentCell->GridCoordinate) != p_visitedGridCoordinates.end())
			continue;

		p_visitedGridCoordinates.insert(currentCell->GridCoordinate);

		if (m_obstacles.find(currentCell->GridCoordinate) == m_obstacles.end() && currentCell->CurrentEntity == nullptr)
		{
			p_gridCoordinate = currentCell->GridCoordinate;
			return;
		}

		if (currentCell->Neighbors[LEFT_NEIGHBOR] != nullptr)
			cellQueue.push(currentCell->Neighbors[LEFT_NEIGHBOR]);

		if (currentCell->Neighbors[RIGHT_NEIGHBOR] != nullptr)
			cellQueue.push(currentCell->Neighbors[RIGHT_NEIGHBOR]);

		if (currentCell->Neighbors[TOP_NEIGHBOR] != nullptr)
			cellQueue.push(currentCell->Neighbors[TOP_NEIGHBOR]);

		if (currentCell->Neighbors[BOTTOM_NEIGHBOR] != nullptr)
			cellQueue.push(currentCell->Neighbors[BOTTOM_NEIGHBOR]);
	}
}

void Core::Application::OnDropEntity(ECB::Entity& p_entity, sf::Vector2f  pos)
{
	GridCoordinate gridCoordinate = Systems::GridSystem::WorldToGrid(pos);

	if (m_gridSystem->IsInsideGrid(gridCoordinate))
	{
		auto testt = m_gridSystem->GetCell(gridCoordinate.X, gridCoordinate.Y);
		if (m_obstacles.find(gridCoordinate) != m_obstacles.end() || testt->CurrentEntity != nullptr)
		{
			if (auto cell = m_gridSystem->GetCell(gridCoordinate.X, gridCoordinate.Y))
			{
				std::unordered_set<GridCoordinate, GridCoordinateHash> visitedGridCoordinates;
				RetrieveAvailableNeighbor(cell, gridCoordinate, visitedGridCoordinates);
			}
		}

		m_gridSystem->GetCell(gridCoordinate.X, gridCoordinate.Y)->CurrentEntity = &p_entity;
		sf::Vector2f snappedPosition = Systems::GridSystem::GridToWorld(gridCoordinate.X, gridCoordinate.Y);

		auto tagComponent = p_entity.GetComponent<ECB::Components::TagHandlerComponent>();
		

		p_entity.SetPosition(snappedPosition);
		tagComponent->ClearTags(EGridEntityState::DRAGGING);
	}
}

void Core::Application::HandlePathInput()
{
	if (m_inputManager.IsKeyPressed(Inputs::EKey::P))
	{
		auto tagComponent = m_startEntity->GetComponent<ECB::Components::TagHandlerComponent>();

		if(tagComponent->HasAllTags(EGridEntityState::MOVING))
			return;

		ComputePath();
	}
}

void Core::Application::ComputePath()
{
	GridCoordinate start = Systems::GridSystem::WorldToGrid(m_startEntity->GetPosition());
	GridCoordinate goal  = Systems::GridSystem::WorldToGrid(m_endEntity->GetPosition());

	if (m_gridSystem->IsInsideGrid(start) && m_gridSystem->IsInsideGrid(goal)
		&& m_obstacles.find(start) == m_obstacles.end() && m_obstacles.find(goal) == m_obstacles.end())
	{
		if (m_pathAnimationDelay > 0.0f)
		{
			m_pathFinding->Clear();
			m_path.clear();
			m_pathFinding->Initialize(start, goal);

			m_isPathfinding = true;
		}
		else
		{
			m_path = m_pathFinding->FindPath(start, goal, m_obstacles);
		}
	}
	else
	{
		m_path.clear();
	}
}

void Core::Application::HandleMoveInput()
{
	if (m_inputManager.IsKeyPressed(Inputs::EKey::SPACE))
	{
		auto tagComponent = m_startEntity->GetComponent<ECB::Components::TagHandlerComponent>();

		if (!m_path.empty() && !tagComponent->HasAllTags(EGridEntityState::MOVING))
		{
			tagComponent->AddTags(EGridEntityState::MOVING);
			m_currentPathIndex = 0;
			m_previousStartCoordinate = Systems::GridSystem::WorldToGrid(m_startEntity->GetPosition());
		}
	}
}

void Core::Application::MoveEntityAlongPath(float p_deltaTime)
{
	auto tagComponent = m_startEntity->GetComponent<ECB::Components::TagHandlerComponent>();

	if (tagComponent->HasAllTags(EGridEntityState::MOVING) && m_currentPathIndex < m_path.size())
	{
		sf::Vector2f currentPosition = m_startEntity->GetPosition();
		GridCoordinate targetGridCoordinate = m_path[m_currentPathIndex];
		sf::Vector2f targetPosition = Systems::GridSystem::GridToWorld(targetGridCoordinate.X, targetGridCoordinate.Y);

		sf::Vector2f direction = targetPosition - currentPosition;

		float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

		if (distance > 0.0f)
		{
			sf::Vector2f normalizedDirection = direction / distance;
			sf::Vector2f movement = normalizedDirection * m_moveSpeed * p_deltaTime;

			if (std::sqrt(movement.x * movement.x + movement.y * movement.y) > distance)
			{
				movement = direction;
			}

			m_startEntity->SetPosition(currentPosition + movement);

			if (std::sqrt((targetPosition - m_startEntity->GetPosition()).x * (targetPosition - m_startEntity->GetPosition()).x +
				(targetPosition - m_startEntity->GetPosition()).y * (targetPosition - m_startEntity->GetPosition()).y) < 1.0f)
			{
				m_startEntity->SetPosition(targetPosition);
				m_currentPathIndex++;
			}
		}
		else
		{
			m_currentPathIndex++;
		}

		if (m_currentPathIndex >= m_path.size())
		{
			tagComponent->ClearTags(EGridEntityState::MOVING);
		}
	}
}
