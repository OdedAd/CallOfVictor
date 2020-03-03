#pragma once
#include "Maze.h"
#include "Team.h"
#include "Node.h"
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
private:
	Maze* maze_ = nullptr;
	std::vector<Team> teams_;
	
};
