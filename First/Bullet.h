#pragma once
#include "Maze.h"
#include "Node.h"
const int MSZ = 100;
const double delta = 0.0001;

class Bullet
{
public:
	Bullet();
	Bullet(double x, double y);
	~Bullet();
	void showMe();
	void SetIsMoving(bool move);
	bool GetIsMoving();
	void move(Maze maze);
	double getX();
	double getY();
	void SetDir(double angle);
	void SimulateMotion(double map[MSZ][MSZ], Maze maze);
	
private:
	double x, y;
	double dirx, diry;
	bool isMoving;
};

