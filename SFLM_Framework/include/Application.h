#pragma once

#include "Context/Window.h"
#include "Core/ECB/Entity.h"
#include "Core/ECB/GridEntity.h"
#include "Inputs/InputManager.h"
#include "Systems/AStar.h"
#include "Systems/GridSystem.h"

namespace Core
{
	class Application
	{
	public:
		Application(Context::Window& p_window, Inputs::InputManager& p_inputManager);
		~Application() = default;

		void Update(float p_deltaTime);
		void Render() const;
		
		void SetAnimationDelay(float p_speed);

	private:
		void SetWall(const GridCoordinate& p_gridCoordinate);
		void RemoveWall(const GridCoordinate& p_gridCoordinate);
		void HandleWallInputs();
		void HandlePathInput();
		void ComputePath();
		void HandleMoveInput();
		void MoveEntityAlongPath(float p_deltaTime);
		void RetrieveAvailableNeighbor(Cell* p_cell, GridCoordinate& p_gridCoordinate, std::unordered_set<GridCoordinate, GridCoordinateHash>& p_visitedGridCoordinates);
		void OnDropEntity(ECB::Entity& p_entity, sf::Vector2f p_position);

	private:
		GridSettings m_gridSettings;

		Context::Window&      m_window;
		Inputs::InputManager& m_inputManager;
		Systems::GridSystem*  m_gridSystem;
		Systems::AStar*       m_pathFinding;

		std::unique_ptr<ECB::GridEntity> m_startEntity;
		std::unique_ptr<ECB::GridEntity> m_endEntity;

		std::vector<GridCoordinate> m_path;
		std::unordered_set<GridCoordinate, GridCoordinateHash> m_obstacles;

		sf::Vector2f m_worldMousePosition;

		GridCoordinate m_previousStartCoordinate;
		GridCoordinate m_hoveredGridCoordinate;

		sf::Clock m_pathfindingClock;

		float m_pathAnimationDelay = 0.0f;
		float m_moveSpeed = 200.0f;

		int m_currentPathIndex = 0;

		bool m_isPathfinding = false;
	};
}
