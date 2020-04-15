#pragma once

#define COLOR_LENGTH 3

static double DEFAULT_TEAM_COLOR[] = { 0 , 0 , 1 }; //blue
static double DEFAULT_BULLET_COLOR[] = { 0 , 0 , 0 }; //black
static double DEFAULT_GRENADE_COLOR[] = { 0.75 , 0.75 , 0.75 }; //grey

#include "Room.h"

class Team;
class Maze;

static const int maze_size = 100;
static const int num_of_rooms = 20;
static const int num_of_pickups = 5;

class Utils
{
public:
	
	/**
	 * \brief prints the given map
	 * \param map a map to print
	 */
	static void print_map(double** map);

	
	/**
	 * \brief find the safest point in the given maze
	 * \param maze the game maze
	 * \param team the team that is playing now(for map purpose)
	 * \return a point2d object that represents the safest point
	 */
	static Point2D& find_minimum_in_matrix(Maze& maze, Team* team);
	
	/**
	 * \brief find the safest point in the give room
	 * \param maze the game's maze
	 * \param room the game room that we are in
	 * \param team the team that is playing now(for map purpose)
	 * \return a point2d object that represents the safest point
	 */
	static Point2D& find_minimum_in_room(Maze& maze, Room& room, Team* team);
	
	/**
	 * \brief find the most dangerous point in the given maze
	 * \param maze the game's maze
	 * \return a point2d object that represents the most dangerous point
	 */
	static Point2D& find_maximum_in_matrix(Maze& maze);
	
	/**
	 * \brief clears the temporary maze allocations
	 * \param matrix the maze to clear
	 * \param size a size of the maze
	 */
	static void clear_temporary_maze(Node** matrix, const int size);
	
	/**
	 * \brief compare the given maps to check generation randomness
	 * \param team_one_map the first team's map
	 * \param team_two_map the second team's map
	 * \return true if maps are equal and false otherwise
	 */
	static bool compare_maps(double** team_one_map, double** team_two_map);


private:


};

