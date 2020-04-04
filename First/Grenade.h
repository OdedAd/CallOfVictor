#pragma once
#include "Bullet.h"
#include "Utils.h"


#ifndef PI
#define PI 3.14
#endif

const int NUM_BULLETS = 16;
class Bullet;

class Grenade
{
public:
	Grenade() = default;
	Grenade(double x, double y);
	~Grenade();
	void explode();
	void show_me() const;
	void move_bullets(Maze maze);
	void simulate_explosion(double map[maze_size][maze_size], Maze& maze);
private:
	double x_, y_;
	Bullet* bullets_[NUM_BULLETS];

};

