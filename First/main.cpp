#include <iostream>
#include <queue>
#include <time.h>
#include <vector>
#include "Bullet.h"
#include "GLUT.h"
#include "Grenade.h"
#include "Node.h"

#include "Team.h"
#include "Player.h"
#include <windows.h> 


#include "GameMgr.h"
#include "Maze.h"


using namespace std;

const int width = 600; // window width
const int height = 600; // window height

//double map[maze_size][maze_size] = { 0 };
//double** map;

Bullet* pb = nullptr;
Grenade* pg;
bool move_on = false;

void init()
{
	srand(time(nullptr)); // pseudo randomization

	glClearColor(0.7, 0.7, 0.7, 0);
	
	GameMgr::get_instance().init_game();

	glOrtho(-1, 1, -1, 1, -1, 1);


}

void draw_maze()
{
	double sz, x, y;

	for (auto i = 0; i < maze_size; i++)
		for (auto j = 0; j < maze_size; j++)
		{
			// set color
			switch (GameMgr::get_instance().get_maze().get_at_pos(i, j).get_value())
			{
			case SPACE:
				glColor3d(1, 1, 1); // white
				break;
			case WALL:
				glColor3d(0.4, 0, 0); // dark red
				break;
			case START:
				glColor3d(0.4, 0.8, 1); // light blue
				break;
			case TARGET:
				glColor3d(1, 0, 0); // red
				break;
			case PICKUP_AMMO:
				glColor3d(1, 0.64, 0); // orange
				break;
			case PICKUP_MED:
				glColor3d(0.64, 0, 1); // orange
				break;
			}

			// draw rectangle
			sz = 2.0 / maze_size;
			x = j * sz - 1;
			y = i * sz - 1;

			glBegin(GL_POLYGON);
			glVertex2d(x, y);
			glVertex2d(x + sz, y);
			glVertex2d(x + sz, y + sz);
			glVertex2d(x, y + sz);

			glEnd();
		}
}

void draw_map()
{
	double sz, xx, yy;
	double** map = GameMgr::get_instance().get_heat_map();

	for (auto i = 0; i < maze_size; i++)
		for (auto j = 0; j < maze_size; j++)
		{
			if (GameMgr::get_instance().get_maze().get_at_pos(i, j).get_value() == SPACE)
			{
				double c;
				//c = 1 - map[i][j];// 1(white) is very safe, 0(black) is very dangerous
				c = 1 - map[i][j];// 1(white) is very safe, 0(black) is very dangerous
				glColor3d(c, c, c);
				// draw rectangle
				sz = 2.0 / maze_size;
				xx = (j * sz - 1);
				yy = i * sz - 1;

				glBegin(GL_POLYGON);
				glVertex2d(xx, yy);
				glVertex2d(xx + sz, yy);
				glVertex2d(xx + sz, yy + sz);
				glVertex2d(xx, yy + sz);

				glEnd();
			}
		}
	delete map;
}
//void generate_map()
//{
//	const int num_tries = 1000;
//	int col, row;
//	double x, y, sz;
//	Grenade* pg = nullptr;
//
//	for (auto i = 0; i < num_tries; i++)
//	{
//		do
//		{
//			col = rand() % maze_size;
//			row = rand() % maze_size;
//		} while (GameMgr::get_instance().get_maze().get_at_pos(row, col).get_value() != SPACE);
//		sz = 2.0 / maze_size;
//		x = col * sz - 1;
//		y = row * sz - 1;
//		pg = new Grenade(x, y);
//		pg->simulate_explosion(map, GameMgr::get_instance().get_maze());
//		delete pg;
//	}
//}


void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer

	draw_maze();

	if (pg != nullptr)
	{
		//	pb->showMe();
		pg->show_me();
	}

	for (Team cur_team : GameMgr::get_instance().get_teams())
	{
		for (Player* cur_player : cur_team.get_teammates())
		{
			cur_player->show_me();
		}
	}

	glutSwapBuffers();// show what was drawn in "frame buffer"
}

void display_map()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer

	draw_maze();
	draw_map();

	glutSwapBuffers();// show what was drawn in "frame buffer"
}

// checks if dx,dy is on SPACE in maze
bool check_is_space(const double dx, const double dy)
{
	int i = maze_size * (dy + 1) / 2;
	int j = maze_size * (dx + 1) / 2;
	return  GameMgr::get_instance().get_maze().get_at_pos(i, j).get_value() == SPACE;
}

void idle()
{

	//Sleep(100);

	if (move_on)
	{
		if (pg != nullptr)
		{
			//		pb->SetIsMoving(CheckIsSpace(pb->getX(),pb->getY()));
			//		pb->move();
			pg->move_bullets(GameMgr::get_instance().get_maze());

			//		move_on = pg->GetIsMoving();
		}

		for (Team cur_team : GameMgr::get_instance().get_teams())
		{
			for (Player* cur_player : cur_team.get_teammates())
			{
				cur_player->move(GameMgr::get_instance().get_maze());
			}
		}
	}
	glutPostRedisplay();// calls indirectly to display
}

void menu(const int choice)
{
	switch(choice)
	{
	case 1:
		move_on = false;
		//generate_map();
		//map = game_mgr.get_maze().get_room_at(0).get_room_maze();
		GameMgr::get_instance().generate_map();
		glutDisplayFunc(display_map);
		break;
	case 2:
		glutDisplayFunc(display);
		//		pb->SetIsMoving(true);
		if (pg != nullptr)
		{
			pg->explode();
		}

		for (Team cur_team : GameMgr::get_instance().get_teams())
		{
			for (Player* cur_player : cur_team.get_teammates())
			{
				cur_player->set_is_moving(true);
			}
		}

		move_on = true;
		break;
	case 3:
		GameMgr::get_instance().get_teams()[0].get_teammates()[0]->set_hp(2);
		break;
	case 4:
		GameMgr::get_instance().get_teams()[0].get_teammates()[0]->set_ammo(0);
		GameMgr::get_instance().get_teams()[0].get_teammates()[0]->set_hp(4);
		break;
	case 5:
		GameMgr::get_instance().get_teams()[0].get_teammates()[0]->set_hp(4);
		break;
	}
}

void mouse(const int button, const int state, const int x, const int y)
{
	double xx, yy;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		xx = 2 * (double)x / width - 1;
		yy = 2 * ((double)height - y) / height - 1;

		//		pb = new Bullet(xx,yy);
		pg = new Grenade(xx, yy);
		cout << "player moving" << endl;
		GameMgr::get_instance().get_teams()[0].get_teammates()[0]->set_is_moving(true);
		GameMgr::get_instance().get_teams()[0].get_teammates()[0]->move(GameMgr::get_instance().get_maze());
		GameMgr::get_instance().get_teams()[0].get_teammates()[0]->set_is_moving(false);
		cout << "end of player moving" << endl;
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		//		pb->SetIsMoving(true);
		//		pg->explode();
		//		move_on = true;
	}
}
void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("Dungeon ");

	glutDisplayFunc(display); // refresh function
	glutIdleFunc(idle); // idle: when nothing happens
	glutMouseFunc(mouse);
	// menu
	glutCreateMenu(menu);
	glutAddMenuEntry("Generate map", 1);
	glutAddMenuEntry("Explode", 2);
	glutAddMenuEntry("Lower HP", 3);
	glutAddMenuEntry("Lower AMMO", 4);
	glutAddMenuEntry("Check Runaway", 5);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	init();

	glutMainLoop();
}