#pragma once

#include "Point2D.h"
#include "Node.h"

class Maze;
static const int maze_size = 100;
static const int num_of_rooms = 20;
static const int num_of_pickups = 5;

class Utils
{

public:
	static Point2D& find_minimum_in_matrix(Maze& maze);
	static Point2D& find_maximum_in_matrix(Maze& maze);
	static void clear_temporary_maze(Node** matrix, const int size);


private:


};

