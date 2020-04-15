#include "Grenade.h"

#include <iostream>

Grenade::Grenade(const double x, const double y, const int megatons, const int fuze, const double color[COLOR_LENGTH])
{
	int i;
	this->x_ = x;
	this->y_ = y;
	double alpha, delta = 2 * PI / NUM_BULLETS;
	m_megatons = megatons;

	for (int i = 0; i < COLOR_LENGTH; i++)
		m_color_[i] = color[i];

	for (i = 0, alpha = 0; i < NUM_BULLETS; i++, alpha += delta)
	{
		bullets_[i] = new Bullet(x, y, m_megatons, m_color_);
		bullets_[i]->set_dir(alpha);
	}

	m_fuze = fuze;
	isExploded = false;

}

Grenade::Grenade(const int i, const int j, const int megatons, const int fuze, const double color[COLOR_LENGTH])
{
	int index;
	this->x_ = (j * 2.0) / (double)maze_size - 1;
	this->y_ = (i * 2.0) / (double)maze_size - 1;

	m_megatons = megatons;
	m_fuze = fuze;
	isExploded = false;

	for (int i = 0; i < COLOR_LENGTH; i++)
		m_color_[i] = color[i];

	double alpha, delta = 2 * PI / NUM_BULLETS;
	for (index = 0, alpha = 0; index < NUM_BULLETS; index++, alpha += delta)
	{
		bullets_[index] = new Bullet(x_, y_, m_megatons, m_color_);
		bullets_[index]->set_dir(alpha);
	}

}

Grenade::~Grenade()
{
	for (auto bullet : bullets_)
	{
		delete bullet;
	}
}


void Grenade::explode(Maze& maze)
{
	static int tic_counter = 0;

	if (tic_counter < m_fuze)
		++tic_counter;
	else
	{
		for (int i = 0; i < NUM_BULLETS; i++)
			bullets_[i]->set_is_moving(true);

		move_bullets(maze);

		tic_counter = 0;
		isExploded = true;
	}
}

void Grenade::show_me() const
{
	for (int i = 0; i < NUM_BULLETS; i++)
		bullets_[i]->show_me();
}

void Grenade::move_bullets(Maze& maze)
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

void Grenade::simulate_explosion(double** map, Maze& maze)
{
	for (int i = 0; i < NUM_BULLETS; i++)
	{
		bullets_[i]->set_is_moving(true);
		bullets_[i]->simulate_motion(map, maze);
	}
}

bool Grenade::get_is_exploded() const
{
	return isExploded;
}

double Grenade::get_row() const
{
	return this->y_;
}

double Grenade::get_col() const
{
	return this->x_;
}
