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
	//Grenade() = default;
	Grenade(double x, double y, int megatons = DEFAULT_MAGATONS, int fuze = DEFAULT_FUZE, const double color[COLOR_LENGTH] = DEFAULT_GRENADE_COLOR);  //openGL cord
	Grenade(int i, int j, int megatons = DEFAULT_MAGATONS, int fuze = DEFAULT_FUZE, const double color[COLOR_LENGTH] = DEFAULT_GRENADE_COLOR); // maze cord
	~Grenade();
	void explode(Maze& maze);
	void show_me() const;
	void move_bullets( Maze& maze);
	void simulate_explosion(double map[maze_size][maze_size], Maze& maze);
	void simulate_explosion(double** map, Maze& maze);
	bool get_is_exploded() const;
	double get_row() const;
	double get_col() const ;
private:
	double x_, y_;
	Bullet* bullets_[NUM_BULLETS];

	int m_megatons;
	int m_fuze;
	bool isExploded;
	//color in RGB
	double m_color_[COLOR_LENGTH];
};

