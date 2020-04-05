#pragma once

#include <stack>
#include <iostream>

#include "Maze.h"
#include "Node.h"
#include "GLUT.H"


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

	const int m_ID_;
	int m_ammo_;
	int m_grenade_cost;
	const int m_max_ammo_;
	int m_cur_hp_;
	const int m_max_hp_;
	int m_old_value = 0;

	int m_dirx_, m_diry_; //deprecated
	bool m_is_moving_;

	bool m_is_running_for_hp_cond_;
	bool m_collision;
	int m_idle_counter;

	const double player_delta_ = 0.0001;

	void fill_path_stack();


public:

	Player(GameMgr* mgr ,int id, Team* team, Node* location, int max_ammo = 10, int maxHP = 10, int grenade_cost = -1);
	virtual ~Player() = default;
	virtual void show_me() const;

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
	virtual void choose_direction();

	///<summary>
	/// Reaction to getting hit.
	///</summary>
	virtual void get_hit(int damage);

	Node* get_location() const;

	void move(Maze& maze);
	void set_dir(double angle); //deprecated
	void simulate_motion(double map[maze_size][maze_size], Maze maze) const;

	Team* get_team() const;
	int get_ID() const;
	void set_is_moving(bool move);
	bool get_is_moving() const;
	void set_hp(int value);
	int get_hp() const;
	int get_max_hp() const;
	void set_ammo(int value);
	int get_ammo() const;
	int get_old_value() const;



};

