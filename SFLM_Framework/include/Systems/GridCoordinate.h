#pragma once

struct GridCoordinate
{
	int X;
	int Y;

	inline bool operator==(const GridCoordinate& p_other) const
	{
		return X == p_other.X && Y == p_other.Y;
	}

	inline bool operator!=(const GridCoordinate& p_other) const
	{
		return !(*this == p_other);
	}
};

struct GridCoordinateHash
{
	inline std::size_t operator()(const GridCoordinate& p_coordinate) const
	{
		return std::hash<int>()(p_coordinate.X ^ (p_coordinate.Y << 16));
	}
};
