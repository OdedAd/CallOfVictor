#include "GameMgr.h"

#include <iostream>
#include <omp.h>

///
///Initialize the game. call all relevant initialization function and generate the maze
///
void GameMgr::init_game()
{
	is_game_over_ = false;
	fighting_rooms_ = new vector<int>(10);
	generate_maze();
	generate_pickups();
	generate_teams();
	generate_map();
	std::cout << (Utils::compare_maps(teams_[0]->get_map(), teams_[1]->get_map()) ? "Maps are different" : "Maps are the same") << std::endl;
	PlaySound(TEXT("Sounds/AtDoomsGate.wav"), NULL, SND_ASYNC | SND_FILENAME);
}

///
/// return an instance of game manager
///
GameMgr& GameMgr::get_instance()
{
	static GameMgr* instance;
	if (instance == nullptr)
	{
		instance = new GameMgr();
	}
	return *instance;
}

///
///Initialize the maze.
///
void GameMgr::generate_maze()
{
	std::cout << "-----Generating maze-----" << std::endl;
	this->maze_.setup_maze();
	std::cout << "-----Finished generating maze-----\n" << std::endl;
}

///
///Initialize the pickups
///
void GameMgr::generate_pickups()
{
	std::cout << "-----Generating Pickups-----" << std::endl;
	for (auto i = 0; i < num_of_pickups; i++)
	{
		init_pickup(PickupType::ammo, PICKUP_AMMO);
		init_pickup(PickupType::med_kit, PICKUP_MED);
	}
	std::cout << "-----Finished generating Pickups-----\n" << std::endl;
}

/**
 * \brief initialize a pickup based on it's type and color
 * \param type the pickup type to initialize
 * \param color_type the color of the pickup
 */
void GameMgr::init_pickup(const PickupType type, const int color_type)
{
	const auto room = maze_.get_room_at(rand() % num_of_rooms);
	auto* const location = room.get_random_point_in_room();
	const auto pickup_object = PickupObject(location, type);
	pickup_objects_.push_back(pickup_object);
	maze_.get_at_pos(location->get_row(), location->get_col()).set_value(color_type);
}

///
///Initialize the teams.
///
void GameMgr::generate_teams()
{
	std::cout << "-----Generating teams and players-----" << std::endl;
	int running_id = 0;
	int rooms[2] = { 0,maze_.get_num_existing_rooms() - 1 };
	const auto max_num_of_players = 4;

	for (auto i = 0; i < num_of_teams; ++i)
	{
		double color[3] = { i % 2,0,(i + 1) % 2 };
		auto team = new Team(color);
		//add a player in a random location at room 0
		auto team_room = maze_.get_room_at(rooms[i]);
		int k, j;
		int cur_value;

		const auto team_room_left_top_row = team_room.get_left_top().get_row();
		const auto team_room_left_top_col = team_room.get_left_top().get_col();
		const auto team_room_right_bottom_row = team_room.get_right_bottom().get_row();
		const auto team_room_right_bottom_col = team_room.get_right_bottom().get_col();

		for (auto cur_num_of_players = 0; cur_num_of_players < max_num_of_players; cur_num_of_players++)
		{

			do
			{
				k = rand() % (team_room_right_bottom_row - team_room_left_top_row) + team_room_left_top_row;
				j = rand() % (team_room_right_bottom_col - team_room_left_top_col) + team_room_left_top_col;
				cur_value = maze_.get_at_pos(k, j).get_value();

			} while (cur_value != SPACE); //collision prevention

			maze_.get_at_pos(k, j).set_value(PLAYER);

			int player_type = cur_num_of_players;
			//player_type = rand() % 4;

			switch (player_type)
			{
			case SNIPER_TYPE:
			{
				team->add_player(new Sniper(this, ++running_id, team, &maze_.get_at_pos(k, j), (i + 1) * 2));
				break;
			}
			case BERSERKER_TYPE:
			{
				team->add_player(new Berserker(this, ++running_id, team, &maze_.get_at_pos(k, j), (i + 1) * 2));
				break;
			}
			case GRENADIER_TYPE:
			{
				team->add_player(new Grenadier(this, ++running_id, team, &maze_.get_at_pos(k, j), (i + 1) * 2));
				break;
			}
			case SURVIVOR_TYPE:
			{
				team->add_player(new Survivor(this, ++running_id, team, &maze_.get_at_pos(k, j), (i + 1) * 2));
				break;
			}
			default:
			{
				team->add_player(new Player(this, ++running_id, team, &maze_.get_at_pos(k, j), (i + 1) * 2));
				break;
			}
			}

		}
		this->add_team(team);
	}
	std::cout << "-----Finished generating teams and players-----\n" << std::endl;
}

