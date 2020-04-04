#include "Bullet.h"

#include "GameMgr.h"
#include "GLUT.H"

Bullet::Bullet(const double x, const double y, const int stopping_power)
{
	this->x_ = x;
	this->y_ = y;
	//dirx_ = (rand() % 101)-50;
	//diry_ = (rand() % 101)-50;
	//const auto len = sqrt(dirx_ * dirx_ + diry_ * diry_);
	//dirx_ /= len;
	//diry_ /= len;
	is_moving_ = false;

	m_stopping_power_ = stopping_power;
}

Bullet::Bullet(int i, int j, const Point2D& target_location, int stopping_power)
{
	this->x_ = (j * 2.0) / (double)maze_size - 1;
	this->y_ = (i * 2.0) / (double)maze_size - 1;

	m_stopping_power_ = stopping_power;

	set_dir(Point2D(i, j).get_angle_between_two_points(target_location));
	is_moving_ = true;
}


void Bullet::show_me() const
{
	glColor3d(0, 0, 0);
	glBegin(GL_POLYGON);
	glVertex2d(x_ - 0.01, y_);
	glVertex2d(x_, y_ + 0.01);
	glVertex2d(x_ + 0.01, y_);
	glVertex2d(x_, y_ - 0.01);
	glEnd();
}

void Bullet::set_is_moving(const bool move)
{
	is_moving_ = move;
}

bool Bullet::get_is_moving() const
{
	return is_moving_;
}

void Bullet::move(Maze& maze)
{
	int i;
	int j;

	int cur_value;
	double start_x = x_;
	double start_y = y_;

	while (is_moving_)
	{
		i = maze_size * (y_ + 1) / 2;
		j = maze_size * (x_ + 1) / 2;
		cur_value = maze.get_at_pos(i, j).get_value();
		if (cur_value == SPACE)
		{
			x_ += 0.001 * dirx_;
			y_ += 0.001 * diry_;
		}
		else if (cur_value == PLAYER)
		{
			const auto distance = sqrt(pow(x_ - start_x, 2)
				+ pow(y_ - start_y, 2));
			int damage = m_stopping_power_ - distance;
			if (damage < 0) damage = 0;
			GameMgr::get_instance().hit_player(Point2D(i, j), damage);

			is_moving_ = false;
		}
		else
			is_moving_ = false;
	}
}

double Bullet::get_x() const
{
	return x_;
}

double Bullet::get_y() const
{
	return y_;
}

void Bullet::set_dir(double angle)
{
	dirx_ = cos(angle);
	diry_ = sin(angle);
}

void Bullet::simulate_motion(double map[maze_size][maze_size], Maze& maze)
{
	int i = maze_size * (y_ + 1) / 2;
	int j = maze_size * (x_ + 1) / 2;

	try
	{
		while (maze.get_at_pos(i, j).get_value() != WALL)
		{
			map[i][j] += delta;
			x_ += 0.001 * dirx_;
			y_ += 0.001 * diry_;
			i = maze_size * (y_ + 1) / 2;
			j = maze_size * (x_ + 1) / 2;
		}
	}catch (...){}//if i and j get fucked up do nothing with this

}
