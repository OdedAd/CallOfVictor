#include "Team.h"


//todo: check if vector is necessary
Team::Team(const int color[COLOR_LENGTH], const std::vector<Player*> players) :
	m_players_(players) 
{
	for(int i = 0; i < 3; i++)
		m_color_[i] = color[i];
}


std::vector<Player*>& Team::get_teammates()
{
	return m_players_;
}

void Team::add_player(Player* player)
{
	m_players_.push_back(player);
}

int* Team::get_color()
{
	return m_color_;
}