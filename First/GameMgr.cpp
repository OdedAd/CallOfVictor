#include "GameMgr.h"

///
///Initialize the game. call all relevant initialization function and generate the maze
///
void GameMgr::init_game()
{
	generate_maze();
	generate_teams();
	generate_pickups();
}

///
///Initialize the maze.
///
void GameMgr::generate_maze()
{
	this->maze_.setup_maze();
}

///
///Initialize the pickups
///
void GameMgr::generate_pickups()
{
	for (int i = 0; i < num_of_pickups; i++)
	{
		init_pickup(PickupType::ammo,PICKUP_AMMO);
		init_pickup(PickupType::med_kit,PICKUP_MED);
	}
}

void GameMgr::init_pickup(const PickupType type, const int color_type)
{
	const auto room = maze_.get_room_at(rand()%num_of_rooms);
	const auto location = room.get_random_point_in_room();
	const auto pickup_object = PickupObject(location,type);
	pickup_objects_.push_back(pickup_object);
	maze_.get_at_pos(location->get_row(),location->get_col()).set_value(color_type);
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
//  relative to the given location using heuristic distance.
/// If there is no such PickupObject , nullptr is returned.
///</summary>
Point2D& GameMgr::find_nearest_pickup(Point2D& location, PickupType type)
{
	Point2D* p = nullptr;
	Node temp_node;
	temp_node.set_point(location);
	double min_distance = -1;

	for (PickupObject cur_pickup : pickup_objects_)
	{
		if (cur_pickup.get_type() == type)
		{
			temp_node.set_target(cur_pickup.get_position());
			double cur_distance = temp_node.compute_h();
			if (min_distance == -1  || cur_distance < min_distance)
			{
				min_distance = cur_distance;
				p = cur_pickup.get_position();
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
	Node temp_node;
	temp_node.set_point(location);
	double min_distance = -1;

	for (Team cur_team : teams_)
	{
		if (!cur_team.compare_color(my_team.get_color()))
		{
			for (Player* cur_player : cur_team.get_teammates())
			{
				temp_node.set_target(&cur_player->get_location()->get_point());
				const double cur_distance = temp_node.compute_h();
				if (min_distance == -1 || cur_distance < min_distance)
				{
					min_distance = cur_distance;
					p = &cur_player->get_location()->get_point();
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
	int cur_node_value = maze_.get_at_pos(row, col).get_value();

	if (cur_node_value != WALL)
	{
		if (cur_node_value == SPACE || cur_node_value == PATH)
			cost = 0.5; // space or path cost
		//else if (curNodeValue == PICKUP) // pickup 
		//	cost = 0.1;
		else if (cur_node_value == PLAYER) // player 
			cost = 2;

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