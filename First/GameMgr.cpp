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
			team->add_player(new Player(this, team, &maze_.get_at_pos(k, j)));
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


///<summary>
/// Find the nearest pickup of the given type
//  reletive to the given location using heuristic distance.
/// If there is no such PickupObject , nullptr is returned.
///</summary>
Point2D& GameMgr::find_nearest_pickup(Point2D& location, PickupType type)
{
	Point2D* p = nullptr;
	Node tempNode;
	tempNode.set_point(location);
	double minDistance = -1;
	double curDistance;

	for (PickupObject curPickup : pickup_objects_)
	{
		if (curPickup.get_type() == type)
		{
			tempNode.set_target(curPickup.get_position());
			curDistance = tempNode.compute_h();
			if (minDistance == -1  || curDistance < minDistance)
			{
				minDistance = curDistance;
				p = curPickup.get_position();
			}
		}
	}


	return *p;
}


///<summary>
/// Find the nearest pickup of the given type
//  reletive to the given location using heuristic distance.
/// If there is no such PickupObject , nullptr is returned.
///</summary>
Point2D& GameMgr::find_nearest_enemy(Point2D& location, Team& my_team,bool& is_shootable)
{
	Point2D* p = nullptr;
	Node tempNode;
	tempNode.set_point(location);
	double minDistance = -1;
	double curDistance;

	for (Team curTeam : teams_)
	{
		if (curTeam != my_team)
		{
			for (Player* curPlayer : curTeam.get_teammates())
			{
				tempNode.set_target(&curPlayer->get_location()->get_point());
				curDistance = tempNode.compute_h();
				if (minDistance == -1 || curDistance < minDistance)
				{
					minDistance = curDistance;
					p = &curPlayer->get_location()->get_point();
				}

			}
		}
	}


	return *p;
}

Node* GameMgr::a_star(Point2D& start, Point2D& target)
{
	std::priority_queue <Node*, std::vector<Node*>, CompareNodes> pq;
	std::vector<Node> gray;
	std::vector<Node> black;
	std::vector<Node>::iterator black_it;

	auto pn = new Node(start, &target, maze_.get_at_pos(start.get_row(), start.get_col()).get_value(), 0, nullptr);
	pq.push(pn);
	gray.push_back(*pn);
	while (!pq.empty())
	{
		pn = pq.top();
		pq.pop();

		if (pn->get_point() == target) // the target has been found
		{
			return pn;
		}
		else // pn is not target
		{
			// remove Node from gray and add it to black
			auto gray_it = find(gray.begin(), gray.end(), *pn); // operator == must be implemented in Node
			if (gray_it != gray.end())
			{
				gray.erase(gray_it);
			}
			black.push_back(*pn);
			// check the neighbors
			check_neighbors(pn, gray, black, pq);
		}
	}

	return nullptr;
}

void GameMgr::check_neighbors(Node* pn, std::vector<Node>& gray, std::vector<Node>& black,
	std::priority_queue <Node*, std::vector<Node*>, CompareNodes>& pq)
{
	// try down
	if (pn->get_point().get_row() < maze_size - 1)
		check_node(pn->get_point().get_row() + 1, pn->get_point().get_col(), pn, gray, black, pq);
	// try up
	if (pn->get_point().get_row() > 0)
		check_node(pn->get_point().get_row() - 1, pn->get_point().get_col(), pn, gray, black, pq);
	// try left
	if (pn->get_point().get_col() > 0)
		check_node(pn->get_point().get_row(), pn->get_point().get_col() - 1, pn, gray, black, pq);
	// try right
	if (pn->get_point().get_col() < maze_size - 1)
		check_node(pn->get_point().get_row(), pn->get_point().get_col() + 1, pn, gray, black, pq);

	//TODO: add diagonal movement support
}

void GameMgr::check_node(const int row, const int col, Node* pn, std::vector<Node>& gray, std::vector<Node>& black,
	std::priority_queue <Node*, std::vector<Node*>, CompareNodes>& pq)
{
	Point2D pt;
	double cost = 0;

	pt.set_row(row);
	pt.set_col(col);
	int curNodeValue = maze_.get_at_pos(row, col).get_value();

	if (curNodeValue != WALL)
	{
		if (curNodeValue == SPACE)
			cost = 0.5; // space cost
		//else if (curNodeValue == PICKUP) // pickup 
		//	cost = 0.5;
		else if (curNodeValue == PLAYER) // player 
			cost = 5;
		const auto pn1 = new Node(pt, pn->get_target(), maze_.get_at_pos(pt.get_row(), pt.get_col()).get_value(), pn->get_g() + cost, pn);

		const auto black_it = find(black.begin(), black.end(), *pn1);
		const auto gray_it = find(gray.begin(), gray.end(), *pn1);
		if (black_it == black.end() && gray_it == gray.end()) // it is not black and not gray!
		{// i.e. it is white
			pq.push(pn1);
			gray.push_back(*pn1);
		}
	}
}


///
///adds a team to the teams vector
///
void GameMgr::add_team(const Team& team)
{
	this->teams_.push_back(team);
}