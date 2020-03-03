#include "Player.h"
#include "GLUT.H"


Player::Player(Team* team, Node* location, int max_ammo, int maxHP) :
	m_team(team), m_location(location),
	m_ammo(max_ammo), m_maxAmmo(max_ammo),
	m_curHP(maxHP), m_maxHP(maxHP)
{
	double len;

	//random starting direction, later when we add the brain of the player 
	//the direction will be chosen somewhat intelligently before each move.  
	m_dirx = (int)(rand() % 3) - 1;
	m_diry = (int)(rand() % 3) - 1;
	m_isMoving = false;

}

void Player::showMe()
{
	double y = m_location->getPoint().getRow() ;
	double x = m_location->getPoint().getCol() ;

	int R = m_team->GetColor()[0];
	int G = m_team->GetColor()[1];
	int B = m_team->GetColor()[2];

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
void Player::RunAway()
{

}

///<summary>
/// Go the nearest health station.
///</summary>
void Player::Heal()
{

}

///<summary>
/// Go to the nearest ammo dump to refill your ammo.
///</summary>
void Player::Reload()
{

}

///<summary>
/// Find a target and attack if in range.
///</summary>
void Player::Fight()
{

}

Node* Player::GetLocation()
{
	return m_location;
}

void Player::SetIsMoving(bool move)
{
	m_isMoving = move;
}

bool Player::GetIsMoving()
{
	return m_isMoving;
}

void Player::move(Maze maze)
{
	int x = m_location->getPoint().getRow();
	int y = m_location->getPoint().getCol();

	if (m_isMoving && (maze.get_at_pos(x + m_dirx,y + m_diry).GetValue() == SPACE || maze.get_at_pos(x + m_dirx,y + m_diry).GetValue() == SPACE ))
	{
		m_location->getPoint().setRow(x + m_dirx);
		m_location->getPoint().setCol(y + m_diry);
		//TODO:
		//will need to add setValue to the node i am at (he is now SPACE) and the new node (he is now PLAYER).
		//right now this function holds a copy of the maze and can't effect his nodes, so either the method will
		// recive somthing else or some other function in another class will recive the node coordinates and will 
		// deal with the change in my behalf.

	}

}

void Player::SetDir(double angle)
{
	m_dirx = cos(angle);
	m_diry = sin(angle);
}

//NOT DONE. maybe will be needed for the danger map. TBD.
void Player::SimulateMotion(double map[MSZ_player][MSZ_player], Node maze[MSZ_player][MSZ_player])
{
	int x = m_location->getPoint().getRow();
	int y = m_location->getPoint().getCol();


	while (maze[x + m_dirx][y + m_diry].GetValue() == SPACE)
	{
		map[x][y] += player_delta;
		x += m_dirx;
		y += m_diry;
		
	}
}
