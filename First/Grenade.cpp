#include "Grenade.h"

Grenade::Grenade(const double x, const double y): x_(x),y_(y)
{
	int i;
	double alpha, delta = 2*PI/NUM_BULLETS;
	for (i = 0,alpha = 0; i < NUM_BULLETS; i++,alpha+=delta)
	{
		bullets_[i] = new Bullet(x, y);
		bullets_[i]->set_dir(alpha);
	}

}

void Grenade::explode()
{
	for (int i = 0; i < NUM_BULLETS; i++)
		bullets_[i]->set_is_moving(true);
}

void Grenade::show_me() const
{
	for (int i = 0; i < NUM_BULLETS; i++)
		bullets_[i]->show_me();
}

void Grenade::move_bullets(Maze maze)
{
	for (int i = 0; i < NUM_BULLETS; i++)
		bullets_[i]->move(maze);

}

void Grenade::simulate_explosion(double map[maze_size][maze_size], Maze& maze)
{
	for (int i = 0; i < NUM_BULLETS; i++)
	{
		bullets_[i]->set_is_moving(true);
		bullets_[i]->simulate_motion(map, maze);
	}

}
