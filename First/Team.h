#pragma once

#include <string>
#include <vector>
#define COLOR_LENGTH 3
#include "Player.h"

 static int default_color[] = { 0 , 0 , 1 }; //blue

class Team
{


private:
	std::vector<Player*> m_players_;

	//color in RGB
	 int m_color_[COLOR_LENGTH];
	int players_alive_;
	double** map_;

public:
	Team(const int color[COLOR_LENGTH] = default_color, std::vector<Player*> players = {}) ;
	~Team();
	void clear_map() const;
	std::vector<Player*>& get_teammates();
	void add_player(Player* player);
	const int* get_color() const;
	bool compare_color(const int* other_color);
	void reduce_players_alive(int count);
	int get_players_alive() const;
	std::string get_team_name() const;
	double** get_map();
	

	bool operator!=(const Team& rhs) const {
		return
			m_color_ == rhs.get_color();
	}
};

