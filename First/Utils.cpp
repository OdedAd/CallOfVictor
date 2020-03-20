#include "Utils.h"

#include "Maze.h"

Point2D& Utils::find_maximum_in_matrix(Maze& maze)
{
	Point2D* max_value_location = new Point2D();

	max_value_location->set_row(0);
	max_value_location->set_col(0);
	double max_value = maze.get_at_pos(0, 0).get_value();

	for (auto row_index = 0; row_index < maze_size; row_index++)
	{
		for (auto col_index = 0; col_index < maze_size; col_index++)
		{
			double cur_value = maze.get_at_pos(row_index, col_index).get_value();
			if (max_value < cur_value)
			{
				max_value_location->set_row(row_index);
				max_value_location->set_col(col_index);
				max_value = maze.get_at_pos(row_index,col_index).get_value();
			}
		}
	}

	return *max_value_location;
}

void Utils::clear_temporary_maze(Node** matrix,const int size)
{
	for (auto i = 0; i < size; ++i)
	{
		delete[] matrix[i];
	}
	delete[] matrix;
}

Point2D& Utils::find_minimum_in_matrix(Maze& maze)
{
	Point2D* min_value_location = new Point2D();

	min_value_location->set_row(0);
	min_value_location->set_col(0);
	double min_value = maze.get_at_pos(0, 0).get_value();

	for (int row_index = 0; row_index < maze_size; row_index++)
	{
		for (int col_index = 0; col_index < maze_size; col_index++)
		{
			double cur_value = maze.get_at_pos(row_index, col_index).get_value();
			if (min_value > cur_value)
			{
				min_value_location->set_row(row_index);
				min_value_location->set_col(col_index);
				min_value = maze.get_at_pos(row_index,col_index).get_value();
			}
		}
	}

	return *min_value_location;
}
