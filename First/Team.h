#pragma once

//#include "Player.h"
#include <vector>

class Player;

 static int DEFAULT_COLOR[] = { 0 , 0 , 1 }; //blue

class Team
{


private:
	std::vector<Player*> m_players;

	//color in RGB
	 int m_color[3]; 

public:
	Team(int color[3] = DEFAULT_COLOR, std::vector<Player*> players = {}) ;

	std::vector<Player*>& GetTeammates();
	void AddPlayer(Player* player);
	int* GetColor();
};

