#include "Utils.h"

#include <iostream>
#include "GameMgr.h"
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
				max_value = maze.get_at_pos(row_index, col_index).get_value();
			}
		}
	}

	return *max_value_location;
}

void Utils::clear_temporary_maze(Node** matrix, const int size)
{
	for (auto i = 0; i < size; ++i)
	{
		delete[] matrix[i];
	}
	delete[] matrix;
}

bool Utils::compare_maps(double** team_one_map, double** team_two_map)
{
	for (int i = 0; i < maze_size; ++i)
	{
		for (int j = 0; j < maze_size; ++j)
		{
			if(team_one_map[i][j] != team_two_map[i][j])
				return true;
		}
	}
	return false;
}

void Utils::print_map(double** map)
{
	cout.precision(3);
	for (int i = 0; i < maze_size; ++i)
	{
		for (int j = 0; j < maze_size; ++j)
		{
			std::cout << map[i][j] << " ,";
		}
		std::cout << "\b" << std::endl;
	}
}

Point2D& Utils::find_minimum_in_matrix(Maze& maze, Team* team)
{
	Point2D* min_value_location = new Point2D();
	double** map = team->get_map();

	min_value_location->set_row(0);
	min_value_location->set_col(0);
	double min_value = 100;

	for (int row_index = 0; row_index < maze_size; row_index++)
	{
		for (int col_index = 0; col_index < maze_size; col_index++)
		{
			double cur_value = map[row_index][col_index];
			if (min_value > cur_value && (maze.get_at_pos(row_index, col_index).get_value() == SPACE ||
				maze.get_at_pos(row_index, col_index).get_value() == PLAYER)
				&& cur_value > 0.001f)
			{
				min_value_location->set_row(row_index);
				min_value_location->set_col(col_index);
				min_value = cur_value;
			}
		}
	}
	return *min_value_location;
}

Point2D& Utils::find_minimum_in_room(Maze& maze, Room& room, Team* team)
{
	Point2D* min_value_location = new Point2D();
	double** map = team->get_map();
	
	min_value_location->set_row(room.get_left_top().get_row());
	min_value_location->set_col(room.get_left_top().get_col());
	double min_value = 100;

	for (int row_index = room.get_left_top().get_row(); row_index < room.get_right_bottom().get_row(); row_index++)
	{
		for (int col_index = room.get_left_top().get_col(); col_index < room.get_right_bottom().get_col(); col_index++)
		{
			double cur_value = map[row_index][col_index];
			if (min_value > cur_value && (maze.get_at_pos(row_index, col_index).get_value() == SPACE ||
				maze.get_at_pos(row_index, col_index).get_value() == PLAYER)
				&& cur_value > 0.0001f)
			{
				min_value_location->set_row(row_index);
				min_value_location->set_col(col_index);
				min_value = cur_value;
			}
		}
	}
	
	return *min_value_location;
}
