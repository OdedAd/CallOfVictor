#pragma once

#include "Point2D.h"
#include "Node.h"

class Maze;
static const int maze_size = 100;
class Utils
{

public:
	Point2D& FindMinimumInMatrix(Maze maze);
	Point2D& FindMaximumInMatrix(Maze maze);


private:


};

