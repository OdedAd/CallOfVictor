#include <iostream>
#include <time.h>
#include "GLUT.h"
#include <windows.h> 
#include "GameMgr.h"

using namespace std;

const int width = 600; // window width
const int height = 600; // window height
bool is_show_room_map_calculated = false;

Grenade* pg;
bool move_on = false;

void init()
{
	srand(time(nullptr)); // pseudo randomization
	glClearColor(0.7, 0.7, 0.7, 0);
	glOrtho(-1, 1, -1, 1, -1, 1);

	GameMgr::get_instance().init_game();
}

void draw_maze()
{
	const auto size_factor = 2.0 / maze_size;

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
				glColor3d(0.2, 0.2, 0.2); // orange
				break;
			case PICKUP_MED:
				glColor3d(0, 1, 0); // green
				break;
			default: break;
			}

			const auto x = j * size_factor - 1;
			const auto y = i * size_factor - 1;

			glBegin(GL_POLYGON);
			glVertex2d(x, y);
			glVertex2d(x + size_factor, y);
			glVertex2d(x + size_factor, y + size_factor);
			glVertex2d(x, y + size_factor);

			glEnd();
		}
}

void draw_map()
{
	double sz = sz = 2.0 / maze_size, xx, yy;
	if (!is_show_room_map_calculated)
	{
		is_show_room_map_calculated = true;
		GameMgr::get_instance().generate_map_for_room(GameMgr::get_instance().get_maze().get_room_at(0),
			GameMgr::get_instance().get_teams()[0]);
	}
	double** map = GameMgr::get_instance().get_teams()[1]->get_map();
	auto maze = GameMgr::get_instance().get_maze();

	for (auto i = 0; i < maze_size; i++)
		for (auto j = 0; j < maze_size; j++)
		{
			if (maze.get_at_pos(i, j).get_value() == SPACE)
			{
				double c = 1 - map[i][j];// 1(white) is very safe, 0(black) is very dangerous
				//double c = 1 - map[i][j];// 1(white) is very safe, 0(black) is very dangerous
				glColor3d(c, c, c);
				// draw rectangle
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
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer
	draw_maze();

	if (pg != nullptr)
	{
		pg->show_me();
	}

	for (auto cur_team : GameMgr::get_instance().get_teams())
	{
		for (Player* cur_player : cur_team->get_teammates())
		{
			if (cur_player->get_hp() > 0)
			{
				cur_player->show_me();
			}
		}
	}

	for (auto cur_bullet : GameMgr::get_instance().get_bullets())
		cur_bullet->show_me();

	for (auto cur_grenade : GameMgr::get_instance().get_grenades())
		cur_grenade->show_me();

	glutSwapBuffers();// show what was drawn in "frame buffer"
}

void display_map()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer

	draw_maze();
	draw_map();

	glutSwapBuffers();// show what was drawn in "frame buffer"
}

void idle()
{
	Sleep(50);
	if (move_on && !GameMgr::get_instance().is_game_over())
	{
		GameMgr::get_instance().play_one_turn();
	}

	if (GameMgr::get_instance().is_game_over())
	{
		cout << "Game Over!" << endl;
		GameMgr::get_instance().clear_all_resources();
		system("pause");
		exit(0);
	}
	glutPostRedisplay();// calls indirectly to display
}

void menu(const int choice)
{
	switch (choice)
	{
	case 1:
		move_on = false;
		GameMgr::get_instance().generate_map();
		glutDisplayFunc(display_map);
		break;
	case 2:
		glutDisplayFunc(display);
		for (auto cur_team : GameMgr::get_instance().get_teams())
			for (Player* cur_player : cur_team->get_teammates())
				cur_player->set_is_moving(true);

		move_on = true;
		break;

	case 3:
		for (auto* cur_team : GameMgr::get_instance().get_teams())
			for (auto* cur_player : cur_team->get_teammates())
			{
				cur_player->set_hp(1);
				cur_player->set_ammo(0);
				cur_player->set_is_moving(true);
			}
		move_on = true;
		break;

	case 4:
		for (auto cur_team : GameMgr::get_instance().get_teams())
			for (Player* cur_player : cur_team->get_teammates())
			{
				cur_player->set_hp(cur_player->get_max_hp());
				cur_player->set_ammo(0);
				cur_player->set_is_moving(true);
			}
		move_on = true;
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

		cout << "player moving" << endl;
		GameMgr::get_instance().get_teams()[0]->get_teammates()[0]->set_is_moving(true);
		GameMgr::get_instance().get_teams()[0]->get_teammates()[0]->move(GameMgr::get_instance().get_maze());
		GameMgr::get_instance().get_teams()[0]->get_teammates()[0]->set_is_moving(false);
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
	glutCreateWindow("Dungeon");

	glutDisplayFunc(display); // refresh function
	glutIdleFunc(idle); // idle: when nothing happens
	glutMouseFunc(mouse);
	// menu
	glutCreateMenu(menu);
	glutAddMenuEntry("Generate map", 1);
	glutAddMenuEntry("Start Game", 2);
	glutAddMenuEntry("Check Heal", 3);
	glutAddMenuEntry("Check Reload", 4);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	init();

	glutMainLoop();
}