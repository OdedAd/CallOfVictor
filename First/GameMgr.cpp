#include "GameMgr.h"

///
///Initialize the game. call all relevant initialization function and generate the maze
///
void GameMgr::init_game()
{
	generate_maze();
	generate_teams();
}

///
///Initialize the maze.
///
void GameMgr::generate_maze()
{
	this->maze_.setup_maze();
}

///
///Initialize the teams.
///
void GameMgr::generate_teams()
{
	int rooms[2] = {0,maze_.get_num_existing_rooms()-1};
	for (auto i = 0; i < num_of_teams; ++i)
	{
		int color[3] = {i%2,0,(i+1)%2};
		auto team = new Team(color);
		//add a player in a random location at room 0
		auto team_room = maze_.get_room_at(rooms[i]);
		int k, j;
		const auto max_num_of_players = 5;

		for (auto cur_num_of_players = 0; cur_num_of_players < max_num_of_players; cur_num_of_players++)
		{
			const auto team_room_left_top_row = team_room.get_left_top().get_row();
			const auto team_room_left_top_col = team_room.get_left_top().get_col();
			const auto team_room_right_bottom_row = team_room.get_right_bottom().get_row();
			const auto team_room_right_bottom_col = team_room.get_right_bottom().get_col();

			do
			{
				k = rand() % (team_room_right_bottom_row - team_room_left_top_row) + team_room_left_top_row;
				j = rand() % (team_room_right_bottom_col - team_room_left_top_col) + team_room_left_top_col;
			} while (maze_.get_at_pos(k, j).get_value() == PLAYER); //collision prevention

			maze_.get_at_pos(k, j).set_value(PLAYER);
			team->add_player(new Player(team, &maze_.get_at_pos(k, j)));
		}
		this->add_team(*team);
	}
}

Maze& GameMgr::get_maze()
{
	return this->maze_;
}

std::vector<Team>& GameMgr::get_teams()
{
	return this->teams_;
}

Point2D& GameMgr::find_nearest_pickup(PickupType type)
{
	Point2D p;

	return p;
}

Point2D& GameMgr::find_nearest_enemy(Team& my_team,bool& is_shootable)
{
	Point2D p;

	return p;
}

Node* GameMgr::a_star(Point2D& start, Point2D& target)
{
	return nullptr;
}

///
///adds a team to the teams vector
///
void GameMgr::add_team(const Team& team)
{
	this->teams_.push_back(team);
}