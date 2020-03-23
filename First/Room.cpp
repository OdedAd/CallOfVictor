#include "Room.h"
#include <math.h>
#include <iostream>

#include "Granade.h"

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

bool Room::check_overlapping(Room* pother) const
{
	const auto horiz_dist = abs(center_.get_col() - pother->center_.get_col());
	const auto vert_dist = abs(center_.get_row() - pother->center_.get_row());
	const auto vsz = height_ / 2 + pother->height_ / 2;
	const auto hsz = width_ / 2 + pother->width_ / 2;

	return horiz_dist <= hsz + 2 && vert_dist <= vsz + 2;
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

Point2D* Room::get_random_point_in_room() const
{
	const auto row = rand() % (height_-1);
	const auto col = rand() % (width_-1);
	return new Point2D(row + left_top_.get_row(),col + left_top_.get_col());
}

void Room::set_map(double** new_map)
{
	this->map_ = new_map;
}

double** Room::get_map() const
{
	return this->map_;
}

