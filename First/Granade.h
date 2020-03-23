#pragma once
#include "Bullet.h"
#include "Node.h"
#include "Utils.h"

//const double PI = 3.14;

#ifndef PI
#define PI 3.14
#endif

const int NUM_BULLETS = 16;
class Bullet;

class Granade
{
public:
	Granade();
	Granade(double x, double y);
	~Granade();
	void explode();
	void show_me();
	void move_bullets(Maze maze);
	void simulate_explosion(double map[maze_size][maze_size], Maze& maze);
private:
	double x_, y_;
	Bullet* bullets_[NUM_BULLETS];

};

