#include "Player.h"

#include <iostream>

#include "GLUT.H"

#include "GameMgr.h"


Player::Player(GameMgr* mgr, int id, Team* team, Node* location, const int max_ammo, const int maxHP, int grenade_cost) :
	m_mgr_(mgr), m_ID_(id), m_team_(team), m_location_(location),
	m_ammo_(max_ammo), m_max_ammo_(max_ammo),
	m_cur_hp_(maxHP), m_max_hp_(maxHP)
{
	//random starting direction, later when we add the brain of the player
	//the direction will be chosen somewhat intelligently before each move.
	m_dirx_ = 0;//(int)(rand() % 3) - 1;
	m_diry_ = 0;//(int)(rand() % 3) - 1;
	m_is_moving_ = false;
	m_is_running_for_hp_cond_ = false;
	m_collision = false;

	m_cur_target_node_ = nullptr;

	if (grenade_cost < 0)
		m_grenade_cost = max_ammo / 2;
	else
		m_grenade_cost = grenade_cost;

	m_idle_counter = 0;
}

void Player::show_me() const
{
	double y = m_location_->get_point().get_row();
	double x = m_location_->get_point().get_col();

	int R = m_team_->get_color()[0];
	int G = m_team_->get_color()[1];
	int B = m_team_->get_color()[2];

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
/// Run away to a safer place.
///</summary>
void Player::run_away()
{
	//get the room where the player is now
	//auto current_room = GameMgr::get_instance().get_maze().get_room_at(m_location_->get_point());

	//get heat map in room -NOT YET

	//std::cout << "in run function: my location row = " << m_location_->get_point().get_row()
		//<< " col = " << m_location_->get_point().get_col() << std::endl;
	//get cover (find minimum in that matrix)
	if (m_is_running_for_hp_cond_)
	{
		heal();
	}
	else
	{
		Point2D target = Utils::find_minimum_in_matrix(GameMgr::get_instance().get_maze());
		if (target == m_location_->get_point())
		{
			m_is_running_for_hp_cond_ = true;
		}
		else
		{
			//std::cout << "in run function: target_location row = " << target.get_row()
				//<< " col = " << target.get_col() << std::endl;
			//move to the minimum target
			m_cur_target_node_ = m_mgr_->a_star(m_location_->get_point(), target, m_team_);
			m_is_moving_ = true;
		}
	}
}

///<summary>
/// Go the nearest health station.
///</summary>
void Player::heal()
{
	//std::cout << "in heal function: my location row = " << m_location_->get_point().get_row()
		//<< " col = " << m_location_->get_point().get_col() << std::endl;

	Point2D target = m_mgr_->find_nearest_pickup(m_location_->get_point(), PickupType::med_kit);

	//std::cout << "in heal function: target_location row = " << target.get_row()
		//<< " col = " << target.get_col() << std::endl;

	m_cur_target_node_ = m_mgr_->a_star(m_location_->get_point(), target, m_team_);

	bool is_successful = false;
	is_successful = m_mgr_->pickup(this, target);

	if (is_successful) //picked up the medkit successfully, heal up.
	{
		m_cur_hp_ = m_max_hp_;
		m_is_moving_ = false;
		m_is_running_for_hp_cond_ = false;
	}
	else //couldn't pick up the medkit, need to move closer.
	{
		m_cur_target_node_ = m_mgr_->a_star(m_location_->get_point(), target, m_team_);
		m_is_moving_ = true;
	}
}

///<summary>
/// Go to the nearest ammo dump to refill your ammo.
///</summary>
void Player::reload()
{
	//std::cout << "in reload function: my location row = " << m_location_->get_point().get_row()
		//<< " col = " << m_location_->get_point().get_col() << std::endl;

	Point2D target = m_mgr_->find_nearest_pickup(m_location_->get_point(), PickupType::ammo);

	//std::cout << "in reload function: target_location row = " << target.get_row()
		//<< " col = " << target.get_col() << std::endl;

	if (target.get_col() != -1) //a target was found, procced to pickup.
	{
		bool is_successful = false;
		is_successful = m_mgr_->pickup(this, target);

		if (is_successful) //picked up the ammo successfully, stock up on ammo.
		{
			m_ammo_ = m_max_ammo_;
			m_is_moving_ = false;
			m_is_running_for_hp_cond_ = false;
		}
		else //couldn't pick up the ammo, need to move closer.
		{
			m_cur_target_node_ = m_mgr_->a_star(m_location_->get_point(), target, m_team_);
			m_is_moving_ = true;
		}
	}
	else //no ammo available, abandon all hope like a headless chicken
	{
		run_away();
	}

}

///<summary>
/// Find a target and attack if in range.
///</summary>
void Player::fight()
{
	bool is_shootable;
	//std::cout << "in fight function: my location row = " << m_location_->get_point().get_row()
		//<< " col = " << m_location_->get_point().get_col() << std::endl;
	Point2D target_location;
	target_location = m_mgr_->find_nearest_enemy(m_location_->get_point(), *m_team_, is_shootable);
	if (target_location.get_col() == -1 && target_location.get_row() == -1)
	{
		m_is_moving_ = false;
	}
	else
	{
		//std::cout << "in fight function: target_location row = " << target_location.get_row()
			//<< " col = " << target_location.get_col() << std::endl;

		bool is_successful = false;
		double distance_from_target = m_location_->get_point().get_distance(target_location);
		if (m_ammo_ >= m_grenade_cost && distance_from_target > 4 && distance_from_target < 10)
		{
			is_successful = m_mgr_->throw_grenade(this, target_location);

			if (is_successful)
			{
				m_ammo_ -= m_grenade_cost;
				m_is_moving_ = false;
			}

		}
		else if(m_ammo_ > 0)
		{
			is_successful = m_mgr_->shoot(this, target_location);

			if (is_successful)
			{
				--m_ammo_;
				m_is_moving_ = false;
			}

		}
		else //no ammo
			reload();

		if (is_successful == false)
		{
			m_cur_target_node_ = m_mgr_->a_star(m_location_->get_point(), target_location, m_team_);
			m_is_moving_ = true;
		}
	}
}


///<summary>
/// The brain of the player, will decide what kind of target to look for 
/// according to the player status(HP and ammo).
///</summary>
void Player::choose_direction()
{
	//Very very problematic
	//for example if AMMO = 0 and HP is bigger then 5 he will go and fight
	if ((m_cur_hp_ >= m_max_hp_ / 2 && m_ammo_> 0)
		|| m_collision == true  //the collision flag is to get rid of two player stuck in  a corridor.
		|| m_idle_counter > 3) // if the player is sitting in place for too long, go fight someone.
	{
		fight();
	}
	else if (m_cur_hp_ < m_max_hp_ / 4)
	{
		heal();
	}
	else if (m_ammo_ >= 5)
	{
		run_away();
	}
	else if (m_ammo_ < 5 )
	{
		reload();
	}
	else
		heal();

	if (m_is_moving_)
		fill_path_stack();
}


void Player::fill_path_stack()
{
	if (m_cur_target_node_ != nullptr && m_cur_target_node_->get_parent() != nullptr)
	{
		Node* next_node = new Node(m_cur_target_node_->get_point(), m_cur_target_node_->get_target(), m_cur_target_node_->get_value()
			, m_cur_target_node_->get_g(), m_cur_target_node_->get_parent());

		while (next_node->get_parent() != nullptr &&
			next_node->get_parent()->get_point() != m_location_->get_point())
		{
			m_cur_path_to_target_.push(&next_node->get_point());
			next_node = next_node->get_parent();
		}
		m_cur_path_to_target_.push(new Point2D(next_node->get_point().get_row(), next_node->get_point().get_col()));// possible waste of memory

		delete next_node;
	}
}

void Player::get_hit(const int damage)
{
	m_cur_hp_ -= damage;
	std::cout << "Player " << m_ID_ << " Recived : " << damage << " damage " << std::endl;

	if (m_cur_hp_ <= 0)
	{
		std::cout << "Player " << m_ID_ << " died" << std::endl;
		GameMgr::get_instance().get_maze().get_at_pos(m_location_->get_point()).set_value(SPACE);
		get_team()->reduce_players_alive(1);
	}
}

Node* Player::get_location() const
{
	return m_location_;
}

void Player::set_is_moving(const bool move)
{
	m_is_moving_ = move;
}

bool Player::get_is_moving() const
{
	return m_is_moving_;
}

void Player::move(Maze& maze)
{
	//maybe this two variables can be class members and not static.
	//static int old_value = 0; // the last value of the node.
	static int step_counter = 0;

	if (m_cur_path_to_target_.empty() || step_counter > 2) //every 2 steps reset the m_cur_path_to_target_ and make new one.
	{
		while (m_cur_path_to_target_.empty() == false)
			m_cur_path_to_target_.pop();
		step_counter = 0;

		choose_direction();
	}

	if (m_is_moving_ && m_cur_path_to_target_.empty() == false)
	{
		m_idle_counter = 0;
		int cur_old_value = m_old_value;

		Point2D* nextPoint = m_cur_path_to_target_.top();
		m_cur_path_to_target_.pop(); //remove the top point from the stack

		int cur_x = m_location_->get_point().get_row();
		int cur_y = m_location_->get_point().get_col();

		//std::cout << "x = " << cur_x << " y = " << cur_y << std::endl;

		int next_x = nextPoint->get_row();
		int next_y = nextPoint->get_col();

		int next_value = maze.get_at_pos(next_x, next_y).get_value();
		if (next_value == SPACE || next_value == PICKUP_AMMO
			|| next_value == PICKUP_MED)
		{
			m_location_->set_point(*nextPoint);

			m_old_value = maze.get_at_pos(next_x, next_y).get_value();
			maze.get_at_pos(next_x, next_y).set_value(PLAYER); //upadte the new location
			maze.get_at_pos(cur_x, cur_y).set_value(cur_old_value); //upadte the old location

			//std::cout << "x = " << cur_x << " y = " << cur_y << std::endl;
			//std::cout << "next_x = " << next_x << " next_y = " << next_y << std::endl;

			m_collision = false;
		}
		else if (next_value == PLAYER)
			m_collision = true;
	}
	else
		++m_idle_counter;

	++step_counter;

}

//deprecated
//Changes = Rounded here
void Player::set_dir(double angle)
{
	m_dirx_ = (int)round(cos(angle));
	m_diry_ = (int)round(sin(angle));
}

//NOT DONE. maybe will be needed for the danger map. TBD.
void Player::simulate_motion(double map[maze_size][maze_size], Maze maze) const
{
	int x = m_location_->get_point().get_row();
	int y = m_location_->get_point().get_col();


	while (maze.get_at_pos(x + m_dirx_, y + m_diry_).get_value() == SPACE)
	{
		map[x][y] += player_delta_;
		x += m_dirx_;
		y += m_diry_;

	}
}

void Player::set_hp(const int value)
{
	this->m_cur_hp_ = value;
}

int Player::get_hp() const
{
	return m_cur_hp_;
}

int Player::get_max_hp() const
{
	return m_max_hp_;
}

void Player::set_ammo(const int value)
{
	this->m_ammo_ = value;
}

int Player::get_ammo() const
{
	return m_ammo_;
}

int Player::get_old_value() const
{
	return m_old_value;
}

Team* Player::get_team() const
{
	return this->m_team_;
}

int Player::get_ID() const
{
	return m_ID_;
}
