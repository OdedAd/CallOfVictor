#pragma once
#include <queue>

#include "CompareNodes.h"
#include "Node.h"
#include "Room.h"

const int maze_size = 100;
const int num_of_rooms = 20;


class Maze
{
private:
	Node maze_[maze_size][maze_size];
	Room rooms_[num_of_rooms];
	int num_existing_rooms_ = 0;

public:
	Node** get_sub_matrix(Point2D,Point2D);
	Node& get_at_pos(int i,int j);
	void setup_maze();
	Room& get_room_at(unsigned short index);
	Room& generate_room();
	void dig_tunnels();
	void generate_path(Point2D start,Point2D target);
	void add_neighbors(Node* pn, std::vector<Node> &gray, std::vector<Node> &black,
                   std::priority_queue <Node*, std::vector<Node*>, CompareNodes> &pq);
	void add_node(int row, int col, Node* pn, std::vector<Node> &gray, std::vector<Node> &black,
             std::priority_queue <Node*, std::vector<Node*>, CompareNodes> &pq);
};

