#include <iostream>
#include <queue>
#include <time.h>
#include <vector>
#include "Bullet.h"
#include "CompareNodes.h"
#include "GLUT.h"
#include "Granade.h"
#include "Node.h"
#include "Room.h"

#include "Team.h"
#include "Player.h"
#include <windows.h> 


using namespace std;

const int W = 600; // window width
const int H = 600; // window height

const int NUM_ROOMS = 20;

bool run_bfs = false;

Node maze[MSZ][MSZ];
double map[MSZ][MSZ] = { 0 };
Room rooms[NUM_ROOMS];
int num_existing_rooms = 0;

Bullet* pb = nullptr;
Granade* pg;
bool move_on = false;

vector <Node> gray; // gray nodes

vector <Team*> teams;

void setup_maze();
void add_teams();


void Init()
{
	srand(time(nullptr)); // pseudo randomization

	glClearColor(0.7, 0.7, 0.7, 0);

	setup_maze();

	glOrtho(-1, 1, -1, 1, -1, 1);

}

Room GenerateRoom()
{
	int width, h,ci,cj;
	Room* pr=nullptr;
	bool isOveralaping;

	do
	{
		delete pr;
		isOveralaping = false;
		width = 6 + rand() % 10;
		h = 6 + rand() % 10;

		ci = h / 2 + rand() % (MSZ - h);
		cj = width / 2 + rand() % (MSZ - width);

		pr = new Room(ci, cj, width, h);
		for (auto i = 0; i < num_existing_rooms && !isOveralaping; i++)
		{
			if (rooms[i].CheckOverlapping(pr))
				isOveralaping = true;
			
		}
	} while (isOveralaping);

	// pr is not overlapping with other rooms
	for (auto i = pr->getLeftTop().getRow(); i <= pr->getRightBottom().getRow(); i++)
		for (auto j = pr->getLeftTop().getCol(); j <= pr->getRightBottom().getCol(); j++)
			maze[i][j].SetValue(SPACE);
	return *pr;
}

// check if the node at row,col is white or gray that is better then the previous one
// and if so add it to pq
void AddNode(int row, int col, Node* pn, vector<Node> &gray, vector<Node> &black,
	priority_queue <Node*, vector<Node*>, CompareNodes> &pq)
{
	Point2D pt;
	Node* pn1;
	vector<Node>::iterator gray_it;
	vector<Node>::iterator black_it;
	double cost;

	pt.setRow(row);
	pt.setCol(col);
	if (maze[row][col].GetValue() == SPACE)
		cost = 0.1; // space cost
	else if (maze[row][col].GetValue() == WALL)
		cost = 3;
	else // player or pickup object or something, we don't want pathes right next to them.
		cost = 5;
	// cost depends on is it a wall or a space
	pn1 = new Node(pt, pn->getTarget(), maze[pt.getRow()][pt.getCol()].GetValue(), pn->getG()+cost, pn);

	black_it = find(black.begin(), black.end(), *pn1);
	gray_it = find(gray.begin(), gray.end(), *pn1);
	if (black_it == black.end() && gray_it==gray.end()) // it is not black and not gray!
	{// i.e. it is white
		pq.push(pn1);
		gray.push_back(*pn1);
	}
}


void AddNeighbours(Node* pn, vector<Node> &gray, vector<Node> &black,
	priority_queue <Node*, vector<Node*>, CompareNodes> &pq)
{
	// try down
	if(pn->getPoint().getRow()<MSZ-1)
		AddNode(pn->getPoint().getRow() + 1, pn->getPoint().getCol(), pn, gray, black, pq);
	// try up
	if (pn->getPoint().getRow() >0)
		AddNode(pn->getPoint().getRow() - 1, pn->getPoint().getCol(), pn, gray, black, pq);
	// try left
	if (pn->getPoint().getCol() > 0)
		AddNode(pn->getPoint().getRow() , pn->getPoint().getCol()- 1, pn, gray, black, pq);
	// try right
	if (pn->getPoint().getCol() <MSZ-1)
		AddNode(pn->getPoint().getRow(), pn->getPoint().getCol() + 1, pn, gray, black, pq);
}


// implement A* from start to target
void GeneratePath(Point2D start, Point2D target)
{
	priority_queue <Node*, vector<Node*>, CompareNodes> pq;
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

void DigTunnels()
{
	int i, j;

	for (i = 0; i < NUM_ROOMS; i++)
	{
		cout << "Path from " << i << endl;
		for (j = i + 1; j < NUM_ROOMS; j++)
		{
			cout << " to " << j << endl;
			GeneratePath(rooms[i].getCenter(), rooms[j].getCenter());
		}
	}
}

void add_teams()
{
	int red[3] = { 1 , 0, 0 };
	Team* redTeam = new Team(red);

	//add a player in a random location at room 0
	Room teamRoom = rooms[0];
	int teamRoomLeftTopRow, teamRoomLeftTopCol, teamRoomRightBottomRow, teamRoomRightBottomCol , i , j;
	int maxNumOfPlayers = 5;

	for (int curNumOfPlayers = 0; curNumOfPlayers < maxNumOfPlayers; curNumOfPlayers++)
	{
		teamRoomLeftTopRow = teamRoom.getLeftTop().getRow();
		teamRoomLeftTopCol = teamRoom.getLeftTop().getCol();
		teamRoomRightBottomRow = teamRoom.getRightBottom().getRow();
		teamRoomRightBottomCol = teamRoom.getRightBottom().getCol();

		do
		{
			i = rand() % (teamRoomRightBottomRow - teamRoomLeftTopRow) + teamRoomLeftTopRow;
			j = rand() % (teamRoomRightBottomCol - teamRoomLeftTopCol) + teamRoomLeftTopCol;

		} while (maze[i][j].GetValue() == PLAYER); //collision prevention

		maze[i][j].SetValue(PLAYER);
		redTeam->AddPlayer(new Player(redTeam, &maze[i][j]));
	}
	teams.push_back(redTeam);
}

void setup_maze()
{
	int i, j, k;

	for (i = 0; i < MSZ; i++)
		for (j = 0; j < MSZ; j++)
		{
			maze[i][j].SetValue(WALL);
			maze[i][j].SetPoint(Point2D(i , j));
		}
	

	for (num_existing_rooms = 0; num_existing_rooms < NUM_ROOMS; num_existing_rooms++)
		rooms[num_existing_rooms] = GenerateRoom();


	for (k = 0; k < 30; k++)
	{
		i = rand() % MSZ;
		j = rand() % MSZ;
		maze[i][j].SetValue(WALL);
	}

	DigTunnels();

	add_teams();

}

void DrawMaze()
{
	int i, j;
	double sz, x, y;

	for(i=0;i<MSZ;i++)
		for (j = 0; j < MSZ; j++)
		{
			// set color
			switch (maze[i][j].GetValue())
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

	DrawMaze();

	if (pg != NULL)
	{
	//	pb->showMe();
		pg->showMe();
	}

	for (Team* curTeam : teams)
	{
		for (Player* curPlayer : curTeam->GetTeammates())
		{
			curPlayer->showMe();
		}
	}

	glutSwapBuffers();// show what was drawn in "frame buffer"
}

void displayMap()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer

	DrawMaze();
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
			pg->moveBullets(maze);

			//		move_on = pg->GetIsMoving();


		}

		for (Team* curTeam : teams)
		{
			for (Player* curPlayer : curTeam->GetTeammates())
			{
				curPlayer->move(maze);
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

		for (Team* curTeam : teams)
		{
			for (Player* curPlayer : curTeam->GetTeammates())
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



	Init();

	glutMainLoop();
}