/**
 * \brief get the maze object
 * \return a reference to the maze object
 */
Maze& GameMgr::get_maze()
{
	return this->maze_;
}

/**
 * \brief get all teams playing
 * \return vector with the teams playing
 */
std::vector<Team*>& GameMgr::get_teams()
{
	return this->teams_;
}

/**
 * \brief get all bullets for drawing purpose
 * \return a vector containing all bullets
 */
vector<Bullet*>& GameMgr::get_bullets()
{
	return this->bullets_;
}

/**
 * \brief get all grenades for drawing purpose
 * \return a vector containing all grenades
 */
vector<Grenade*>& GameMgr::get_grenades()
{
	return this->grenades_;
}

///<summary>
/// Find the nearest pickup of the given type
///  relative to the given location using heuristic distance.
/// If there is no such PickupObject , target doesn't change.
///</summary>
void GameMgr::find_nearest_pickup(Point2D& location, Point2D& target, const PickupType type)
{
	Node temp_node;
	temp_node.set_point(location);
	double min_distance = -1;

	for (auto cur_pickup : pickup_objects_)
	{
		if (cur_pickup.get_type() == type && cur_pickup.get_quantity() > 0)
		{
			temp_node.set_target(cur_pickup.get_position());
			const auto current_distance = temp_node.compute_h();
			if (min_distance == -1 || current_distance < min_distance)
			{
				min_distance = current_distance;
				target = *cur_pickup.get_position();
			}
		}
	}
}

///<summary>
/// Find the nearest enemy
///  relative to the given location using heuristic distance.
/// If there is no such enemy , nullptr is returned.
///</summary>
Point2D& GameMgr::find_nearest_enemy(Point2D& location, Team& my_team, bool& is_shootable)
{
	Point2D* p = nullptr;
	Node temp_node;
	temp_node.set_point(location);
	double min_distance = -1;

	for (Team* cur_team : teams_)
	{
		if (!cur_team->compare_color(my_team.get_color()))
		{
			for (Player* cur_player : cur_team->get_teammates())
			{
				if (cur_player->get_hp() > 0)
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
	}
	if (p == nullptr)
	{
		p = new Point2D(-1, -1);
	}
	return *p;
}

/**
 * \brief Implementation of the A* algorithm
 * \param start the starting point for the algorithm
 * \param target the target to search for
 * \param callers_team the calling team
 * \return the target node with all parents
 */
Node* GameMgr::a_star(Point2D& start, Point2D& target, Team* callers_team)
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
			check_neighbors(pn, gray, black, pq, callers_team);
		}
	}

	return nullptr;
}

/**
 * \brief help function to the A* that checks all positions to march to
 * \param pn the current node we are in
 * \param gray the gray vector reference
 * \param black the black vector reference
 * \param pq the priority queue reference
 * \param callers_team the team which invoked the a*
 */
void GameMgr::check_neighbors(Node* pn, std::vector<Node>& gray, std::vector<Node>& black,
	std::priority_queue <Node*, std::vector<Node*>, CompareNodes>& pq, Team* callers_team)
{
	// try down
	if (pn->get_point().get_row() < maze_size - 1)
		check_node(pn->get_point().get_row() + 1, pn->get_point().get_col(), pn, gray, black, pq, callers_team);
	// try up
	if (pn->get_point().get_row() > 0)
		check_node(pn->get_point().get_row() - 1, pn->get_point().get_col(), pn, gray, black, pq, callers_team);
	// try left
	if (pn->get_point().get_col() > 0)
		check_node(pn->get_point().get_row(), pn->get_point().get_col() - 1, pn, gray, black, pq, callers_team);
	// try right
	if (pn->get_point().get_col() < maze_size - 1)
		check_node(pn->get_point().get_row(), pn->get_point().get_col() + 1, pn, gray, black, pq, callers_team);

}

/**
 * \brief an helper function to the check_neighbor function that checks the given node in row and col
 * \param row the row of the checked node
 * \param col the column of the checked node
 * \param pn the current node
 * \param gray the gray vector reference
 * \param black the black vector reference
 * \param pq the priority queue reference
 * \param callers_team the team which invoked the a*
 */
