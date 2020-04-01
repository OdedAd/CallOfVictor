#include "LogicGrenade.h"
#include "GLUT.H"

#include "GameMgr.h"


LogicGrenade::LogicGrenade(double x, double y)
{

}

LogicGrenade::LogicGrenade(Point2D location, int megatons, int fuze)
{
	this->x_ = location.get_row();
	this->y_ = location.get_col();
	m_location = location;
	m_megatons = megatons;
}

void LogicGrenade::explode(Maze& maze)
{
	if (m_fuze > 0)
		--m_fuze;
	else
	{
		int radius = 5;
		x_ = m_location.get_row();
		y_ = m_location.get_col();

		int cur_value = maze.get_at_pos(x_, y_).get_value();
		Point2D target;

		if (cur_value == SPACE)
		{
			/*
			target.set_col(y_ - 1);
			target.set_row(x_ - 1);
			GameMgr::get_instance().get_bullets().push_back(new LogicBullet(m_location, target, m_megatons));

			target.set_col(y_);
			target.set_row(x_ - 1);
			GameMgr::get_instance().get_bullets().push_back(new LogicBullet(m_location, target, m_megatons));

			target.set_col(y_ + 1);
			target.set_row(x_ - 1);
			GameMgr::get_instance().get_bullets().push_back(new LogicBullet(m_location, target, m_megatons));

			target.set_col(y_ + 1);
			target.set_row(x_);
			GameMgr::get_instance().get_bullets().push_back(new LogicBullet(m_location, target, m_megatons));

			target.set_col(y_ + 1);
			target.set_row(x_ + 1);
			GameMgr::get_instance().get_bullets().push_back(new LogicBullet(m_location, target, m_megatons));

			target.set_col(y_);
			target.set_row(x_ + 1);
			GameMgr::get_instance().get_bullets().push_back(new LogicBullet(m_location, target, m_megatons));

			target.set_col(y_ - 1);
			target.set_row(x_ + 1);
			GameMgr::get_instance().get_bullets().push_back(new LogicBullet(m_location, target, m_megatons));

			target.set_col(y_ - 1);
			target.set_row(x_ );
			GameMgr::get_instance().get_bullets().push_back(new LogicBullet(m_location, target, m_megatons));
			*/
		}
		else if (cur_value == PLAYER)
		{
			GameMgr::get_instance().hit_player(m_location, m_megatons);
		}

	}
}

void LogicGrenade::show_me() const
{

	double y = m_location.get_row();
	double x = m_location.get_col();

	glColor3d(0, 1, 1);

	double sz = 2.0 / 100;
	double xx = (x * sz - 1);
	double yy = y * sz - 1;

	glBegin(GL_POLYGON);
	glVertex2d(xx - 0.01, yy - 0.01);
	glVertex2d(xx - 0.01, yy + 0.01);
	glVertex2d(xx + 0.01, yy - 0.01);
	glVertex2d(xx - 0.01, yy - 0.01);

	glEnd();

}

void LogicGrenade::simulate_explosion(double map[maze_size][maze_size], Maze& maze)
{

}
