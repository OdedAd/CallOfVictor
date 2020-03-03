#pragma once

#include "Point2D.h"
#include "Node.h"

//static const int MSZ = 100;
static const int MSZ_UTILS = 100;

class Utils
{

public:
	Point2D& FindMinimumInMatrix(Node maze[MSZ_UTILS][MSZ_UTILS]);
	Point2D& FindMaximumInMatrix(Node maze[MSZ_UTILS][MSZ_UTILS]);


private:


};

