#include "GameMgr.h"

void GameMgr::init_game()
{
	generate_maze();
	generate_teams();
}

void GameMgr::generate_maze()
{
	this->maze_ = new Maze();
	this->maze_->setup_maze();
}

void GameMgr::generate_teams()
{
	for (auto i = 0; i < num_of_teams; ++i)
	{
		auto team = new Team(i);
		//add a player in a random location at room 0
		auto team_room = maze_->get_room_at(0);
		int k, j;
		const auto max_num_of_players = 5;

		for (auto cur_num_of_players = 0; cur_num_of_players < max_num_of_players; cur_num_of_players++)
		{
			const auto team_room_left_top_row = team_room.getLeftTop().getRow();
			const auto team_room_left_top_col = team_room.getLeftTop().getCol();
			const auto team_room_right_bottom_row = team_room.getRightBottom().getRow();
			const auto team_room_right_bottom_col = team_room.getRightBottom().getCol();

			do
			{
				k = rand() % (team_room_right_bottom_row - team_room_left_top_row) + team_room_left_top_row;
				j = rand() % (team_room_right_bottom_col - team_room_left_top_col) + team_room_left_top_col;
			} while (maze_->get_at_pos(k, j).GetValue() == PLAYER); //collision prevention

			maze_->get_at_pos(k, j).SetValue(PLAYER);
			team->AddPlayer(new Player(team, &maze_->get_at_pos(k, j)));
		}
		this->add_team(*team);
	}
}

void GameMgr::add_team(const Team& team)
{
	this->teams_.push_back(team);
}