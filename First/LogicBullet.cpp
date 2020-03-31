#include "LogicBullet.h"
#include "GLUT.H"

#include "GameMgr.h"



LogicBullet::LogicBullet(Point2D location, const Point2D& target_location, int stopping_power)
{
	this->x_ = location.get_row();
	this->y_ = location.get_col();
	location_ = location;
	target_location_ = target_location;
	m_stopping_power = stopping_power;
	is_moving_ = true;

	set_dir(location.get_angle_between_two_points(target_location));
}

LogicBullet::LogicBullet(double x, double y, int stopping_power)
{
}

void LogicBullet::show_me() const
{
	double y = location_.get_row();
	double x = location_.get_col();

	glColor3d(0, 1, 0);

	double sz = 2.0 / 100;
	double xx = (x * sz - 1);
	double yy = y * sz - 1;

	glBegin(GL_POLYGON);
	glVertex2d(xx - 0.01, yy);
	glVertex2d(xx, yy + 0.01);
	glVertex2d(xx + 0.01, yy);
	glVertex2d(xx, yy - 0.01);

	glEnd();
}

void LogicBullet::set_is_moving(bool move)
{
	is_moving_ = move;
}

bool LogicBullet::get_is_moving() const
{
	return is_moving_;
}

void LogicBullet::move(Maze& maze)
{
	x_ = location_.get_row();
	y_ = location_.get_col();
	int cur_value;
	int step_counter = 0;

	while (is_moving_)
	{
		cur_value = maze.get_at_pos(x_, y_).get_value();

		if (cur_value == SPACE)
		{
			x_ += dirx_;
			y_ += diry_;
			++step_counter;
		}
		if (cur_value == PLAYER)
		{
			location_.set_row(x_);
			location_.set_col(y_);
			int damage = m_stopping_power - step_counter;
			if (damage < 0) damage = 0;
			GameMgr::get_instance().hit_player(location_, damage);
			is_moving_ = false;
		}
		else
		{
			location_.set_row(x_);
			location_.set_col(y_);
			is_moving_ = false;
		}
	}
}

double LogicBullet::get_x() const
{
	return location_.get_row();
}

double LogicBullet::get_y() const
{
	return location_.get_col();
}

void LogicBullet::set_dir(double angle)
{
	dirx_ = (int)round(cos(angle));
	diry_ = (int)round(sin(angle));
}

void LogicBullet::simulate_motion(double map[maze_size][maze_size], Maze& maze)
{
}
