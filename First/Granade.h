#pragma once
#include "Bullet.h"
#include "Node.h"

const int NUM_BULLETS = 16;

class Granade
{
public:
	Granade();
	Granade(double x, double y);
	~Granade();
	void explode();
	void showMe();
	void moveBullets(Maze maze);
	void SimulateExplosion(double map[MSZ][MSZ], Maze maze);
private:
	double x, y;
	Bullet* bullets[NUM_BULLETS];

};

