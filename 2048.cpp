#include<stdio.h>
#include<easyx.h>
#include<conio.h>

using namespace std;

#define MAX_GRID 4
#define GRID_width 100
#define INTERVAl 15

enum Color
{
	zero = RGB(205, 193, 180),
	twoTo1 = RGB(238, 228, 218),
	twoTo2 = RGB(237, 224, 200),
	twoTo3 = RGB(242, 177, 121),
	twoTo4 = RGB(245, 149, 99),
	twoTo5 = RGB(246, 124, 95),
	twoTo6 = RGB(246, 94, 59),
	twoTo7 = RGB(242, 177, 121),
	twoTo8 = RGB(237, 204, 97),
	twoTo9 = RGB(255, 0, 128),
	twoTo10 = RGB(145, 0, 72),
	twoTo11 = RGB(242, 17, 158),
	back = RGB(187, 173, 160)
};
Color arr[13] = { zero,twoTo1,twoTo2,twoTo3,twoTo4,twoTo5,twoTo6,twoTo7,twoTo8,twoTo9,twoTo10,twoTo11,back };
int map[MAX_GRID][MAX_GRID];//全局变量初始化为0
POINT pos[MAX_GRID][MAX_GRID];//内部存有x，y
int num[15] = { 0,2,4,8,16,32,64,128,256,512,1024,2048 };
bool flag = false;
int J = 0;

void GameDraw();
void GameInit();
void CreateNumber();
void GameControl();
void moveup();
void movedown();
void moveleft();
void moveright();
void GameJudge();
void winGame();

void _2048()
{
	J = 0;
	initgraph(MAX_GRID * GRID_width + 5 * INTERVAl, MAX_GRID * GRID_width + 5 * INTERVAl + 100, 1);
	GameInit();
	while (1)
	{
		GameDraw();
		GameControl();
		GameJudge();
		if (J == -1)
		{
			return;
		}
	}
	
	//getchar();
	//return 0;
}

void GameDraw()
{
	setbkcolor(back);
	cleardevice();
	settextstyle(50, 0, _T("Consolas"));
	settextcolor(RGB(119, 110, 101));
	outtextxy(45, MAX_GRID * GRID_width + 5 * INTERVAl + 20, "按下Backspace返回");


	for (int i = 0; i < MAX_GRID; i++)
	{
		for (int k = 0; k < MAX_GRID; k++)
		{
			for (int q = 0; q < 12; q++)
			{
				if (map[i][k] == num[q])
				{
					setfillcolor(arr[q]);
					solidrectangle(pos[i][k].x, pos[i][k].y, pos[i][k].x + GRID_width, pos[i][k].y + GRID_width);
					if (map[i][k] != 0)
					{
						char number[6] = " ";
						settextcolor(RGB(119, 110, 101));
						settextstyle(50, 0, _T("Consolas"));
						setbkmode(TRANSPARENT);//背景透明

						sprintf_s(number, "%d", map[i][k]);

						int tempx = GRID_width / 2 - textwidth(number) / 2;
						int tempy = GRID_width / 2 - textheight(number) / 2;
						//用格子一半减去字符一半

						outtextxy(pos[i][k].x + tempx, pos[i][k].y + tempy, number);
					}
				}
			}
		}
	}
}

void GameInit()
{
	//随机数种子
	srand(GetTickCount());
	//计算格子
	for (int i = 0; i < MAX_GRID; i++)
	{
		for (int k = 0; k < MAX_GRID; k++)
		{
			pos[i][k].x = k * GRID_width + (k + 1) * INTERVAl;
			pos[i][k].y = i * GRID_width + (i + 1) * INTERVAl;
		}
	}
	//测试
	//map[2][2]=8;

	//初始化
	CreateNumber();
	CreateNumber();
}

int twoOrFour()
{
	if (rand() % 10 == 1)
	{
		return 4;
	}
	else
	{
		return 2;
	}
}

void CreateNumber()
{
	while (1)
	{
		int x = rand() % MAX_GRID, y = rand() % MAX_GRID;
		if (map[x][y] == 0)
		{
			map[x][y] = twoOrFour();
			break;
		}
	}
}

