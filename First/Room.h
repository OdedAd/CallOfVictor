#pragma once
#include "Point2D.h"

class Room
{
public:
	Room();
	~Room();
	Room(int ci, int cj, int w, int h);
	Point2D get_left_top()const;
	Point2D get_right_bottom()const;
	bool check_overlapping(Room * pother) const;
	void to_string() const;
	Point2D get_center()const;
	Point2D* get_random_point_in_room() const;
	void set_map(double** new_map);
	double** get_map() const;
	
private:
	Point2D left_top_, right_bottom_, center_;
	int width_, height_;
	double** map_;
};

