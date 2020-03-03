#include "Maze.h"

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

	for(i=0;i<maze_size;i++)
	{
		for (j = 0; j < maze_size ; j++)
		{
				maze_[i][j].SetValue(WALL);
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

	//DigTunnels();
}

Room& Maze::get_room_at(const unsigned short index)
{
	if (index>=num_of_rooms)
	{
		throw "get_room_at: Index out of bounds!";
	}
	return rooms_[index];
}

Room& Maze::generate_room()
{
	Room* pr=nullptr;
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
	int i, j;

	for (i = 0; i < num_of_rooms; i++)
	{
		std::cout << "Path from " << i << std::endl;
		for (j = i + 1; j < num_of_rooms; j++)
		{
			std::cout << " to " << j << std::endl;
			generate_path(rooms_[i].getCenter(), rooms_[j].getCenter());
		}
	}
}

void Maze::generate_path(Point2D start, Point2D target)
{
	std::priority_queue <Node*, std::vector<Node*>, CompareNodes> pq;
	vector<Node> gray;
	vector<Node> black;
	Node *pn;
	bool stop = false;
	vector<Node>::iterator gray_it;
	vector<Node>::iterator black_it;
	double wall_cost = 10;
	double space_cost = 0.2;
	pn = new Node(start, &target, maze[start.getRow()][start.getCol()].GetValue(), 0, nullptr);
	pq.push(pn);
	gray.push_back(*pn);
	while (!pq.empty()&&!stop)
	{
		// take the best node from pq
		pn = pq.top();
			// remove top Node from pq
		pq.pop();
		if (pn->getPoint() == target) // the path has been found
		{
			stop = true;
			// restore path to dig tunnels
			// set SPACE instead of WALL on the path
			while (!(pn->getPoint() == start))
			{
				maze[pn->getPoint().getRow()][pn->getPoint().getCol()].SetValue(SPACE);
				pn = pn->getParent();
			}
			return;
		}
		else // pn is not target
		{
			// remove Node from gray and add it to black
			gray_it = find(gray.begin(), gray.end(), *pn); // operator == must be implemented in Node
			if (gray_it != gray.end())
				gray.erase(gray_it);
			black.push_back(*pn);
			// check the neighbours
			AddNeighbours(pn, gray, black, pq);
		}
	}
}
