#include "Maze.h"
#include <iostream>

/**
 * \brief get a sub matrix
 * \param top_left the top left point of room
 * \param bottom_right the bottom right point of the room
 * \return sub matrix of nodes from left top to bottom right
 */
Node** Maze::get_sub_matrix(const Point2D top_left, const Point2D bottom_right)
{
	const auto rows = bottom_right.get_row() - top_left.get_row();
	const auto cols = bottom_right.get_col() - top_left.get_col();
	auto sub_maze = new Node * [rows];
	for (auto i = 0; i < rows; ++i)
	{
		sub_maze[i] = new Node[cols];
	}

	for (int i = top_left.get_row(); i < bottom_right.get_row(); ++i)
	{
		for (int j = top_left.get_col(); j < bottom_right.get_col(); ++j)
		{
			sub_maze[i - top_left.get_row()][j - top_left.get_col()] = maze_[i][j];
		}
	}

	return sub_maze;
}

Node& Maze::get_at_pos(const int i, const int j)
{
	if (i >= maze_size || i < 0 || j >= maze_size || j < 0)
	{
		throw std::runtime_error("Invalid coordinates");
	}
	return this->maze_[i][j];
}

Node& Maze::get_at_pos(Point2D& position)
{
	return this->maze_[position.get_row()][position.get_col()];
}

void Maze::setup_maze()
{
	int i, j;

	for (i = 0; i < maze_size; i++)
	{
		for (j = 0; j < maze_size; j++)
		{
			maze_[i][j].set_value(WALL);
			maze_[i][j].set_point(Point2D(i, j));
		}
	}
	for (num_existing_rooms_ = 0; num_existing_rooms_ < num_of_rooms; num_existing_rooms_++)
	{
		rooms_[num_existing_rooms_] = generate_room();
	}

	for (auto k = 0; k < 80; k++)
	{
		i = rand() % maze_size;
		j = rand() % maze_size;
		maze_[i][j].set_value(WALL);
	}

	dig_tunnels();
}

Room& Maze::get_room_at(const unsigned short index)
{
	if (index >= num_of_rooms)
	{
		throw "get_room_at: Index out of bounds!";
	}
	return rooms_[index];
}

Room& Maze::get_room_at(const Point2D& point)
{
	int target_room_index = 0;
	bool is_found = false;

	if (point.get_row() >= maze_size || point.get_col() >= maze_size)
	{
		throw std::invalid_argument("get_room_at: Point out of bounds!");
	}

	for (int cur_room_index = 0; (cur_room_index < num_existing_rooms_) && (is_found == false); ++cur_room_index)
	{
		if ((rooms_[cur_room_index].get_left_top().get_row() - 1 <= point.get_row() && point.get_row() <= rooms_[cur_room_index].get_right_bottom().get_row() + 1)
			&& (rooms_[cur_room_index].get_left_top().get_col() - 1 <= point.get_col() && point.get_col() <= rooms_[cur_room_index].get_right_bottom().get_col() + 1))
		{
			target_room_index = cur_room_index;
			is_found = true;
		}
	}

	if (is_found)
		return rooms_[target_room_index];
	
	throw std::runtime_error("get_room_at: Point not in room!");
}

Room& Maze::generate_room()
{
	Room* pr = nullptr;
	bool is_overlapping;

	do
	{
		delete pr;
		is_overlapping = false;
		const auto width = 6 + rand() % 10;
		const auto height = 6 + rand() % 10;
		const auto center_i = height / 2 + rand() % (maze_size - height);
		const auto center_j = width / 2 + rand() % (maze_size - width);

		pr = new Room(center_i, center_j, width, height);
		for (auto i = 0; i < num_existing_rooms_ && !is_overlapping; i++)
		{
			if (rooms_[i].check_overlapping(pr))
				is_overlapping = true;

		}
	} while (is_overlapping);
	// pr is not overlapping with other rooms
	for (auto i = pr->get_left_top().get_row(); i <= pr->get_right_bottom().get_row(); i++)
	{
		for (auto j = pr->get_left_top().get_col(); j <= pr->get_right_bottom().get_col(); j++)
		{
			maze_[i][j].set_value(SPACE);
		}
	}
	return *pr;
}

void Maze::dig_tunnels()
{
	for (auto i = 0; i < num_of_rooms; i++)
	{
		std::cout << "Path from " << i << std::endl;
		for (auto j = i + 1; j < num_of_rooms; j++)
		{
			std::cout << " to " << j << std::endl;
			generate_path(rooms_[i].get_center(), rooms_[j].get_center());
		}
	}
}

void Maze::generate_path(Point2D start, Point2D target)
{
	std::priority_queue <Node*, std::vector<Node*>, CompareNodes> pq;
	std::vector<Node> gray;
	std::vector<Node> black;
	std::vector<Node>::iterator black_it;
	auto pn = new Node(start, &target, maze_[start.get_row()][start.get_col()].get_value(), 0, nullptr);
	pq.push(pn);
	gray.push_back(*pn);
	while (!pq.empty())
	{
		pn = pq.top();
		pq.pop();

		if (pn->get_point() == target) // the path has been found
		{
			// restore path to dig tunnels
			// set SPACE instead of WALL on the path
			while (!(pn->get_point() == start))
			{
				maze_[pn->get_point().get_row()][pn->get_point().get_col()].set_value(SPACE);
				pn = pn->get_parent();
			}
			return;
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
			add_neighbors(pn, gray, black, pq);
		}
	}
}

void Maze::add_neighbors(Node* pn, std::vector<Node>& gray, std::vector<Node>& black,
	std::priority_queue <Node*, std::vector<Node*>, CompareNodes>& pq) const
{
	// try down
	if (pn->get_point().get_row() < maze_size - 1)
		add_node(pn->get_point().get_row() + 1, pn->get_point().get_col(), pn, gray, black, pq);
	// try up
	if (pn->get_point().get_row() > 0)
		add_node(pn->get_point().get_row() - 1, pn->get_point().get_col(), pn, gray, black, pq);
	// try left
	if (pn->get_point().get_col() > 0)
		add_node(pn->get_point().get_row(), pn->get_point().get_col() - 1, pn, gray, black, pq);
	// try right
	if (pn->get_point().get_col() < maze_size - 1)
		add_node(pn->get_point().get_row(), pn->get_point().get_col() + 1, pn, gray, black, pq);
}

void Maze::add_node(const int row, const int col, Node* pn, std::vector<Node>& gray, std::vector<Node>& black,
	std::priority_queue <Node*, std::vector<Node*>, CompareNodes>& pq) const
{
	Point2D pt;
	double cost;

	pt.set_row(row);
	pt.set_col(col);
	if (maze_[row][col].get_value() == SPACE)
		cost = 0.1; // space cost
	else if (maze_[row][col].get_value() == WALL)
		cost = 3;
	else // player or pickup object or something, we don't want pathes right next to them.
		cost = 5;
	// cost depends on is it a wall or a space
	const auto pn1 = new Node(pt, pn->get_target(), maze_[pt.get_row()][pt.get_col()].get_value(), pn->get_g() + cost, pn);

	const auto black_it = find(black.begin(), black.end(), *pn1);
	const auto gray_it = find(gray.begin(), gray.end(), *pn1);
	if (black_it == black.end() && gray_it == gray.end()) // it is not black and not gray!
	{// i.e. it is white
		pq.push(pn1);
		gray.push_back(*pn1);
	}
}

int Maze::get_num_existing_rooms() const
{
	return num_existing_rooms_;
}
