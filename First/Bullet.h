#pragma once
#include "Maze.h"
#include "Node.h"
const double delta = 0.0001;

class Bullet
{
public:
	Bullet();
	Bullet(double x, double y);
	~Bullet();
	void show_me() const;
	void set_is_moving(bool move);
	bool get_is_moving() const;
	void move(Maze maze);
	double get_x() const;
	double get_y() const;
	void set_dir(double angle);
	void simulate_motion(double map[maze_size][maze_size], Maze maze);
	
private:
	double x_, y_;
	double dirx_, diry_;
	bool is_moving_;
};

