#pragma once
#include "Player.h"


class GameMgr;
class Team;

class Survivor :
	public Player
{
public:
	Survivor(GameMgr* mgr, int id, Team* team, Node* location, int steps_to_rethink = DEFAULT_STEPS_TO_RETHINK,
		int max_ammo = 3, int maxHP = 180,
		int grenade_cost = -1, int shooting_ammo_cost = -1, int melee_ammo_cost = -1,
		int grenade_dmg = -1, int shooting_dmg = -1, int melee_dmg = -1);

	void show_me() const;

private:

	///<summary>
	/// The brain of the player, will decide what kind of target to look for
	/// according to the player status(HP and ammo).
	///</summary>
	virtual void choose_action();
};

