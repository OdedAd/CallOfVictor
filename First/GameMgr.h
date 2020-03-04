#pragma once
#include "Maze.h"
#include "Team.h"
#include "Node.h"
#include "PickupObject.h"
#include "Player.h"
#include "Room.h"

const int num_of_teams = 2;

class GameMgr
{
public:
	void generate_maze();
	void init_game();
	void add_team(const Team& team);
	void generate_teams();
	Maze& get_maze();
	std::vector<Team>& get_teams();
	Point2D& find_nearest_pickup(Point2D& location, PickupType type);
	Point2D& find_nearest_enemy(Point2D& location, Team& my_team,bool& is_shootable);
	Node* a_star(Point2D& start, Point2D& target);
private:
	Maze maze_;
	std::vector<Team> teams_;
	std::vector<PickupObject> pickup_objects_;
};
