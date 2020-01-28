#pragma once

#include "Team.h"
#include "Node.h"

class Player
{
private:
	Team* m_team;
	Node* m_location;

	int m_ammo;
	const int m_maxAmmo;
	int m_curHP;
	const int m_maxHP;


public:

	Player(Team* team , Node* location, int max_ammo = 10, int maxHP = 10) :
		m_team(team), m_location(location),
		m_ammo(max_ammo), m_maxAmmo(max_ammo),
		m_curHP(maxHP), m_maxHP(maxHP) {}

	///<summary>
	/// Run away to a safer place.
	///</summary>
	void RunAway();

	///<summary>
	/// Go the nearest health station.
	///</summary>
	void Heal();

	///<summary>
	/// Go to the nearest ammo dump to refill your ammo.
	///</summary>
	void Reload();

	///<summary>
	/// Find a target and attack if in range.
	///</summary>
	void Fight();



};

