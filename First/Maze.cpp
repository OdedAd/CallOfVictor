#include "Maze.h"
#include <iostream>

Node** Maze::get_sub_matrix(Point2D top_left, Point2D bottom_right)
{

	return nullptr;
}

Node& Maze::get_at_pos(const int i, const int j)
{
	return this->maze_[i][j];
}

void Maze::setup_maze()
{
	int i, j;

	for (i = 0; i < maze_size; i++)
	{
		for (j = 0; j < maze_size; j++)
		{
			maze_[i][j].SetValue(WALL);
			maze_[i][j].SetPoint(Point2D(i , j));
		}
	}
	for (num_existing_rooms_ = 0; num_existing_rooms_ < num_of_rooms; num_existing_rooms_++)
	{
		rooms_[num_existing_rooms_] = generate_room();
	}


	for (auto k = 0; k < 30; k++)
	{
		i = rand() % maze_size;
		j = rand() % maze_size;
		maze_[i][j].SetValue(WALL);
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
			if (rooms_[i].CheckOverlapping(pr))
				is_overlapping = true;

		}
	} while (is_overlapping);

	// pr is not overlapping with other rooms
	for (auto i = pr->getLeftTop().getRow(); i <= pr->getRightBottom().getRow(); i++)
	{
		for (auto j = pr->getLeftTop().getCol(); j <= pr->getRightBottom().getCol(); j++)
		{
			maze_[i][j].SetValue(SPACE);
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
			generate_path(rooms_[i].getCenter(), rooms_[j].getCenter());
		}
	}
}

void Maze::generate_path(Point2D start, Point2D target)
{
	std::priority_queue <Node*, std::vector<Node*>, CompareNodes> pq;
	std::vector<Node> gray;
	std::vector<Node> black;
	std::vector<Node>::iterator black_it;
	auto pn = new Node(start, &target, maze_[start.getRow()][start.getCol()].GetValue(), 0, nullptr);
	pq.push(pn);
	gray.push_back(*pn);
	while (!pq.empty())
	{
		pn = pq.top();
		pq.pop();
		
		if (pn->getPoint() == target) // the path has been found
		{
			// restore path to dig tunnels
			// set SPACE instead of WALL on the path
			while (!(pn->getPoint() == start))
			{
				maze_[pn->getPoint().getRow()][pn->getPoint().getCol()].SetValue(SPACE);
				pn = pn->getParent();
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

void Maze::add_neighbors(Node* pn, std::vector<Node> &gray, std::vector<Node> &black,
                   std::priority_queue <Node*, std::vector<Node*>, CompareNodes> &pq)
{
	// try down
	if(pn->getPoint().getRow()<maze_size-1)
		add_node(pn->getPoint().getRow() + 1, pn->getPoint().getCol(), pn, gray, black, pq);
	// try up
	if (pn->getPoint().getRow() >0)
		add_node(pn->getPoint().getRow() - 1, pn->getPoint().getCol(), pn, gray, black, pq);
	// try left
	if (pn->getPoint().getCol() > 0)
		add_node(pn->getPoint().getRow() , pn->getPoint().getCol()- 1, pn, gray, black, pq);
	// try right
	if (pn->getPoint().getCol() <maze_size-1)
		add_node(pn->getPoint().getRow(), pn->getPoint().getCol() + 1, pn, gray, black, pq);
}

void Maze::add_node(const int row, const int col, Node* pn, std::vector<Node> &gray, std::vector<Node> &black,
             std::priority_queue <Node*, std::vector<Node*>, CompareNodes> &pq)
{
	Point2D pt;
	double cost;

	pt.setRow(row);
	pt.setCol(col);
	if (maze_[row][col].GetValue() == SPACE)
		cost = 0.1; // space cost
	else if (maze_[row][col].GetValue() == WALL)
		cost = 3;
	else // player or pickup object or something, we don't want pathes right next to them.
		cost = 5;
	// cost depends on is it a wall or a space
	const auto pn1 = new Node(pt, pn->getTarget(), maze_[pt.getRow()][pt.getCol()].GetValue(), pn->getG() + cost, pn);

	const auto black_it = find(black.begin(), black.end(), *pn1);
	const auto gray_it = find(gray.begin(), gray.end(), *pn1);
	if (black_it == black.end() && gray_it==gray.end()) // it is not black and not gray!
	{// i.e. it is white
		pq.push(pn1);
		gray.push_back(*pn1);
	}
}

int Maze::get_num_existing_rooms() const
{
	return num_existing_rooms_;
}
