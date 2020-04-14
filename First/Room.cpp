#include "Room.h"
#include <iostream>
#include "GameMgr.h"

using namespace std;

Room::Room(const int ci, const int cj, const int width, const int height, const short id) {
	center_.set_col(cj);
	center_.set_row(ci);
	left_top_.set_col(cj - width / 2);
	left_top_.set_row(ci - height / 2);
	right_bottom_.set_col(cj + width / 2);
	right_bottom_.set_row(ci + height / 2);
	width_ = width;
	height_ = height;
	id_ = id;
}

double Room::get_height() const
{
	return this->height_;
}

double Room::get_width() const
{
	return this->width_;
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
	const auto row = rand() % (height_ - 1);
	const auto col = rand() % (width_ - 1);
	Point2D* newPoint = new Point2D(row + left_top_.get_row(), col + left_top_.get_col());
	return newPoint;
}

Point2D* Room::get_point_in_room() const
{
	int col, row;
	do
	{
		col = left_top_.get_col()+(rand() % (width_-1));
		row = left_top_.get_row()+(rand() % (col-1));
	} while (GameMgr::get_instance().get_maze().get_at_pos(row, col).get_value() != SPACE);
	return new Point2D(row,col);
}

Node** Room::get_room_maze() const
{
	return GameMgr::get_instance().get_maze().get_sub_matrix(left_top_,right_bottom_);
}

short Room::get_id() const
{
	return this->id_;
}