void GameMgr::check_node(const int row, const int col, Node* pn, std::vector<Node>& gray, std::vector<Node>& black,
	std::priority_queue <Node*, std::vector<Node*>, CompareNodes>& pq, Team* callers_team)
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
		else if (cur_node_value == PLAYER)
		{ // player
			if (callers_team != nullptr && get_player_at_pos(pt)->get_team() == callers_team)
				cost = 5000;
			else
				cost = 5;
		}

		cost += callers_team->get_map()[row][col] * 10;
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

/**
 * \brief adds a team to the teams vector
 * \param team the team to add
 */
void GameMgr::add_team(Team* team)
{
	this->teams_.push_back(team);
}

/**
 * \brief throw grenade action
 * \param calling_player the player who throws the grenade
 * \param target the target point of the grenade
 * \return true if player threw a grenade and false otherwise
 */
bool GameMgr::throw_grenade(Player* calling_player, Point2D& target)
{
	Point2D calling_player_point = calling_player->get_location()->get_point();
	double distance = calling_player_point.get_distance(target);

	if (distance < calling_player->get_throw_dis_max() &&
		distance > calling_player->get_throw_dis_min())
	{
		int start_i = target.get_row();
		int start_j = target.get_col();
		PlaySound(TEXT("Sounds/PinDrop.wav"), NULL, SND_ASYNC | SND_FILENAME);
		grenades_.push_back(new Grenade(start_i, start_j, calling_player->get_grenade_dmg(), DEFAULT_FUZE, calling_player->get_team()->get_color()));
		return true;
	}
	return false;
}

/**
 * \brief the shoot action
 * \param calling_player the player who shoots
 * \param target the target point of the bullet
 * \return true if player shot was successful and false otherwise
 */
bool GameMgr::shoot(Player* calling_player, Point2D& target)
{
	Point2D calling_player_point = calling_player->get_location()->get_point();

	int start_i = calling_player_point.get_row();
	int start_j = calling_player_point.get_col();
	double angle = Point2D(start_i, start_j).get_angle_between_two_points(target);
	start_i += (int)round(cos(angle));
	start_j += (int)round(sin(angle));

	if (calling_player_point.get_distance(target) <= 1)
	{
		PlaySound(TEXT("Sounds/Shoot.wav"), NULL, SND_ASYNC | SND_FILENAME);
		bullets_.push_back(new Bullet(start_i, start_j, target, calling_player->get_shooting_dmg()));
		return true;
	}

	try
	{
		Room& targets_room = maze_.get_room_at(target);
		Room& player_room = maze_.get_room_at(calling_player_point);
		if (player_room == targets_room)
		{
			bullets_.push_back(new Bullet(start_i, start_j, target, calling_player->get_shooting_dmg()));
			PlaySound(TEXT("Sounds/Shoot.wav"), NULL, SND_ASYNC | SND_FILENAME);
			return true;
		}
	}
	catch (...) {}; //if we get to the catch it means one of the points was not in a room
					// and because the points are the location of a player and he can only be
					// on a space or a path it means that he is in a PATH and we don't shoot
					// in PATH so he will need to keep going.

	return false;
}

/**
 * \brief the stab action
 * \param calling_player the player who stabs
 * \param target the target point of the stab
 * \return true if player stab was successful and false otherwise
 */
bool GameMgr::stab(Player* calling_player, Point2D& target)
{
	Player* targetPlayer = get_player_at_pos(target);
	Point2D calling_player_point = calling_player->get_location()->get_point();

	if (targetPlayer->get_hp() > 0 && calling_player_point.get_distance(target) <= calling_player->get_stab_dis())
	{
		hit_player(target, calling_player->get_melee_dmg());
		PlaySound(TEXT("Sounds/Stab.wav"), NULL, SND_ASYNC | SND_FILENAME);
		return true;
	}

	return false;
}

/**
 * \brief make the player at the target get hit
 * \param target the target point
 * \param damage the damage inflected
 */
void GameMgr::hit_player(Point2D& target, const int damage)
{
	get_player_at_pos(target)->get_hit(damage);
}

/**
 * \brief collect a pickup action
 * \param calling_player the player who collects the pickup
 * \param target the target point of the pickup
 * \return true if pickup successful and false otherwise
 */
