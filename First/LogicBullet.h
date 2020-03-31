#pragma once
#include "Maze.h"
#include "Node.h"
#include "Point2D.h"



class LogicBullet
{
public:
	LogicBullet(Point2D location, const Point2D& target_location, int stopping_power);
	~LogicBullet() = default;

	LogicBullet(double x, double y, int stopping_power);
	void show_me() const;
	void set_is_moving(bool move);
	bool get_is_moving() const;
	void move(Maze& maze);
	double get_x() const;
	double get_y() const;
	void set_dir(double angle);
	void simulate_motion(double map[maze_size][maze_size], Maze& maze);

private:
	double x_, y_;
	double dirx_, diry_;
	bool is_moving_;
	int m_stopping_power;
	Point2D location_;
	Point2D target_location_;
};
