#pragma once
#include "Point2D.h"

const int SPACE = 0;
const int WALL = 1;
const int START = 2;
const int TARGET = 3;
const int PATH = 4; // belongs to the path to target
const int GRAY = 5; // Fringe
const int BLACK = 6; // VISITED

const int PLAYER = 7; // player
const int PICKUP_MED = 8; // player
const int PICKUP_AMMO = 9;




class Node
{
public:
	Node();
	~Node() = default;

	Node(Point2D & pt, Point2D * t, int v, double g, Node * pr);

private:
	int value_;
	double h_, g_;
	Node* parent_;
	Point2D* target_;
	Point2D point_;


public:
	void set_value(int value);
	void set_point(Point2D point);
	void set_target(Point2D* target);
	int get_value() const;
	double get_g() const;
	double compute_h() const;
	double get_f() const;
	Point2D& get_point();
	Node* get_parent() const;
	Point2D* get_target() const;
	bool operator == (const Node &other) {
		return point_ == other.point_;
	}
};

