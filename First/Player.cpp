#include "Player.h"

#include <iostream>

#include "GLUT.H"

#include "GameMgr.h"

Player::Player(GameMgr* mgr ,Team* team, Node* location, const int max_ammo, const int maxHP) :
	m_mgr_(mgr), m_team_(team), m_location_(location),
	m_ammo_(max_ammo), m_max_ammo_(max_ammo),
	m_cur_hp_(maxHP), m_max_hp_(maxHP)
{
	//random starting direction, later when we add the brain of the player 
	//the direction will be chosen somewhat intelligently before each move.  
	m_dirx_ = 0;//(int)(rand() % 3) - 1;
	m_diry_ = 0;//(int)(rand() % 3) - 1;
	m_is_moving_ = false;

	m_cur_target_node_ = nullptr;
}

void Player::show_me() const
{
	double y = m_location_->get_point().get_row() ;
	double x = m_location_->get_point().get_col() ;

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

}

///<summary>
/// Go the nearest health station.
///</summary>
void Player::heal()
{
	std::cout<< "in fight function: my location row = " << m_location_->get_point().get_row()
		<<" col = " << m_location_->get_point().get_col() << std::endl;
	
	Point2D target = m_mgr_->find_nearest_pickup(m_location_->get_point(), PickupType::med_kit);

	std::cout << "in fight function: target_location row = " << target.get_row()
		<< " col = " << target.get_col() << std::endl;
	
	m_cur_target_node_ = m_mgr_->a_star(m_location_->get_point(), target);
}

///<summary>
/// Go to the nearest ammo dump to refill your ammo.
///</summary>
void Player::reload()
{
	std::cout<< "in fight function: my location row = " << m_location_->get_point().get_row()
		<<" col = " << m_location_->get_point().get_col() << std::endl;
	
	Point2D target = m_mgr_->find_nearest_pickup(m_location_->get_point(), PickupType::ammo);

	std::cout << "in fight function: target_location row = " << target.get_row()
		<< " col = " << target.get_col() << std::endl;
	
	m_cur_target_node_ = m_mgr_->a_star(m_location_->get_point(), target);
}

///<summary>
/// Find a target and attack if in range.
///</summary>
void Player::fight()
{
	bool is_shootable;
	std::cout<< "in fight function: my location row = " << m_location_->get_point().get_row()
		<<" col = " << m_location_->get_point().get_col() << std::endl;
	Point2D target_location;
	target_location = m_mgr_->find_nearest_enemy(m_location_->get_point(), *m_team_, is_shootable);

	std::cout << "in fight function: target_location row = " << target_location.get_row()
		<< " col = " << target_location.get_col() << std::endl;

	m_cur_target_node_ = m_mgr_->a_star(m_location_->get_point(), target_location);
}

static Point2D nextPoint;


///<summary>
/// The brain of the player, will decide what kind of target to look for 
/// according to the player status(HP and ammo).
///</summary>
void Player::choose_direction()
{
	//Very very problematic
	//for example if AMMO = 0 and HP is bigger then 5 he will go and fight
	if (m_cur_hp_ >= 5)
	{
		fight();
	}
	else if (m_cur_hp_ < 3)
	{
		heal();
	}
	else if (m_ammo_ >= 5)
	{
		run_away();
	}
	else if (m_ammo_ < 5)
	{
		reload();
	}


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
		m_cur_path_to_target_.push(new Point2D (next_node->get_point().get_row() , next_node->get_point().get_col()));// possible waste of memory

		delete next_node;
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
	if (m_is_moving_)
	{
		//maybe this two variables can be class members and not static.
		static int old_value = 0; // the last value of the node.
		static int step_counter = 0;

		int cur_old_value = old_value;

		if (m_cur_path_to_target_.empty() || step_counter > 10) //every 10 steps reset the m_cur_path_to_target_ and make new one.
		{
			while (m_cur_path_to_target_.empty() == false)
				m_cur_path_to_target_.pop();
			step_counter = 0;

			choose_direction();
		}

		Point2D* nextPoint = m_cur_path_to_target_.top();
		m_cur_path_to_target_.pop(); //remove the top point from the stack

		m_is_moving_ = true;

		int cur_x = m_location_->get_point().get_row();
		int cur_y = m_location_->get_point().get_col();

		std::cout << "x = " << cur_x << " y = " << cur_y << std::endl;

		int next_x = nextPoint->get_row();
		int next_y = nextPoint->get_col();

		if (m_is_moving_ && (maze.get_at_pos(next_x, next_y).get_value() == SPACE || maze.get_at_pos(next_x, next_y).get_value() == PATH))
		{
			m_location_->set_point(*nextPoint);

			old_value = maze.get_at_pos(next_x, next_y).get_value();
			maze.get_at_pos(next_x, next_y).set_value(PLAYER); //upadte the new location
			maze.get_at_pos(cur_x, cur_y).set_value(cur_old_value); //upadte the old location

			std::cout << "x = " << cur_x << " y = " << cur_y << std::endl;
			std::cout << "next_x = " << next_x << " next_y = " << next_y << std::endl;
		}
		++step_counter;
	}
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


	while (maze.get_at_pos(x + m_dirx_,y + m_diry_).get_value() == SPACE)
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

void Player::set_ammo(const int value)
{
	this->m_ammo_ = value;
}
