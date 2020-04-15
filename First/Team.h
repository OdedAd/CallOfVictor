#pragma once

#include <string>
#include <vector>
#include "Player.h"
#include "Utils.h"

class Team
{


private:
	std::vector<Player*> m_players_;

	//color in RGB
	double m_color_[COLOR_LENGTH];
	int players_alive_;
	double** map_;

public:
	Team(const double color[COLOR_LENGTH] = DEFAULT_TEAM_COLOR, std::vector<Player*> players = {}) ;
	~Team();
	void clear_map() const;
	std::vector<Player*>& get_teammates();
	void add_player(Player* player);
	const double* get_color() const;
	bool compare_color(const double* other_color);
	void reduce_players_alive(int count);
	int get_players_alive() const;
	std::string get_team_name() const;
	double** get_map();
	

	bool operator!=(const Team& rhs) const {
		return
			m_color_ == rhs.get_color();
	}
};

