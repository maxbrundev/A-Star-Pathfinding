#include "Systems/AStar.h"

#include "Systems/Cell.h"

Systems::AStar::AStar(const GridSettings& p_gridSettings) :
m_settings(p_gridSettings)
{
	m_neighborsBuffer.reserve(4);
}

void Systems::AStar::Initialize(const GridCoordinate& p_start, const GridCoordinate& p_goal)
{
	m_startCoordinate = p_start;
	m_goalCoordinate  = p_goal;

	m_frontier.push({ p_start, 0.0f });
	m_cameFrom[p_start] = p_start;
	m_costSoFar[p_start] = 0.0f;
	m_openSet.insert(p_start);
}

void Systems::AStar::Clear()
{
	m_frontier = std::priority_queue<QueueItem, std::vector<QueueItem>, std::greater<>>();
	m_cameFrom.clear();
	m_costSoFar.clear();
	m_checkedTiles.clear();
	m_path.clear();
	m_closedSet.clear();
	m_openSet.clear();
}

void Systems::AStar::ComputeNeighbors(const GridCoordinate& p_gridCoordinate, const std::unordered_set<GridCoordinate, GridCoordinateHash>& p_obstacles)
{
	m_neighborsBuffer.clear();

	for (int i = 0; i < 4; i++)
	{
		int newX = p_gridCoordinate.X + NeighborsDirection[i][0];
		int newY = p_gridCoordinate.Y + NeighborsDirection[i][1];

		if (newX >= 0 && newX < m_settings.Width && newY >= 0 && newY < m_settings.Height)
		{
			GridCoordinate neighbor = { newX, newY };

			if (p_obstacles.find(neighbor) == p_obstacles.end())
			{
				m_neighborsBuffer.emplace_back(neighbor);
			}
		}
	}
}

float Systems::AStar::Heuristic(const GridCoordinate& p_a, const GridCoordinate& p_b) const
{
	// Manhattan Distance
	return static_cast<float>(std::abs(p_a.X - p_b.X) + std::abs(p_a.Y - p_b.Y));
}

std::vector<GridCoordinate> Systems::AStar::FindPath(const GridCoordinate& p_start, const GridCoordinate& p_goal, const std::unordered_set<GridCoordinate, GridCoordinateHash>& p_obstacles)
{
	m_frontier = std::priority_queue<QueueItem, std::vector<QueueItem>, std::greater<>>();
	m_cameFrom.clear();
	m_costSoFar.clear();
	m_checkedTiles.clear();
	m_closedSet.clear();
	m_openSet.clear();

	m_frontier.push({p_start, 0.0f });
	m_cameFrom[p_start] = p_start;
	m_costSoFar[p_start] = 0.0f;
	m_openSet.insert(p_start);

	while (!m_frontier.empty())
	{
		GridCoordinate current = m_frontier.top().Coordinate;
		m_frontier.pop();

		m_openSet.erase(current);
		m_closedSet.insert(current);
		m_checkedTiles.push_back(current);

		if (current == p_goal)
			break;

		ComputeNeighbors(current, p_obstacles);

		for (const auto& neighbor : m_neighborsBuffer)
		{
			if (m_closedSet.find(neighbor) != m_closedSet.end())
				continue;

			float newCost = m_costSoFar[current] + 1.0f;

			if (m_costSoFar.find(neighbor) == m_costSoFar.end() || newCost < m_costSoFar[neighbor])
			{
				m_costSoFar[neighbor] = newCost;
				float priority = newCost + Heuristic(p_goal, neighbor) * 1.4f;
				m_frontier.push({neighbor, priority});
				m_cameFrom[neighbor] = current;
				m_openSet.insert(neighbor);
			}
		}
	}

	std::vector<GridCoordinate> path;
	if (m_cameFrom.find(p_goal) != m_cameFrom.end())
	{
		GridCoordinate current = p_goal;

		while (current != p_start) 
		{
			path.push_back(current);

			current = m_cameFrom[current];

			if (m_cameFrom.find(current) == m_cameFrom.end()) 
			{
				path.clear();
				return path;
			}
		}

		path.push_back(p_start);
		std::reverse(path.begin(), path.end());
	}

	return path;
}

bool Systems::AStar::Step(const std::unordered_set<GridCoordinate, GridCoordinateHash>& p_obstacles)
{
	if (m_frontier.empty())
		return true;

	QueueItem currentItem = m_frontier.top();
	m_frontier.pop();
	GridCoordinate current = currentItem.Coordinate;

	m_openSet.erase(current);
	m_closedSet.insert(current);
	m_checkedTiles.push_back(current);

	if (current == m_goalCoordinate)
	{
		GridCoordinate currentPath = m_goalCoordinate;
		while (currentPath != m_cameFrom[currentPath])
		{
			m_path.push_back(currentPath);
			currentPath = m_cameFrom[currentPath];

			if (m_cameFrom.find(currentPath) == m_cameFrom.end()) 
			{
				m_path.clear();
				return true;
			}
		}
		m_path.push_back(m_startCoordinate);
		std::reverse(m_path.begin(), m_path.end());
		return true;
	}

	ComputeNeighbors(current, p_obstacles);

	for (const auto& neighbor : m_neighborsBuffer)
	{
		if (m_closedSet.find(neighbor) != m_closedSet.end())
			continue;

		float newCost = m_costSoFar[current] + 1.0f;

		if (m_costSoFar.find(neighbor) == m_costSoFar.end() || newCost < m_costSoFar[neighbor])
		{
			m_costSoFar[neighbor] = newCost;
			float priority = newCost + Heuristic(m_goalCoordinate, neighbor) * 1.4f;
			m_frontier.push({neighbor, priority});
			m_cameFrom[neighbor] = current;
			m_openSet.insert(neighbor);
		}
	}

	return false;
}
