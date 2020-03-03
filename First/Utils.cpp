#include "Utils.h"

#include "Maze.h"

Point2D& Utils::FindMaximumInMatrix(Maze maze)
{
	Point2D* maxValueLocation = new Point2D();
	int rowIndex, colIndex;
	double curValue, maxValue;

	maxValueLocation->set_row(0);
	maxValueLocation->set_col(0);
	maxValue = maze.get_at_pos(0,0).get_value();

	for (rowIndex = 0; rowIndex < maze_size; rowIndex++)
	{
		for (colIndex = 0; colIndex < maze_size; colIndex++)
		{
			curValue = maze.get_at_pos(rowIndex,colIndex).get_value();
			if (maxValue < curValue)
			{
				maxValueLocation->set_row(rowIndex);
				maxValueLocation->set_col(colIndex);
				maxValue = maze.get_at_pos(rowIndex,colIndex).get_value();
			}
		}
	}

	return *maxValueLocation;
}

Point2D& Utils::FindMinimumInMatrix(Maze maze)
{
	Point2D* minValueLocation = new Point2D();
	int rowIndex, colIndex;
	double curValue , minValue;

	minValueLocation->set_row(0);
	minValueLocation->set_col(0);
	minValue = maze.get_at_pos(0,0).get_value();

	for (rowIndex = 0; rowIndex < maze_size; rowIndex++)
	{
		for (colIndex = 0; colIndex < maze_size; colIndex++)
		{
			curValue = maze.get_at_pos(rowIndex,colIndex).get_value();
			if (minValue > curValue)
			{
				minValueLocation->set_row(rowIndex);
				minValueLocation->set_col(colIndex);
				minValue = maze.get_at_pos(rowIndex,colIndex).get_value();
			}
		}
	}

	return *minValueLocation;
}