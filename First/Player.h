#pragma once

#include "Maze.h"
#include "Team.h"
#include "Node.h"
#include <stack>

class GameMgr;

class Player
{
private:
	GameMgr* m_mgr_;
	Team* m_team_;
	Node* m_location_;
	Node* m_cur_target_node_;
	std::stack<Point2D*> m_cur_path_to_target_;

	int m_ammo_;
	const int m_max_ammo_;
	int m_cur_hp_;
	const int m_max_hp_;

	int m_dirx_, m_diry_; //deprecated
	bool m_is_moving_;


	const double player_delta_ = 0.0001;


public:

	Player(GameMgr* mgr ,Team* team, Node* location, int max_ammo = 10, int maxHP = 10);

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


	Node* get_location() const;

	void set_is_moving(bool move);
	bool get_is_moving() const;
	void move(Maze& maze);
	void set_dir(double angle); //deprecated
	void simulate_motion(double map[maze_size][maze_size], Maze maze) const;
	void set_hp(int value);
	void set_ammo(int value);

};

