#pragma once

#include "Maze.h"
#include "Team.h"
#include "Node.h"
#include "PickupObject.h"
#include "Grenade.h"

#include "Player.h"
#include "Sniper.h"

#include "LogicBullet.h"

using namespace std;


const int num_of_teams = 2;

//class LogicBullet;

class GameMgr
{
public:
	static GameMgr& get_instance();
	void generate_maze();
	void init_pickup(PickupType type, const int color_type);
	void generate_pickups();
	void init_game();
	void add_team(Team* team);
	bool throw_grenade(Player* calling_player, Point2D& target);
	void generate_teams();
	Maze& get_maze();
	std::vector<Team*>& get_teams();
	//vector<LogicBullet*>& get_bullets();
	vector<Bullet*>& get_bullets();
	vector<Grenade*>& get_grenades();

	void find_nearest_pickup(Point2D& location, Point2D& target, PickupType type);
	Point2D& find_nearest_enemy(Point2D& location, Team& my_team,bool& is_shootable);
	Node* a_star(Point2D& start, Point2D& target, Team* callers_team = nullptr);
	bool shoot(Player* calling_player, Point2D& target);
	bool stab(Player* calling_player, Point2D& target);
	void hit_player(Point2D& target, const int damage);
	bool pickup(Player* calling_player, Point2D& target);
	void generate_map();
	double** get_heat_map();
	void play_one_turn();
	void clear_map();
	bool is_game_over() const;
	void clear_all_resources();

	GameMgr(GameMgr const&) = delete;
	GameMgr(GameMgr const &&) = delete;
	void operator=(GameMgr const&) = delete;

private:
	void delete_team_related_allocations();
	void check_neighbors(Node* pn, std::vector<Node>& gray, std::vector<Node>& black,
		std::priority_queue <Node*, std::vector<Node*>, CompareNodes>& pq, Team* callers_team = nullptr);
	void check_node(int row, int col, Node* pn, std::vector<Node>& gray, std::vector<Node>& black,
	                std::priority_queue <Node*, std::vector<Node*>, CompareNodes>& pq, Team* callers_team = nullptr);

	GameMgr() = default;

	Maze maze_;
	std::vector<Team*> teams_;
	std::vector<PickupObject> pickup_objects_;
	//vector<LogicBullet*> bullets_;
	vector<Bullet*> bullets_;
	vector<Grenade*> grenades_;

	bool is_game_over_;

	Player* get_player_at_pos(Point2D& position);
	Player& get_player_at_pos_ref(Point2D& position);
	PickupObject* get_pickup_at_pos(Point2D& position);
	double map_[maze_size][maze_size];

};
