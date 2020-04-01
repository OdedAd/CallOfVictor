#pragma once
#include "LogicBullet.h"
#include "Utils.h"
#include "Point2D.h"

class LogicGrenade
{

public:
	LogicGrenade(double x, double y);
	LogicGrenade(Point2D location, int megatons, int fuze = 3);
	~LogicGrenade() = default;
	void explode(Maze& maze);
	void show_me() const;
	void simulate_explosion(double map[maze_size][maze_size], Maze & maze);
private:
	double x_, y_;
	Point2D m_location;
	int m_megatons;
	int m_fuze;
};

