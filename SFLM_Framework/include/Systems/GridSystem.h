#pragma once

#include "Context/Window.h"

#include "Systems/Cell.h"
#include "Systems/GridCoordinate.h"
#include "Systems/GridSettings.h"

namespace Systems
{
	class GridSystem final
	{
	public:
		GridSystem(Context::Window& p_window, GridSettings& p_gridSettings);
		~GridSystem();

		static sf::Vector2f GridToWorld(int p_x, int p_y);
		static GridCoordinate WorldToGrid(const sf::Vector2f& p_worldPosition);

		bool IsInsideGrid(const GridCoordinate& p_gridCoordinate) const;
		bool IsInsideGrid(int p_x, int p_y) const;

		Cell* GetCell(int p_x, int p_y) const;
		int GetWidth() const;
		int GetHeight() const;

	private:
		void Initialize();
		void SetNeighbors() const;

	public:
		inline static int CELL_SIZE;
		inline static sf::Vector2f GRID_OFFSET;
		
	private:
		GridSettings& m_settings;
		Context::Window& m_window;
		Cell** m_cells;
		int Width;
		int Height;
	};
}
