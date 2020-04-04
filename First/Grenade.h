#pragma once
#include "Bullet.h"
#include "Utils.h"


#ifndef PI
#define PI 3.14
#endif

static const int NUM_BULLETS = 16;
static const int DEFAULT_MAGATONS = 10;
static const int DEFAULT_FUZE = 3;

class Bullet;

class Grenade
{
public:
	Grenade() = default;
	Grenade(double x, double y, int megatons = DEFAULT_MAGATONS, int fuze = DEFAULT_FUZE);  //openGL cord
	Grenade(int i, int j, int megatons = DEFAULT_MAGATONS, int fuze = DEFAULT_FUZE); // maze cord
	~Grenade();
	void explode(Maze& maze);
	void show_me() const;
	void move_bullets( Maze& maze);
	void simulate_explosion(double map[maze_size][maze_size], Maze& maze);
	bool get_is_exploded();
private:
	double x_, y_;
	Bullet* bullets_[NUM_BULLETS];

	int m_megatons;
	int m_fuze;
	bool isExploded;
};

