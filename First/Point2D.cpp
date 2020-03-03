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
