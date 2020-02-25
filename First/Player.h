#pragma once

#include "Team.h"
#include "Node.h"


//TODO: remove this when we have this value in the utils class.
static const int MSZ_player = 100;

class Player
{
private:
	Team* m_team;
	Node* m_location;

	int m_ammo;
	const int m_maxAmmo;
	int m_curHP;
	const int m_maxHP;

	int m_dirx, m_diry;
	bool m_isMoving;


	const double player_delta = 0.0001;


public:

	Player(Team* team, Node* location, int max_ammo = 10, int maxHP = 10);

	void showMe();

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


	Node* GetLocation();

	void SetIsMoving(bool move);
	bool GetIsMoving();
	void move(Node maze[MSZ_player][MSZ_player]);
	void SetDir(double angle);
	void SimulateMotion(double map[MSZ_player][MSZ_player], Node maze[MSZ_player][MSZ_player]);


};