bool GameMgr::pickup(Player* calling_player, Point2D& target)
{
	Node* target_node = &maze_.get_at_pos(target.get_row(), target.get_col());
	int target_value = target_node->get_value();
	int distance = (int)calling_player->get_location()->get_point().get_distance(target);

	if (target_value == PICKUP_AMMO || target_value == PICKUP_MED || distance <= 1)
	{
		PickupObject* targetPickupObject = get_pickup_at_pos(target);
		int quantity = targetPickupObject->get_quantity();

		if (distance <= 1 && quantity > 0)
		{
			targetPickupObject->pickup(); //pickup successful , update the PickupObject.
			if (targetPickupObject->get_quantity() <= 0)
				maze_.get_at_pos(target).set_value(0);

			if (target_value == PICKUP_AMMO)
				PlaySound(TEXT("Sounds/Reload.wav"), NULL, SND_ASYNC | SND_FILENAME);
			else if (target_value == PICKUP_MED)
				PlaySound(TEXT("Sounds/Heal.wav"), NULL, SND_ASYNC | SND_FILENAME);

			return true;
		}
		else
			return false;
	}
	if (target_value == PLAYER || target_value == SPACE)
		return false;
	else
		throw("at GameMgr::pickup , the given target point is not a pickup object");
}

/**
 * \brief generate the heat map for each team
 */
void GameMgr::generate_map()
{
	std::cout << "Generating heat maps:" << std::endl;
	for (auto* team : teams_)
	{
		std::cout << "Generating map for team " << team->get_team_name() << std::endl;
		const int num_tries = 1500;

		int col, row;
		const auto size_factor = 2.0 / maze_size;

		for (auto i = 0; i < num_tries; i++)
		{
			do
			{
				col = rand() % maze_size;
				row = rand() % maze_size;
			} while (maze_.get_at_pos(row, col).get_value() != SPACE);

			double x = col * size_factor - 1;
			double y = row * size_factor - 1;
			Grenade* pg = new Grenade(x, y);
			pg->simulate_explosion(team->get_map(), maze_);
			delete pg;
		}
		std::cout << "Finished generating map for team " << team->get_team_name() << std::endl;
	}
}

/**
 * \brief clears the heat map of all teams in a specific room
 * \param room the room which we need to clear
 */
void GameMgr::clear_room_map(Room& room)
{
	for (auto i = room.get_left_top().get_row(); i < room.get_right_bottom().get_row(); i++)
	{
		for (auto j = room.get_left_top().get_col(); j < room.get_right_bottom().get_col(); j++)
		{
			for (auto team : teams_)
			{
				team->get_map()[i][j] = 0;
			}
		}
	}
}

/**
 * \brief generate the heat map for that specific room
 * \param room the room to generate heat map to
 * \param my_team the team who issued the call
 */
void GameMgr::generate_map_for_room(Room& room, Team* my_team)
{
	const auto num_tries = 15;
	auto other_team = my_team != teams_[0] ? teams_[0] : teams_[1];

	for (auto i = 0; i < num_tries; i++)
	{
		for (auto player : my_team->get_teammates())
		{
			if (player->is_in_room())
			{
				auto player_room = maze_.get_room_at(player->get_location()->get_point());
				if (room.get_left_top() == player_room.get_left_top())
				{
					const auto pt = player->get_location()->get_point();
					auto pg = new Grenade(pt.get_row(), pt.get_col());
					pg->simulate_explosion(other_team->get_map(), maze_);
					delete pg;
				}
			}
		}
	}
}

/**
 * \brief finds a room that contains at least 1 player for each team
 */
void GameMgr::find_rooms_with_fight()
{
	vector<vector<int>*> team_rooms;
	int i = 0;
	while (!fighting_rooms_->empty()) { fighting_rooms_->pop_back(); }
	for (auto* team : teams_)
	{
		team_rooms.push_back(new vector<int>());
		for (auto* player : team->get_teammates())
		{
			if (player->get_hp() > 0 && player->is_in_room())
			{
				team_rooms[i]->push_back(maze_.get_room_at(player->get_location()->get_point()).get_id());
			}
		}
		std::sort(team_rooms[i]->begin(), team_rooms[i]->end());
		i++;
	}

	std::set_intersection(team_rooms[0]->begin(), team_rooms[0]->end(),
		team_rooms[1]->begin(), team_rooms[1]->end(), std::back_inserter(*fighting_rooms_));

	delete team_rooms[0];
	delete team_rooms[1];
}

/**
 * \brief validates if any team lost
 */
void GameMgr::check_if_teams_alive()
{
	for (auto game_team : teams_)
	{
		if (game_team->get_players_alive() <= 0)
		{
			std::cout << "Team " << game_team->get_team_name() << " lost!" << std::endl;
			is_game_over_ = true;
		}
	}
}

/**
 * \brief explodes a grenade
 */
