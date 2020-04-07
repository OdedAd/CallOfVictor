#include "Survivor.h"

#include "GameMgr.h"

Survivor::Survivor(GameMgr* mgr, int id, Team* team, Node* location, int max_ammo, int maxHP,
	int grenade_cost, int shooting_ammo_cost, int melee_ammo_cost,
	int grenade_dmg, int shooting_dmg, int melee_dmg) :
	Player(mgr, id, team, location, max_ammo, maxHP,
		grenade_cost, shooting_ammo_cost, melee_ammo_cost,
		grenade_dmg, shooting_dmg, melee_dmg)
{

	if (grenade_cost < 0)
		m_grenade_ammo_cost_ = max_ammo;
	else
		m_grenade_ammo_cost_ = grenade_cost;

	if (shooting_ammo_cost < 0)
		m_shooting_ammo_cost_ = 1;
	else
		m_shooting_ammo_cost_ = shooting_ammo_cost;

	if (melee_ammo_cost < 0)
		m_melee_ammo_cost_ = max_ammo;
	else
		m_melee_ammo_cost_ = melee_ammo_cost;


	if (grenade_dmg < 0)
		m_grenade_dmg_ = maxHP / 5;
	else
		m_grenade_dmg_ = grenade_dmg;

	if (shooting_dmg < 0)
		m_shooting_dmg_ = (int)(maxHP * (1.0 / 5.0));
	else
		m_shooting_dmg_ = shooting_dmg;

	if (melee_dmg < 0)
		m_melee_dmg_ = maxHP / 4;
	else
		m_melee_dmg_ = melee_dmg;

	m_throw_dis_min_ = 8;
	m_throw_dis_max_ = 10;
	m_stab_dis_max_ = 2;
}

void Survivor::show_me() const
{
	double y = m_location_->get_point().get_row();
	double x = m_location_->get_point().get_col();

	double R = m_team_->get_color()[0] + 0.35;;
	double G = m_team_->get_color()[1] - 0.35;
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


void Survivor::choose_action()
{
	int scared_hp = (int)(m_max_hp_ * 3.0 / 5.0);

	if (m_cur_hp_ < scared_hp)
	{
		heal();
	}
	else if (m_ammo_ <= m_shooting_ammo_cost_)
	{
		reload();
	}
	else if ((m_cur_hp_ >= scared_hp)
		|| m_collision_ == true  //the collision flag is to get rid of two player stuck in  a corridor.
		|| m_idle_counter_ > 2) // if the player is sitting in place for too long, go fight someone.
	{
		fight();
	}
	else
		heal();

	if (m_is_moving_)
		fill_path_stack();
}
