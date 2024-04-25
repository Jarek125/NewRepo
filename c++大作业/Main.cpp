#include<stdio.h>
#include<easyx.h>
#include<iostream>
#include"Minesweeper.h"

using namespace std;

void stratupScene(ExMessage* msg);
bool isInrect(ExMessage* msg, int x, int y, int w, int h);

int main()
{
	initgraph(600, 400);
	
	IMAGE img;
	loadimage(&img,"images/StartMenu.jpg");
	putimage(0,0,&img);
	

	while (true)
	{	
		ExMessage msg;
		if (peekmessage(&msg, EX_MOUSE))
		{
			if (msg.message == WM_LBUTTONDOWN)
			{
				stratupScene(&msg);
				initgraph(600, 400);
				putimage(0, 0, &img);
				cout << "returntoMenu" << endl;
			}	
		}
	}

	getchar();
	return 0;
}


void stratupScene(ExMessage* msg)
{
	if (isInrect(msg, 242, 170, 352, 246))
	{
		exit(-1);
	}
	else if (isInrect(msg, 90, 65, 200, 140))
	{
		Minesweeper();
		cout << "closeMinesweeper" << endl;
		return;
	}
}

bool isInrect(ExMessage * msg, int x, int y, int w, int h)
{
	if (msg->x > x && msg->x<x + w && msg->y>y && msg->y < y + h)
	{
		return true;
	}
	return false;
}