#include "Team.h"

#include <iostream>


//todo: check if vector is necessary
//TODO: CLEAR ALLOCATIONS OF EACH MAP
Team::Team(const int color[COLOR_LENGTH], const std::vector<Player*> players) :
	m_players_(players),players_alive_(0)
{
	for (int i = 0; i < 3; i++)
		m_color_[i] = color[i];

	this->map_ = new double*[maze_size];
	for (int i = 0; i < maze_size; ++i)
	{
		this->map_[i] = new double[maze_size];
	}

	clear_map();
}

Team::~Team()
{
	for (auto player : this->m_players_)
	{
		delete player;
	}

	for (int i = 0; i < maze_size; ++i)
	{
		delete[] map_[i];
	}
	delete[] map_;
}

void Team::clear_map() const
{
	for (int i = 0; i < maze_size; ++i)
	{
		for (int j = 0; j < maze_size; ++j)
		{
			map_[i][j] = 0;
		}
	}
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
	return this->players_alive_;
}

std::string Team::get_team_name() const
{
	std::string str;
	for (auto m_color : m_color_)
	{
		str += std::to_string(m_color);
	}
	return str;
}

double** Team::get_map()
{
	return this->map_;
}


