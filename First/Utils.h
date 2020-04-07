#pragma once

#include "Point2D.h"
#include "Node.h"
#include "Room.h"

class Maze;
static const int maze_size = 100;
static const int num_of_rooms = 20;
static const int num_of_pickups = 5;

class Utils
{
public:
	static void print_map(double map[maze_size][maze_size]);
	static Point2D& find_minimum_in_matrix(Maze& maze);
	static Point2D& find_minimum_in_room(Maze& maze,Room& room);
	static Point2D& find_maximum_in_matrix(Maze& maze);
	static void clear_temporary_maze(Node** matrix, const int size);
	static void clear_temporary_map(double** map, const int size);


private:


};

