#include "Player.h"
#include "GLUT.H"


Player::Player(Team* team, Node* location, const int max_ammo, const int maxHP) :
	m_team_(team), m_location_(location),
	m_ammo_(max_ammo), m_max_ammo_(max_ammo),
	m_cur_hp_(maxHP), m_max_hp_(maxHP)
{
	double len;

	//random starting direction, later when we add the brain of the player 
	//the direction will be chosen somewhat intelligently before each move.  
	m_dirx_ = (int)(rand() % 3) - 1;
	m_diry_ = (int)(rand() % 3) - 1;
	m_is_moving_ = false;

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

}

///<summary>
/// Go to the nearest ammo dump to refill your ammo.
///</summary>
void Player::reload()
{

}

///<summary>
/// Find a target and attack if in range.
///</summary>
void Player::fight()
{

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

void Player::move(Maze maze) const
{
	int x = m_location_->get_point().get_row();
	int y = m_location_->get_point().get_col();

	if (m_is_moving_ && (maze.get_at_pos(x + m_dirx_,y + m_diry_).get_value() == SPACE || maze.get_at_pos(x + m_dirx_,y + m_diry_).get_value() == SPACE ))
	{
		m_location_->get_point().set_row(x + m_dirx_);
		m_location_->get_point().set_col(y + m_diry_);
		//TODO:
		//will need to add setValue to the node i am at (he is now SPACE) and the new node (he is now PLAYER).
		//right now this function holds a copy of the maze and can't effect his nodes, so either the method will
		// recive somthing else or some other function in another class will recive the node coordinates and will 
		// deal with the change in my behalf.

	}

}

void Player::set_dir(double angle)
{
	m_dirx_ = cos(angle);
	m_diry_ = sin(angle);
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
