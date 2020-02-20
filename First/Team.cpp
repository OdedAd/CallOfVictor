#include "Team.h"


std::vector<Player*>& Team::GetTeammates()
{
	return m_players;
}

void Team::AddPlayer(Player* player)
{
	m_players.push_back(player);
}