#include "Bullet.h"
#include "GLUT.H"
#include <math.h>

Bullet::Bullet()
{
}

Bullet::Bullet(double x, double y)
{
	this->x_ = x;
	this->y_ = y;
	dirx_ = (rand() % 101)-50;
	diry_ = (rand() % 101)-50;
	const auto len = sqrt(dirx_ * dirx_ + diry_ * diry_);
	dirx_ /= len;
	diry_ /= len;
	is_moving_ = false;
}


Bullet::~Bullet()
{
}

void Bullet::show_me() const
{
	glColor3d(0, 0, 0);
	glBegin(GL_POLYGON);
	glVertex2d(x_ - 0.01, y_);
	glVertex2d(x_ , y_+ 0.01);
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

void Bullet::move(Maze maze)
{
	int i, j;
	i = maze_size * (y_ + 1) / 2;
	j = maze_size * (x_ + 1) / 2;

	if (is_moving_)
	{
		if (maze.get_at_pos(i,j).get_value() == SPACE)
		{
			x_ += 0.001 * dirx_;
			y_ += 0.001 * diry_;
		}
		if (maze.get_at_pos(i,j).get_value() == PLAYER)
		{
			//TODO: 
			//add somthing to hurt the player. signel damage at a node or somthing
			//(will send the node coordinates to someone else and he will handle the damage part).

			is_moving_ = false;
		}
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

void Bullet::simulate_motion(double map[maze_size][maze_size], Maze maze)
{

	int i, j;
	i = maze_size * (y_ + 1) / 2;
	j = maze_size * (x_ + 1) / 2;

	while(maze.get_at_pos(i,j).get_value() == SPACE)
	{
		map[i][j] += delta;
		x_ += 0.001*dirx_;
		y_ += 0.001*diry_;
		i = maze_size * (y_ + 1) / 2;
		j = maze_size * (x_ + 1) / 2;
	}

}
