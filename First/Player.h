#pragma once

#include <stack>
#include "Maze.h"
#include "Node.h"

class GameMgr;
class Team;

class Player
{
private:
	GameMgr* m_mgr_;
	Team* m_team_;
	Node* m_location_;
	Node* m_cur_target_node_;
	std::stack<Point2D*> m_cur_path_to_target_;

	const int m_id_;
	int m_ammo_;
	int m_grenade_cost_;
	const int m_max_ammo_;
	int m_cur_hp_;
	const int m_max_hp_;
	int m_old_value_ = 0;

	int m_dirx_, m_diry_; //deprecated
	bool m_is_moving_;

	bool m_is_running_for_hp_cond_;
	bool m_collision_;
	int m_idle_counter_;

	const double player_delta_ = 0.0001;

	void fill_path_stack();


public:

	Player(GameMgr* mgr ,int id, Team* team, Node* location, int max_ammo = 10, int max_hp = 10, int grenade_cost = -1);
	~Player() = default;
	void show_me() const;

	///<summary>
	/// Run away to a safer place.
	///</summary>
	void run_away();

	///<summary>
	/// Go the nearest health station.
	///</summary>
	void heal();

	///<summary>
	/// Go to the nearest ammo dump to refill your ammo.
	///</summary>
	void reload();

	///<summary>
	/// Find a target and attack if in range.
	///</summary>
	void fight();


	///<summary>
	/// The brain of the player, will decide what kind of target to look for
	/// according to the player status(HP and ammo).
	///</summary>
	void choose_direction();

	///<summary>
	/// Reaction to getting hit.
	///</summary>
	void get_hit(int damage);

	Node* get_location() const;

	void set_is_moving(bool move);
	bool get_is_moving() const;
	void move(Maze& maze);
	void set_dir(double angle); //deprecated
	void simulate_motion(double map[maze_size][maze_size], Maze maze) const;
	void set_hp(int value);
	int get_hp() const;
	int get_max_hp() const;
	void set_ammo(int value);
	int get_ammo() const;
	int get_old_value() const;
	Team* get_team() const;
	int get_id() const;
	bool is_in_room() const;
};

