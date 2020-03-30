#pragma once

#include "Maze.h"
#include "Team.h"
#include "Node.h"
#include "PickupObject.h"
#include "Player.h"
#include "Grenade.h"

const int num_of_teams = 2;

class GameMgr
{
public:
	static GameMgr& get_instance();
	void generate_maze();
	void init_pickup(PickupType type, const int color_type);
	void generate_pickups();
	void init_game();
	void add_team(Team* team);
	void generate_teams();
	Maze& get_maze();
	std::vector<Team*>& get_teams();
	Point2D& find_nearest_pickup(Point2D& location, PickupType type);
	Point2D& find_nearest_enemy(Point2D& location, Team& my_team,bool& is_shootable);
	Node* a_star(Point2D& start, Point2D& target);
	bool shoot(Player* calling_player, Point2D& target);
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
		std::priority_queue <Node*, std::vector<Node*>, CompareNodes>& pq);
	void check_node(int row, int col, Node* pn, std::vector<Node>& gray, std::vector<Node>& black,
	                std::priority_queue <Node*, std::vector<Node*>, CompareNodes>& pq);

	GameMgr() = default;

	Maze maze_;
	std::vector<Team*> teams_;
	std::vector<PickupObject> pickup_objects_;
	bool is_game_over_;

	Player* get_player_at_pos(Point2D& position);
	Player& get_player_at_pos_ref(Point2D& position);
	PickupObject* get_pickup_at_pos(Point2D& position);
	double map_[maze_size][maze_size];

};
