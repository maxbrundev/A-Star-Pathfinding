#pragma once

#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Systems/GridCoordinate.h"
#include "Systems/GridSettings.h"

namespace Systems
{
	constexpr int NeighborsDirection[4][2] = 
	{
		{ 1,  0},
		{-1,  0},
		{ 0,  1},
		{ 0, -1} 
	};

	struct QueueItem
	{
		GridCoordinate Coordinate;
		float Priority;

		bool operator>(const QueueItem& p_other) const
		{
			return Priority > p_other.Priority;
		}
	};

	class AStar
	{
	public:
		AStar(const GridSettings& p_gridSettings);
		~AStar() = default;
		void Initialize(const GridCoordinate& p_start, const GridCoordinate& p_goal);
		void Clear();
		std::vector<GridCoordinate> FindPath(const GridCoordinate& p_start, const GridCoordinate& p_goal, const std::unordered_set<GridCoordinate, GridCoordinateHash>& p_obstacles);

		inline const std::vector<GridCoordinate>& GetPath() const { return m_path; }
		inline const std::vector<GridCoordinate>& GetCheckedTiles() const { return m_checkedTiles; }
		inline const std::unordered_set<GridCoordinate, GridCoordinateHash>& GetOpenTiles() const { return m_openSet; }

		bool Step(const std::unordered_set<GridCoordinate, GridCoordinateHash>& p_obstacles);

	private:
		float Heuristic(const GridCoordinate& p_a, const GridCoordinate& p_b) const;
		void ComputeNeighbors(const GridCoordinate& p_gridCoordinate, const std::unordered_set<GridCoordinate, GridCoordinateHash>& p_obstacles);

	private:
		GridSettings m_settings;
		GridCoordinate m_startCoordinate;
		GridCoordinate m_goalCoordinate;

		std::priority_queue<QueueItem, std::vector<QueueItem>, std::greater<>> m_frontier;
		std::unordered_map<GridCoordinate, GridCoordinate, GridCoordinateHash> m_cameFrom;
		std::unordered_map<GridCoordinate, float, GridCoordinateHash>          m_costSoFar;

		std::vector<GridCoordinate> m_neighborsBuffer;

		std::vector<GridCoordinate> m_checkedTiles;
		std::vector<GridCoordinate> m_path;
		std::unordered_set<GridCoordinate, GridCoordinateHash> m_closedSet;
		std::unordered_set<GridCoordinate, GridCoordinateHash> m_openSet;
	};
}
