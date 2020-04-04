#include "GameMgr.h"

#include <iostream>

///
///Initialize the game. call all relevant initialization function and generate the maze
///
void GameMgr::init_game()
{
	is_game_over_ = false;
	generate_maze();
	generate_teams();
	generate_pickups();
}

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
	this->maze_.setup_maze();
}

///
///Initialize the pickups
///
void GameMgr::generate_pickups()
{
	for (int i = 0; i < num_of_pickups; i++)
	{
		init_pickup(PickupType::ammo, PICKUP_AMMO);
		init_pickup(PickupType::med_kit, PICKUP_MED);
	}
}

void GameMgr::init_pickup(const PickupType type, const int color_type)
{
	const auto room = maze_.get_room_at(rand() % num_of_rooms);
	const auto location = room.get_random_point_in_room();
	const auto pickup_object = PickupObject(location, type);
	pickup_objects_.push_back(pickup_object);
	maze_.get_at_pos(location->get_row(), location->get_col()).set_value(color_type);
}


///
///Initialize the teams.
///
void GameMgr::generate_teams()
{
	int running_id = 0;
	int rooms[2] = { 0,maze_.get_num_existing_rooms() - 1 };
	for (auto i = 0; i < num_of_teams; ++i)
	{
		int color[3] = { i % 2,0,(i + 1) % 2 };
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
			team->add_player(new Player(this, ++running_id, team, &maze_.get_at_pos(k, j), 10, 100));
		}
		this->add_team(team);
	}
}

Maze& GameMgr::get_maze()
{
	return this->maze_;
}

std::vector<Team*>& GameMgr::get_teams()
{
	return this->teams_;
}

//vector<LogicBullet*>& GameMgr::get_bullets()
vector<Bullet*>& GameMgr::get_bullets()
{
	return this->bullets_;
}

vector<Grenade*>& GameMgr::get_grenades()
{
	return this->grenades_;
}


///<summary>
/// Find the nearest pickup of the given type
//  relative to the given location using heuristic distance.
/// If there is no such PickupObject , nullptr is returned.
///</summary>
Point2D& GameMgr::find_nearest_pickup(Point2D& location, const PickupType type)
{
	Point2D* p = nullptr;
	Node temp_node;
	temp_node.set_point(location);
	double min_distance = -1;

	for (PickupObject cur_pickup : pickup_objects_)
	{
		if (cur_pickup.get_type() == type && cur_pickup.get_quantity() > 0)
		{
			temp_node.set_target(cur_pickup.get_position());
			double cur_distance = temp_node.compute_h();
			if (min_distance == -1 || cur_distance < min_distance)
			{
				min_distance = cur_distance;
				p = cur_pickup.get_position();
			}
		}
	}

	return *p;
}


///<summary>
/// Find the nearest enemy
//  relative to the given location using heuristic distance.
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

	//TODO: add diagonal movement support
}

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
		else if (cur_node_value == PLAYER) // player
			if (callers_team != nullptr && get_player_at_pos(pt)->get_team() == callers_team)
				cost = 500;
			else
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
void GameMgr::add_team(Team* team)
{
	this->teams_.push_back(team);
}

bool GameMgr::throw_grenade(Player* calling_player, Point2D& target)
{
	Player* targetPlayer = get_player_at_pos(target);
	Point2D calling_player_point = calling_player->get_location()->get_point();
	int start_i = calling_player_point.get_row();
	int start_j = calling_player_point.get_col();

	int max_damage = targetPlayer->get_max_hp() / 5;

	if (calling_player_point.get_distance(target) < 5)
	{
		//bullets_.push_back(new LogicBullet(calling_player->get_location()->get_point(), target, max_damage));
		grenades_.push_back(new Grenade(start_i, start_j, max_damage));
		return true;
	}
	else
		return false;
}

bool GameMgr::shoot(Player* calling_player, Point2D& target)
{
	Player* targetPlayer = get_player_at_pos(target);
	Point2D calling_player_point = calling_player->get_location()->get_point();
	int start_i = calling_player_point.get_row();
	int start_j = calling_player_point.get_col();

	int max_damage = targetPlayer->get_max_hp() / 2;
	//int damage = max_damage - (int)calling_player->get_location()->get_point().get_distance(target);
	//if (damage < 0) damage = 0;

	if (calling_player_point.get_distance(target) < 2)
	{
		//bullets_.push_back(new LogicBullet(calling_player->get_location()->get_point(), target, max_damage));
		bullets_.push_back(new Bullet(start_i , start_j, target, max_damage));
		return true;
	}

	try
	{
		Room& targets_room = maze_.get_room_at(target);
		Room& player_room = maze_.get_room_at(calling_player_point);
		if (player_room == targets_room)
		{
			//bullets_.push_back(new LogicBullet(calling_player->get_location()->get_point(), target, 2));
			bullets_.push_back(new Bullet(start_i, start_j, target, 2));
			return true;
		}
	}
	catch (...) {}; //if we get to the catch it means one of the points was not in a room
					// and because the points are the location of a player and he can only be
					// on a space or a path it means that he is in a PATH and we don't shoot
					// in PATH so he will need to keep going.

	return false;
}

