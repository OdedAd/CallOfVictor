#include "Sniper.h"

#include "GameMgr.h"

Sniper::Sniper(GameMgr* mgr, int id, Team* team, Node* location, int max_ammo, int maxHP,
	int grenade_cost, int shooting_ammo_cost, int melee_ammo_cost,
	int grenade_dmg, int shooting_dmg, int melee_dmg) :
	Player(mgr, id, team, location, max_ammo, maxHP,
		grenade_cost, shooting_ammo_cost, melee_ammo_cost,
		grenade_dmg, shooting_dmg, melee_dmg)
{

	if (grenade_cost < 0)
		m_grenade_ammo_cost_ = m_max_ammo_;
	else
		m_grenade_ammo_cost_ = grenade_cost;

	if (shooting_ammo_cost < 0)
		m_shooting_ammo_cost_ = 2;
	else
		m_shooting_ammo_cost_ = shooting_ammo_cost;

	if (melee_ammo_cost < 0)
		m_melee_ammo_cost_ = m_max_ammo_;
	else
		m_melee_ammo_cost_ = melee_ammo_cost;


	if (grenade_dmg < 0)
		m_grenade_dmg_ = m_max_hp_ / 5;
	else
		m_grenade_dmg_ = grenade_dmg;

	if (shooting_dmg < 0)
		m_shooting_dmg_ = (int)(m_max_hp_ * (3.0/4.0));
	else
		m_shooting_dmg_ = shooting_dmg;

	if (melee_dmg < 0)
		m_melee_dmg_ = m_max_hp_ / 2;
	else
		m_melee_dmg_ = melee_dmg;

	m_throw_dis_min_ = 5;
	m_throw_dis_max_ = 8;
}

void Sniper::show_me() const
{
	double y = m_location_->get_point().get_row();
	double x = m_location_->get_point().get_col();

	double R = m_team_->get_color()[0] + 0.35;;
	double G = m_team_->get_color()[1] + 0.35;
	double B = m_team_->get_color()[2] + 0.35;

	glColor3d(R, G, B);

	// draw rectangle
	double sz = 2.0 / 100;
	double xx = (x * sz - 1);
	double yy = y * sz - 1;

	glBegin(GL_POLYGON);
	glVertex2d(xx, yy);
	glVertex2d(xx + sz, yy);
	glVertex2d(xx + sz, yy + sz);
	glVertex2d(xx, yy + sz);
	glEnd();

}

///<summary>
/// Find a target and attack if in range.
///</summary>
void Sniper::fight()
{
	bool is_shootable;
	Point2D target_location;
	target_location = m_mgr_->find_nearest_enemy(m_location_->get_point(), *m_team_, is_shootable);
	if (target_location.get_col() == -1 && target_location.get_row() == -1)
	{
		m_is_moving_ = false;
	}
	else
	{

		bool is_successful = false;
		double distance_from_target = m_location_->get_point().get_distance(target_location);

		if (distance_from_target > m_stab_dis_max_ && distance_from_target < 4.0)
		{
			run_away();
		}
		else if (m_ammo_ > 0)
		{
			if (distance_from_target <= m_stab_dis_max_ + 1.0)
			{
				is_successful = m_mgr_->stab(this, target_location);

				if (is_successful)
				{
					m_ammo_ -= m_melee_ammo_cost_;
					m_is_moving_ = false;
					std::cout << "Player " << m_id_ << " Stabbed someone " << std::endl;
				}

			}
			if (m_ammo_ >= m_grenade_ammo_cost_ && distance_from_target > m_throw_dis_min_ - 1.0 && distance_from_target < m_throw_dis_max_ + 1.0)
			{
				is_successful = m_mgr_->throw_grenade(this, target_location);

				if (is_successful)
				{
					m_ammo_ -= m_grenade_ammo_cost_;
					m_is_moving_ = false;
					std::cout << "Player " << m_id_ << " Throw a grenade " << std::endl;
				}

			}
			else if(m_ammo_ >= m_shooting_ammo_cost_)
			{
				is_successful = m_mgr_->shoot(this, target_location);

				if (is_successful)
				{
					m_ammo_ -= m_shooting_ammo_cost_;
					m_is_moving_ = false;
					std::cout << "Player " << m_id_ << " Is shooting " << std::endl;
				}

			}
			else //not enough ammo
				reload();

			if (is_successful == false)
			{
				m_cur_target_node_ = m_mgr_->a_star(m_location_->get_point(), target_location, m_team_);
				m_is_moving_ = true;
			}
		}
		else //no ammo
			reload();
	}
}


void Sniper::choose_action()
{
	int scared_hp = (int)(m_max_hp_ * 3.0 / 4.0);
	if (m_ammo_ <= m_shooting_ammo_cost_)
	{
		reload();
	}
	else if ((m_cur_hp_ >= scared_hp)
		|| m_collision_ == true  //the collision flag is to get rid of two player stuck in  a corridor.
		|| m_idle_counter_ > 2) // if the player is sitting in place for too long, go fight someone.
	{
		fight();
	}
	else if (m_cur_hp_ < scared_hp)
	{
		heal();
	}
	else
		heal();

	if (m_is_moving_)
		fill_path_stack();
}




