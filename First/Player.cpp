#include "Player.h"
#include "GLUT.H"


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