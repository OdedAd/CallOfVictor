#include <iostream>
#include <queue>
#include <time.h>
#include <vector>
#include "Bullet.h"
#include "GLUT.h"
#include "Granade.h"
#include "Node.h"

#include "Team.h"
#include "Player.h"
#include <windows.h> 


#include "GameMgr.h"
#include "Maze.h"


using namespace std;

GameMgr game_mgr;

const int W = 600; // window width
const int H = 600; // window height

const int NUM_ROOMS = 20;

bool run_bfs = false;

Node maze[MSZ][MSZ];
double map[MSZ][MSZ] = { 0 };

Bullet* pb = nullptr;
Granade* pg;
bool move_on = false;

void init()
{
	srand(time(nullptr)); // pseudo randomization

	glClearColor(0.7, 0.7, 0.7, 0);

	game_mgr.init_game();

	glOrtho(-1, 1, -1, 1, -1, 1);

}

void draw_maze()
{
	double sz, x, y;

	for(auto i=0;i<MSZ;i++)
		for (auto j = 0; j < MSZ; j++)
		{
			// set color
			switch (game_mgr.get_maze().get_at_pos(i,j).GetValue())
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
			case PLAYER:
				//glColor3d(1, 0, 0); // red
				break;
			}
			// draw rectangle
			sz = 2.0 / MSZ;
			x = j * sz -1;
			y = i * sz -1;

			glBegin(GL_POLYGON);
				glVertex2d(x, y);
				glVertex2d(x+sz, y);
				glVertex2d(x + sz, y+sz);
				glVertex2d(x, y + sz);

			glEnd();
		}
}

void DrawMap()
{
	int i, j;
	double sz, xx, yy;

	for (i = 0; i < MSZ; i++)
		for (j = 0; j < MSZ; j++)
		{
			if (maze[i][j].GetValue() == SPACE)
			{
				double c;
				c = 1 - map[i][j];// 1(white) is very safe, 0(black) is very dangerous
				glColor3d(c, c, c);
				// draw rectangle
				sz = 2.0 / MSZ;
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
void GenerateMap()
{
	int num_tries = 1000;
	int i;
	int col, row;
	double x, y,sz;
	Granade* pg = nullptr;

	for (i = 0; i < num_tries; i++)
	{
		do
		{
			col = rand() % MSZ;
			row = rand() % MSZ;
		} while (maze[row][col].GetValue() != SPACE);
		sz = 2.0 / MSZ;
		x = col * sz - 1;
		y = row * sz - 1;
		pg = new Granade(x, y);
		pg->SimulateExplosion(map,maze);
	}
}


void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer

	draw_maze();

	if (pg != NULL)
	{
	//	pb->showMe();
		pg->showMe();
	}

	for (Team curTeam : game_mgr.get_teams())
	{
		for (Player* curPlayer : curTeam.GetTeammates())
		{
			curPlayer->showMe();
		}
	}

	glutSwapBuffers();// show what was drawn in "frame buffer"
}

void displayMap()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer

	draw_maze();
	DrawMap();

	glutSwapBuffers();// show what was drawn in "frame buffer"
}

// checks if dx,dy is on SPACE in maze
bool CheckIsSpace(double dx, double dy)
{
	int i, j;

	i = MSZ * (dy + 1) / 2;
	j = MSZ * (dx + 1) / 2;
	return  maze[i][j].GetValue() == SPACE;
}

void idle()
{
	
	Sleep(100);

	if (move_on)
	{
		if (pg != NULL)
		{
			//		pb->SetIsMoving(CheckIsSpace(pb->getX(),pb->getY()));
			//		pb->move();
			pg->moveBullets(game_mgr.get_maze());

			//		move_on = pg->GetIsMoving();
		}

		for (Team curTeam : game_mgr.get_teams())
		{
			for (Player* curPlayer : curTeam.GetTeammates())
			{
				curPlayer->move(game_mgr.get_maze());
			}
		}
	}
	glutPostRedisplay();// calls indirectly to display
}

void Menu(int choice)
{
	if (choice == 1) // generate security map
	{
		move_on = false;
		GenerateMap();
		glutDisplayFunc(displayMap);
	}
	else if (choice == 2) // generate security map
	{
		glutDisplayFunc(display);
//		pb->SetIsMoving(true);
		if(pg != NULL)
		{
			pg->explode();
		}

		for (Team curTeam : game_mgr.get_teams())
		{
			for (Player* curPlayer : curTeam.GetTeammates())
			{
				curPlayer->SetIsMoving(true);
			}
		}

		move_on = true;
	}
}

void mouse(int button, int state, int x, int y)
{
	double xx, yy;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		xx = 2 * (double)x / W - 1;
		yy = 2 * ((double)H - y) / H - 1;

//		pb = new Bullet(xx,yy);
		pg = new Granade(xx, yy);
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
	glutInitWindowSize(W, H);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("Dungeon ");

	glutDisplayFunc(display); // refresh function
	glutIdleFunc(idle); // idle: when nothing happens
	glutMouseFunc(mouse);
	// menu
	glutCreateMenu(Menu);
	glutAddMenuEntry("Generate map", 1);
	glutAddMenuEntry("Explode", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	init();

	glutMainLoop();
}