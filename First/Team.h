#pragma once

//#include "Player.h"
#include <vector>

class Player;

class Team
{


private:
	std::vector<Player*> m_players;

	int m_color;

public:
	Team(int color, std::vector<Player*> players = {}) :
		m_color(color), m_players(players)  {}

	std::vector<Player*>& GetTeammates();
	void AddPlayer(Player* player);
};

