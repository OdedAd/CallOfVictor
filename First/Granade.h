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
	void show_me();
	void move_bullets(Maze maze);
	void simulate_explosion(double map[maze_size][maze_size], Maze& maze);
private:
	double x_, y_;
	Bullet* bullets_[NUM_BULLETS];

};

