#include "Systems/GridSystem.h"

Systems::GridSystem::GridSystem(Context::Window& p_window, GridSettings& p_gridSettings) :
m_settings(p_gridSettings),
m_window(p_window),
Width(p_gridSettings.Width),
Height(p_gridSettings.Height)
{
	Initialize();
	SetNeighbors();
}

Systems::GridSystem::~GridSystem()
{
	for (int i = 0; i < Width; ++i)
	{
		delete[] m_cells[i];
	}
	delete[] m_cells;
}

void Systems::GridSystem::Initialize()
{
	const sf::Vector2u windowSize = m_window.GetWindow()->getSize();

	const float maxTileSize = std::min(
		static_cast<float>(windowSize.x) / static_cast<float>(Width),
		static_cast<float>(windowSize.y) / static_cast<float>(Height)
	);

	CELL_SIZE = maxTileSize;

	constexpr float outlineThickness = 2.0f;
	const float tileSize = CELL_SIZE - outlineThickness;

	GRID_OFFSET = sf::Vector2f(
		static_cast<float>(windowSize.x - Width * CELL_SIZE) / 2.0f,
		static_cast<float>(windowSize.y - Height * CELL_SIZE) / 2.0f
	);

	m_cells = new Cell * [Width];
	for (int i = 0; i < Width; ++i)
	{
		m_cells[i] = new Cell[Height];

		for (int j = 0; j < Height; ++j)
		{
			m_cells[i][j].GridCoordinate.X = i;
			m_cells[i][j].GridCoordinate.Y = j;

			m_cells[i][j].Tile.setSize(sf::Vector2f(tileSize, tileSize));

			m_cells[i][j].Tile.setOrigin(tileSize / 2.0f, tileSize / 2.0f);

			m_cells[i][j].Tile.setOutlineThickness(outlineThickness);
			m_cells[i][j].Tile.setOutlineColor(sf::Color::White);

			m_cells[i][j].Tile.setFillColor(sf::Color(200, 200, 180));

			m_cells[i][j].Tile.setPosition(
				GRID_OFFSET.x + i * CELL_SIZE + CELL_SIZE / 2.0f,
				GRID_OFFSET.y + j * CELL_SIZE + CELL_SIZE / 2.0f
			);
		}
	}
}

void Systems::GridSystem::SetNeighbors() const
{
	for (int i = 0; i < Width; ++i)
	{
		for (int j = 0; j < Height; ++j)
		{
			Cell& cell = m_cells[i][j];

			cell.Neighbors[LEFT_NEIGHBOR] = nullptr;
			cell.Neighbors[RIGHT_NEIGHBOR] = nullptr;
			cell.Neighbors[TOP_NEIGHBOR] = nullptr;
			cell.Neighbors[BOTTOM_NEIGHBOR] = nullptr;


			if (j > 0)
			{
				cell.Neighbors[LEFT_NEIGHBOR] = &m_cells[i][j - 1];
			}

			if (j < Height - 1)
			{
				cell.Neighbors[RIGHT_NEIGHBOR] = &m_cells[i][j + 1];
			}

			if (i > 0)
			{
				cell.Neighbors[TOP_NEIGHBOR] = &m_cells[i - 1][j];
			}

			if (i < Width - 1)
			{
				cell.Neighbors[BOTTOM_NEIGHBOR] = &m_cells[i + 1][j];
			}
		}
	}
}

bool Systems::GridSystem::IsInsideGrid(const GridCoordinate& p_gridCoordinate) const
{
	return p_gridCoordinate.X >= 0 && p_gridCoordinate.X < Width && p_gridCoordinate.Y >= 0 && p_gridCoordinate.Y < Height;
}

bool Systems::GridSystem::IsInsideGrid(int p_x, int p_y) const
{
	return p_x >= 0 && p_x < Width && p_y >= 0 && p_y < Height;
}

Cell* Systems::GridSystem::GetCell(int p_x, int p_y) const
{
	if (IsInsideGrid(p_x, p_y))
	{
		return &m_cells[p_x][p_y];
	}

	return nullptr;
}

int Systems::GridSystem::GetWidth() const
{
	return m_settings.Width;
}

int Systems::GridSystem::GetHeight() const
{
	return m_settings.Height;
}

sf::Vector2f Systems::GridSystem::GridToWorld(int p_x, int p_y)
{
	return {
		p_x * CELL_SIZE + GRID_OFFSET.x + CELL_SIZE / 2.0f,
		p_y * CELL_SIZE + GRID_OFFSET.y + CELL_SIZE / 2.0f
	};
}

GridCoordinate Systems::GridSystem::WorldToGrid(const sf::Vector2f& p_worldPosition)
{
	int gridX = static_cast<int>(std::floor((p_worldPosition.x - GRID_OFFSET.x) / CELL_SIZE));
	int gridY = static_cast<int>(std::floor((p_worldPosition.y - GRID_OFFSET.y) / CELL_SIZE));
	return GridCoordinate{gridX, gridY};
}
