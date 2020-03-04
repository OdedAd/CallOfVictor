#pragma once

#include <vector>
#define COLOR_LENGTH 3

class Player;

 static int default_color[] = { 0 , 0 , 1 }; //blue

class Team
{


private:
	std::vector<Player*> m_players_;

	//color in RGB
	 int m_color_[COLOR_LENGTH]; 

public:
	Team(const int color[COLOR_LENGTH] = default_color, std::vector<Player*> players = {}) ;

	std::vector<Player*>& get_teammates();
	void add_player(Player* player);
	const int* get_color() const;

	bool operator!=(const Team& rhs) const {
		return
			m_color_ == rhs.get_color();
	}
};

