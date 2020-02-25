#include "Team.h"



Team::Team(int color[3], std::vector<Player*> players) :
	m_players(players) 
{
	for(int i = 0; i < 3; i++)
		m_color[i] = color[i];
}


std::vector<Player*>& Team::GetTeammates()
{
	return m_players;
}

void Team::AddPlayer(Player* player)
{
	m_players.push_back(player);
}

int* Team::GetColor()
{
	return m_color;
}