#include "Granade.h"

const double PI = 3.14;



Granade::Granade()
{
}

Granade::Granade(double x, double y)
{
	int i;
	double alpha, delta = 2*PI/NUM_BULLETS;
	this->x_ = x;
	this->y_ = y;
	for (i = 0,alpha = 0; i < NUM_BULLETS; i++,alpha+=delta)
	{
		bullets_[i] = new Bullet(x, y);
		bullets_[i]->set_dir(alpha);
	}

}


Granade::~Granade()
{
}

void Granade::explode()
{
	for (int i = 0; i < NUM_BULLETS; i++)
		bullets_[i]->set_is_moving(true);
}

void Granade::show_me()
{
	for (int i = 0; i < NUM_BULLETS; i++)
		bullets_[i]->show_me();
}

void Granade::move_bullets(Maze maze)
{
	for (int i = 0; i < NUM_BULLETS; i++)
		bullets_[i]->move(maze);

}

void Granade::simulate_explosion(double map[maze_size][maze_size], Maze& maze)
{
	for (int i = 0; i < NUM_BULLETS; i++)
	{
		bullets_[i]->set_is_moving(true);
		bullets_[i]->simulate_motion(map, maze);
	}

}
