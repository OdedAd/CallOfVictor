#pragma once

#include <stack>
#include <iostream>
#include "Maze.h"
#include "Node.h"
#include "GLUT.H"

static const int SNIPER_TYPE = 0;
static const int BERSERKER_TYPE = 1;
static const int GRENADIER_TYPE = 2;
static const int SURVIVOR_TYPE = 3;

static const int DEFAULT_STEPS_TO_RETHINK = 2;


class GameMgr;
class Team;

class Player
{
protected:
	GameMgr* m_mgr_;
	Team* m_team_;
	Node* m_location_;
	Node* m_cur_target_node_;
	std::stack<Point2D*> m_cur_path_to_target_;

	const int m_id_;
	int m_ammo_;
	int m_max_ammo_;
	int m_cur_hp_;
	int m_max_hp_;

	int m_grenade_ammo_cost_;
	int m_shooting_ammo_cost_;
	int m_melee_ammo_cost_;

	int m_grenade_dmg_;
	int m_shooting_dmg_;
	int m_melee_dmg_;

	int m_throw_dis_min_;
	int m_throw_dis_max_;
	int m_stab_dis_max_;

	int m_old_value_ = 0;
	int m_dirx_, m_diry_; //deprecated
	bool m_is_moving_;
	bool m_is_in_position_;
	int m_step_counter_;
	const int m_steps_to_rethink_; // how many steps to move without calling choose_action().

	bool m_is_running_for_hp_cond_;
	bool m_collision_;
	int m_idle_counter_;

	const double player_delta_ = 0.0001;

	void fill_path_stack();


	///<summary>
	/// Run away to a safer place.
	///</summary>
	virtual void run_away();

	///<summary>
	/// Go the nearest health station.
	///</summary>
	virtual void heal();

	///<summary>
	/// Go to the nearest ammo dump to refill your ammo.
	///</summary>
	virtual void reload();


	///<summary>
	/// Find a target and attack if in range.
	///</summary>
	virtual void fight();


	///<summary>
	/// The brain of the player, will decide what kind of target to look for
	/// according to the player status(HP and ammo).
	///</summary>
	virtual void choose_action();

public:

	Player(GameMgr* mgr ,int id, Team* team, Node* location, int steps_to_rethink = DEFAULT_STEPS_TO_RETHINK,
		int max_ammo = 10, int max_hp = 100,
		int grenade_cost = -1, int shooting_ammo_cost = -1, int melee_ammo_cost = -1,
		int grenade_dmg = -1, int shooting_dmg = -1, int melee_dmg = -1);
	virtual ~Player() = default;
	virtual void show_me() const;

	bool get_into_position();

	///<summary>
	/// Reaction to getting hit.
	///</summary>
	virtual void get_hit(int damage);

	Node* get_location() const;

	void move(Maze& maze);
	void set_dir(double angle); //deprecated
	void simulate_motion(double map[maze_size][maze_size], Maze maze) const;

	Team* get_team() const;
	int get_id() const;
	void set_is_moving(bool move);
	bool get_is_moving() const;
	void set_hp(int value);
	int get_hp() const;
	int get_max_hp() const;
	void set_ammo(int value);
	int get_ammo() const;
	int get_old_value() const;
	bool is_in_room() const;

	
	int get_grenade_ammo_cost() const
	{
		return m_grenade_ammo_cost_;
	}

	void set_grenade_ammo_cost(const int grenade_ammo_cost)
	{
		m_grenade_ammo_cost_ = grenade_ammo_cost;
	}

	int get_shooting_ammo_cost() const
	{
		return m_shooting_ammo_cost_;
	}

	void set_shooting_ammo_cost(const int shooting_ammo_cost)
	{
		m_shooting_ammo_cost_ = shooting_ammo_cost;
	}

	int get_melee_ammo_cost() const
	{
		return m_melee_ammo_cost_;
	}

	void set_melee_ammo_cost(const int melee_ammo_cost)
	{
		m_melee_ammo_cost_ = melee_ammo_cost;
	}

	int get_grenade_dmg() const
	{
		return m_grenade_dmg_;
	}

	void set_grenade_dmg(const int grenade_dmg)
	{
		m_grenade_dmg_ = grenade_dmg;
	}

	int get_shooting_dmg() const
	{
		return m_shooting_dmg_;
	}

	void set_shooting_dmg(const int shooting_dmg)
	{
		m_shooting_dmg_ = shooting_dmg;
	}

	int get_melee_dmg() const
	{
		return m_melee_dmg_;
	}

	void set_melee_dmg(const int melee_dmg)
	{
		m_melee_dmg_ = melee_dmg;
	}

	int get_throw_dis_min() const
	{
		return m_throw_dis_min_;
	}

	int get_throw_dis_max() const
	{
		return m_throw_dis_max_;
	}

	int get_stab_dis() const
	{
		return m_stab_dis_max_;
	}
};

