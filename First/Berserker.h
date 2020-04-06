#pragma once
#include "Player.h"


class GameMgr;
class Team;

class Berserker :
	public Player
{
public:
	Berserker(GameMgr* mgr, int id, Team* team, Node* location, int max_ammo = 4, int maxHP = 120,
		int grenade_cost = -1, int shooting_ammo_cost = -1, int melee_ammo_cost = -1,
		int grenade_dmg = -1, int shooting_dmg = -1, int melee_dmg = -1);

	void show_me() const;

private:

	///<summary>
	/// The brain of the player, will decide what kind of target to look for
	/// according to the player status(HP and ammo).
	///</summary>
	//virtual void choose_direction();
};