void GameMgr::hit_player(Point2D& target, const int damage)
{
	//Player* targetPlayer = get_player_at_pos(target);
	//targetPlayer->get_hit(damage);
	get_player_at_pos(target)->get_hit(damage);
}

bool GameMgr::pickup(Player* calling_player, Point2D& target)
{
	Node* target_node = &maze_.get_at_pos(target.get_row(), target.get_col());
	int target_value = target_node->get_value();
	if (target_value == PICKUP_AMMO || target_value == PICKUP_MED)
	{
		int distance = (int)calling_player->get_location()->get_point().get_distance(target);
		PickupObject* targetPickupObject = get_pickup_at_pos(target);
		int quantity = targetPickupObject->get_quantity();

		if (distance == 1 && quantity > 0)
		{
			targetPickupObject->pickup(); //pickup successful , update the PickupObject.
			return true;
		}
		else
			return false;
	}
	else if (target_value == PLAYER)
		return false;
	else
		throw("at GameMgr::pickup , the given target point is not a pickup object");
}

void GameMgr::generate_map()
{
	const int num_tries = 1000;
	int col, row;
	double x, y;
	const auto size_factor = 2.0 / maze_size;

	for (auto i = 0; i < num_tries; i++)
	{
		do
		{
			col = rand() % maze_size;
			row = rand() % maze_size;
		} while (maze_.get_at_pos(row, col).get_value() != SPACE && maze_.get_at_pos(row, col).get_value() != PLAYER);

		x = col * size_factor - 1;
		y = row * size_factor - 1;
		Grenade* pg = new Grenade(x, y);
		pg->simulate_explosion(map_, maze_);
		delete pg;
	}
}

double** GameMgr::get_heat_map()
{
	double** returnedMap = new double* [maze_size];
	for (int i = 0; i < maze_size; i++)
	{
		returnedMap[i] = new double[maze_size];
		for (int j = 0; j < maze_size; j++)
		{
			returnedMap[i][j] = map_[i][j];
		}
	}
	return returnedMap;
}

void GameMgr::play_one_turn()
{
	generate_map();
	for (auto game_team : teams_)
	{
		if (game_team->get_players_alive() <= 0)
		{
			std::cout << "Team " << game_team->get_team_name() << " lost!" << std::endl;
			is_game_over_ = true;
		}
	}

	if (is_game_over_ == false)
	{
		for (auto cur_team : teams_)
		{
			for (Player* cur_player : cur_team->get_teammates())
			{
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

		grenades_.erase(std::remove_if(grenades_.begin(), grenades_.end(),
			[](const auto g) { return g->get_is_exploded() == true; }),
			grenades_.end());

		for (auto itr = grenades_.begin(); itr != grenades_.end(); ++itr)
			if ((*itr)->get_is_exploded() == false)
				(*itr)->explode(maze_);

		bullets_.erase(std::remove_if(bullets_.begin(), bullets_.end(),
			[](const auto b) { return b->get_is_moving() == false; }),
			bullets_.end());

		for (auto itr = bullets_.begin(); itr != bullets_.end(); ++itr)
			if ((*itr)->get_is_moving() == true)
				(*itr)->move(maze_);
		//else
			//bullets_.erase(itr);


		//clear_map();
	}
	clear_map();
}

void GameMgr::clear_map()
{
	for (int i = 0; i < maze_size; ++i)
	{
		for (int j = 0; j < maze_size; ++j)
		{
			map_[i][j] = 0;
		}
	}
}

bool GameMgr::is_game_over() const
{
	return this->is_game_over_;
}

void GameMgr::delete_team_related_allocations()
{
	for (auto game_team : teams_)
	{
		delete game_team;
	}
}

void GameMgr::clear_all_resources()
{
	delete_team_related_allocations();
}

Player* GameMgr::get_player_at_pos(Point2D& position)
{
	for (auto curTeam : teams_)
		for (Player* curPlayer : curTeam->get_teammates())
			if (curPlayer->get_location()->get_point() == position)
				return curPlayer;

	throw "at GameMgr::get_player_at_pos , the given position is not a player";
}

PickupObject* GameMgr::get_pickup_at_pos(Point2D& position)
{
	for (PickupObject& curPickup : pickup_objects_)
		if (*(curPickup.get_position()) == position)
			return &curPickup;

	throw "at GameMgr::get_pickup_at_pos , the given position is not a PickupObject";
}

Player& GameMgr::get_player_at_pos_ref(Point2D& position)
{
	for (auto curTeam : teams_)
		for (Player* curPlayer : curTeam->get_teammates())
			if (curPlayer->get_location()->get_point() == position)
				return *curPlayer;

	throw "at GameMgr::get_player_at_pos , the given position is not a player";
}