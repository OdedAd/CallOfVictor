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

const int* Team::get_color() const
{
	return m_color_;
}

bool Team::compare_color(const int* other_color)
{
	for (int i = 0; i < COLOR_LENGTH; ++i)
	{
		if(m_color_[i] != other_color[i])
		{
			return false;
		}
	}
	return true;
}