void GameMgr::explode_grenades()
{
	for (auto itr = grenades_.begin(); itr != grenades_.end(); ++itr)
		if ((*itr)->get_is_exploded() == true)
			PlaySound(TEXT("Sounds/Explosion.wav"), NULL, SND_ASYNC | SND_FILENAME);

	grenades_.erase(std::remove_if(grenades_.begin(), grenades_.end(),
		[](const auto g) { return g->get_is_exploded() == true; }),
		grenades_.end());

	for (auto itr = grenades_.begin(); itr != grenades_.end(); ++itr)
	{
		if ((*itr)->get_is_exploded() == false)
		{
			for (auto team : teams_)
			{
				for (auto i = 0; i < 3; ++i)
				{
					auto pg = new Grenade((*itr)->get_row(), (*itr)->get_col());
					pg->simulate_explosion(team->get_map(), maze_);
					delete pg;
				}
			}
			(*itr)->explode(maze_);
		}
	}
}

/**
 * \brief shoots bullets
 */
void GameMgr::shoot_bullets()
{
	bullets_.erase(std::remove_if(bullets_.begin(), bullets_.end(),
		[](const auto b) { return b->get_is_moving() == false; }),
		bullets_.end());

	for (auto itr = bullets_.begin(); itr != bullets_.end(); ++itr)
	{
		if ((*itr)->get_is_moving() == true)
		{
			for (auto team : teams_)
			{
				Bullet* bull = new Bullet((*itr)->get_x(), (*itr)->get_y());
				bull->simulate_motion(team->get_map(), maze_);
				delete bull;
			}
			(*itr)->move(maze_);
		}
	}
}

/**
 * \brief the turn main function.
 */
void GameMgr::play_one_turn()
{
	check_if_teams_alive();

	if (is_game_over_ == false)
	{
		find_rooms_with_fight();
		for (auto cur_team : teams_)
		{
			for (auto fighting_room : *fighting_rooms_)
			{
				clear_room_map(maze_.get_room_at(fighting_room));
				generate_map_for_room(maze_.get_room_at(fighting_room), teams_[0] != cur_team ? cur_team : teams_[1]);
			}
			int index;
#pragma omp parallel for private (index) schedule(dynamic,1)
			for (index = 0; index < cur_team->get_teammates().size(); index++)
			{
				Player* cur_player = cur_team->get_teammates().at(index);
				if (cur_player->get_hp() > 0)
				{
					cur_player->move(maze_);
				}
				else
				{
					cur_player->set_is_moving(false);
				}
			}
		}

		explode_grenades();
		shoot_bullets();

	}

	static int turn_counter = 0;
	std::cout << " Turn " << ++turn_counter << " ended " << std::endl << std::endl;
}

/**
 * \brief checks if game has ended
 * \return true if game is over
 */
bool GameMgr::is_game_over() const
{
	if (is_game_over_ == true)
		PlaySound(TEXT("Sounds/Win.wav"), NULL, SND_ASYNC | SND_FILENAME);

	return this->is_game_over_;
}

/**
 * \brief clears all memory allocations of team
 */
void GameMgr::delete_team_related_allocations()
{
	for (auto game_team : teams_)
	{
		delete game_team;
	}
}

/**
 * \brief clears all gameMgr related allocation
 */
void GameMgr::clear_all_resources()
{
	delete_team_related_allocations();
	delete fighting_rooms_;
}

/**
 * \brief get the safest point in the room
 * \param room the room which is searched
 * \param team the team which issued the search
 * \return a point which represents the safest point in the room
 */
Point2D GameMgr::get_safest_point_in_room(Room& room, Team* team)
{
	return Utils::find_minimum_in_room(maze_, room, team);
}

/**
 * \brief get the safest point in the maze
 * \param team the team which issued the search
 * \return a point which represents the safest point in the maze
 */
Point2D GameMgr::get_safest_point_in_maze(Team* team)
{
	return Utils::find_minimum_in_matrix(maze_, team);
}

/**
 * \brief get a player at a specific location
 * \param position the position that we search
 * \return a player in that position
 */
Player* GameMgr::get_player_at_pos(Point2D& position)
{
	for (auto curTeam : teams_)
		for (Player* curPlayer : curTeam->get_teammates())
			if (curPlayer->get_location()->get_point() == position)
				return curPlayer;

	throw "at GameMgr::get_player_at_pos , the given position is not a player";
}

/**
 * \brief get a pickup at a specific location
 * \param position the position that we search
 * \return a pickup in that position
 */
PickupObject* GameMgr::get_pickup_at_pos(Point2D& position)
{
	for (PickupObject& curPickup : pickup_objects_)
		if (*(curPickup.get_position()) == position)
			return &curPickup;

	throw "at GameMgr::get_pickup_at_pos , the given position is not a PickupObject";
}