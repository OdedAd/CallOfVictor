#include "Point2D.h"



Point2D::Point2D(const int r, const int c) : row_(r), col_(c)
{
}

int Point2D::get_row() const
{
	return row_;
}
int Point2D::get_col() const
{
	return col_;
}
void Point2D::set_row(const int r) {
	row_ = r;
}
void Point2D::set_col(const int c)
{
	col_ = c;
}

double Point2D::get_angle_between_two_points(const Point2D& other)
{
	
	if (row_ > other.get_row())
	{
		return 0;
	}
	if (col_ < other.get_col())
	{
		return PI/2;
	}
	if (row_ < other.get_row())
	{
		return PI;
	}
	if (col_ > other.get_col())
	{
		return PI + PI / 2;
	}
	if (*this == other) // if they are the same point there is no angle between them.
		return -1;
	return 0;
}
