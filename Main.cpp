#include<stdio.h>
#include<easyx.h>
#include<iostream>
#include"head.h"
#include<Windows.h>

using namespace std;

void stratupScene(ExMessage* msg);
bool isInrect(ExMessage* msg, int x, int y, int w, int h);

int main()
{
	//HWND hwnd = GetForegroundWindow();
	//ShowWindow(hwnd, SW_HIDE);

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

	return 0;
}


void stratupScene(ExMessage* msg)
{
	if (isInrect(msg, 242, 170, 352-242, 246-170))
	{
		exit(-1);
	}
	else if (isInrect(msg, 90, 65, 200-90, 140-65))
	{
		Minesweeper();
		cout << "closeMinesweeper" << endl;
		return;
	}
	else if (isInrect(msg, 240, 65, 355-240, 140-65))
	{
		_2048();
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