void GameControl()
{
	//获取
	char key = _getch();
	switch (key)
	{
	case 'w':
	case 'W':
	case 72:
		moveup();
		break;
	case 's':
	case 'S':
	case 80:
		movedown();
		break;
	case 'a':
	case 'A':
	case 75:
		moveleft();
		break;
	case 'd':
	case 'D':
	case 77:
		moveright();
		break;
	case 8:
		J= -1;
		memset(map, 0, MAX_GRID * MAX_GRID * sizeof(int));
		break;
	}
}

void moveup()
{
	for (int i = 0; i < MAX_GRID; i++)
	{
		int temp = 0;
		for (int begin = 1; begin < MAX_GRID; begin++)
		{
			if (map[begin][i] != 0)
			{
				if (map[temp][i] == 0)
				{
					map[temp][i] = map[begin][i];
					map[begin][i] = 0;
				}
				else if (map[temp][i] == map[begin][i])
				{
					map[temp][i] += map[begin][i];
					map[begin][i] = 0;
					if (map[temp][i] == 2048)
					{
						winGame();
					}
				}
				else
				{
					map[temp + 1][i] = map[begin][i];
					if (temp + 1 != begin)
					{
						map[begin][i] = 0;
					}
				}
				temp++;
				flag = true;
			}
		}
	}
	printf("up\n");
}
void movedown()
{
	for (int i = 0; i < MAX_GRID; i++)
	{
		int temp = MAX_GRID - 1;
		for (int begin = MAX_GRID - 2; begin >= 0; begin--)
		{
			if (map[begin][i] != 0)
			{
				if (map[temp][i] == 0)
				{
					map[temp][i] = map[begin][i];
					map[begin][i] = 0;
				}
				else if (map[temp][i] == map[begin][i])
				{
					map[temp][i] += map[begin][i];
					map[begin][i] = 0;
					if (map[temp][i] == 2048)
					{
						winGame();
					}
				}
				else
				{
					map[temp - 1][i] = map[begin][i];
					if (temp - 1 != begin)
					{
						map[begin][i] = 0;
					}
				}
				temp--;
				flag = true;
			}
		}
	}
	printf("down\n");
}
void moveleft()
{
	for (int i = 0; i < MAX_GRID; i++)
	{
		int temp = 0;
		for (int begin = 1; begin < MAX_GRID; begin++)
		{
			if (map[i][begin] != 0)
			{
				if (map[i][temp] == 0)
				{
					map[i][temp] = map[i][begin];
					map[i][begin] = 0;
				}
				else if (map[i][temp] == map[i][begin])
				{
					map[i][temp] += map[i][begin];///
					map[i][begin] = 0;
					if (map[i][temp] == 2048)
					{
						winGame();
					}
				}
				else
				{
					map[i][temp + 1] = map[i][begin];
					if (temp + 1 != begin)
					{
						map[i][begin] = 0;
					}
				}
				temp++;
				flag = true;
			}
		}
	}
	printf("left\n");
}
void moveright()
{
	for (int i = 0; i < MAX_GRID; i++)
	{
		int temp = MAX_GRID - 1;
		for (int begin = MAX_GRID - 2; begin >= 0; begin--)
		{
			if (map[i][begin] != 0)
			{
				if (map[i][temp] == 0)
				{
					map[i][temp] = map[i][begin];
					map[i][begin] = 0;
				}
				else if (map[i][temp] == map[i][begin])
				{
					map[i][temp] += map[i][begin];
					map[i][begin] = 0;
					if (map[i][temp] == 2048)
					{
						winGame();
					}
				}
				else
				{
					map[i][temp - 1] = map[i][begin];
					if (temp - 1 != begin)
					{
						map[i][begin] = 0;
					}
				}
				temp--;
				flag = true;
			}
		}
	}
	printf("right\n");
}

void GameJudge()
{
	if (flag)
	{
		CreateNumber();
		flag = false;
	}
}

void winGame()
{
	int ret = MessageBox(GetHWnd(), "你赢了！是否再玩一局？", "hint", MB_OKCANCEL);
	if (ret == IDOK)
	{
		cleardevice();
		closegraph();
		memset(map, 0, MAX_GRID * MAX_GRID * sizeof(int));
		_2048();
	}
	else if (ret == IDCANCEL)
	{
		cleardevice();
		closegraph();
		memset(map, 0, MAX_GRID * MAX_GRID * sizeof(int));
		J=-1;//
	}
}