#pragma once
#include <queue>

#include "CompareNodes.h"
#include "Node.h"
#include "Room.h"
#include "Utils.h"


class Maze
{
private:
	Node maze_[maze_size][maze_size];
	Room rooms_[num_of_rooms];
	int num_existing_rooms_ = 0;

public:
	Node** get_sub_matrix(Point2D,Point2D);
	Node& get_at_pos(int i,int j);
	Node& get_at_pos(Point2D& position);
	void setup_maze();
	Room& get_room_at(unsigned short index);
	Room& get_room_at(const Point2D& point);
	Room& generate_room(int id);
	void dig_tunnels();
	void generate_path(Point2D start,Point2D target);
	void add_neighbors(Node* pn, std::vector<Node> &gray, std::vector<Node> &black,
                   std::priority_queue <Node*, std::vector<Node*>, CompareNodes> &pq) const;
	void add_node(int row, int col, Node* pn, std::vector<Node> &gray, std::vector<Node> &black,
             std::priority_queue <Node*, std::vector<Node*>, CompareNodes> &pq) const;
	int get_num_existing_rooms()const;
};

