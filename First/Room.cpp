#include "Room.h"
#include <math.h>
#include <iostream>

using namespace std;

Room::Room()
{
}


Room::~Room()
{
}

Room::Room(int ci, int cj, int w, int h) {
	center_.set_col(cj);
	center_.set_row(ci);
	left_top_.set_col(cj - w / 2);
	left_top_.set_row(ci - h / 2);
	right_bottom_.set_col(cj + w / 2);
	right_bottom_.set_row(ci + h / 2);
	width_ = w;
	height_ = h;

}

Point2D Room::get_left_top() const
{
	return left_top_;
}
Point2D Room::get_right_bottom() const
{
	return right_bottom_;
}

bool Room::check_overlapping(Room* pother)
{
	int horiz_dist, vert_dist, vsz,hsz;
	horiz_dist = abs(center_.get_col() - pother->center_.get_col());
	vert_dist = abs(center_.get_row() - pother->center_.get_row());
	vsz = height_ / 2 + pother->height_ / 2;
	hsz = width_ / 2 + pother->width_ / 2;

	return horiz_dist <= hsz+2 && vert_dist <= vsz+2;
}


void Room::to_string() const
{
	cout << "check new Room " << "center: (" << center_.get_row() << "," << center_.get_col()
		<< "), width: " << width_ << ", height" << height_ << endl;
}

Point2D Room::get_center() const
{
	return center_;
}


