#pragma once
#include "Node.h"

class Room
{
public:
	Room() = default;
	~Room() = default;
	double get_height() const;
	double get_width() const;
	Room(int ci, int cj, int width, int height,short id);
	Point2D get_left_top()const;
	Point2D get_right_bottom()const;
	bool check_overlapping(Room * pother) const;
	void to_string() const;
	Point2D get_center()const;
	Point2D* get_random_point_in_room() const;
	Point2D* get_point_in_room() const;
	Node** get_room_maze() const;
	short get_id() const;

	bool operator == (const Room& other)
	{
		return center_ == other.get_center();
	}
	
private:
	Point2D left_top_, right_bottom_, center_;
	int width_, height_;
	short id_;
};

