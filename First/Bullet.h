#pragma once
#include "Utils.h"
class Maze;
class Point2D;
//#include "Maze.h"
const double delta = 0.0001;


class Bullet
{
public:
	Bullet() = default;
	~Bullet() = default;

	Bullet(double x, double y, int stopping_power = 10, const double color[COLOR_LENGTH] = DEFAULT_BULLET_COLOR); //openGL cord
	Bullet(int i, int j, const Point2D& target_location, int stopping_power = 10, const double color[COLOR_LENGTH] = DEFAULT_BULLET_COLOR);// maze cord
	void show_me() const;
	void set_is_moving(bool move);
	bool get_is_moving() const;
	void move( Maze& maze);
	double get_x() const;
	double get_y() const;
	void set_dir(double angle);
	void simulate_motion(double map[maze_size][maze_size], Maze& maze);
	void simulate_motion(double** map, Maze& maze);

private:
	double x_, y_;
	double dirx_, diry_;
	bool is_moving_;
	int m_stopping_power_;
	//color in RGB
	double m_color_[COLOR_LENGTH];
};

