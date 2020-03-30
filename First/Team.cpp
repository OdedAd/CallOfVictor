#include "Team.h"

#include <iostream>


//todo: check if vector is necessary
Team::Team(const int color[COLOR_LENGTH], const std::vector<Player*> players) :
	m_players_(players)
{
	for (int i = 0; i < 3; i++)
		m_color_[i] = color[i];
}


std::vector<Player*>& Team::get_teammates()
{
	return m_players_;
}

void Team::add_player(Player* player)
{
	m_players_.push_back(player);
	players_alive_++;
}

const int* Team::get_color() const
{
	return m_color_;
}

bool Team::compare_color(const int* other_color)
{
	for (int i = 0; i < COLOR_LENGTH; ++i)
	{
		if (m_color_[i] != other_color[i])
		{
			return false;
		}
	}
	return true;
}

void Team::reduce_players_alive(const int count)
{
	this->players_alive_ -= count;
}

int Team::get_players_alive() const
{
	std::cout << "players of team = " <<get_team_name()<< players_alive_ << std::endl;
	return this->players_alive_;
}

std::string Team::get_team_name() const
{
	std::string str;
	for (auto m_color : m_color_)
	{
		str+=std::to_string(m_color);
	}
	return str;
}


