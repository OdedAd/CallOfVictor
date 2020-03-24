#pragma once

//#include "Utils.h"

#ifndef PI
#define PI 3.14
#endif

class Point2D
{
public:
	Point2D(int r=0 , int c=0);
	int get_row() const;
	int get_col() const;
	void set_row(int r);
	void set_col(int c);
	bool operator == (const Point2D &other)
	{
		return row_ == other.row_ && col_ == other.col_;
	}

	bool operator != (const Point2D& other)
	{
		return !(*this == other);
	}

	double get_angle_between_two_points(const Point2D& other);
	
	double get_distance(Point2D& target) const;

private:
	int row_, col_;
};

