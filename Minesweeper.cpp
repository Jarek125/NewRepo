#include<iostream>
#include<stdlib.h>
#include<time.h>
#include<easyx.h>

using namespace std;

#define ROW 10
#define COL 10
#define IMGW 40
#define Num 10

void initmap(int map[ROW][COL]);
void drawmap(int map[ROW][COL], IMAGE img[]);
void mouseEvent(int map[ROW][COL]);
void openNull(int map[ROW][COL], int row, int col);
void judge(int map[ROW][COL], int row, int col);
void showmap(int map[ROW][COL]);

bool isOver = false;
bool result = false;

void Minesweeper()
{
	//创建图形窗口
	initgraph(COL*IMGW, COL*IMGW, EX_SHOWCONSOLE);
	//生成随机数
	srand((unsigned)time(NULL));

	//定义游戏地图
	int map[ROW][COL] = { 0 };
	//初始化数据
	initmap(map);
	//把数据转换成图形界面
	IMAGE img[12];//定义图片数组
	//加载图片
	for (int i = 0; i < 12; i++)
	{
		char fileName[50] = { 0 };
		sprintf_s(fileName, "./images/%d.jpg", i);
		loadimage(img + i, fileName, IMGW, IMGW);
		//putimage(i * IMGW, 0, img + i);
	}

	while (true)
	{
		mouseEvent(map);
		drawmap(map, img);

		if (isOver)
		{
			if (result==false)
			{
				int ret = MessageBox(GetHWnd(), "你刚刚踩到雷了，是否再玩一局？", "hint", MB_OKCANCEL);
				if (ret == IDOK)
				{
					initmap(map);
					isOver = false;
				}
				else if (ret == IDCANCEL)
				{
					initmap(map);
					cleardevice();
					closegraph();
					return;
				}
			}
			else if (result==true)
			{
				int ret = MessageBox(GetHWnd(), "你刚刚赢了！是否再玩一局？", "hint", MB_OKCANCEL);
				if (ret == IDOK)
				{
					initmap(map);
					isOver = false;
				}
				else if (ret == IDCANCEL)
				{
					initmap(map);
					cleardevice();
					closegraph();
					return;
				}
			}
		}
	}

	//showmap(map);
	//return 0;
}

void showmap(int map[ROW][COL])
{
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			cout << map[i][j] << '\t';
		}
		cout << endl << endl;
	}
	cout << endl << endl;
}

void initmap(int map[ROW][COL])
{
	//把map清零
	memset(map, 0, ROW * COL * sizeof(int));

	//随机埋雷    后续增加难度需要调整这里
	for (int i = 0; i < Num; )
	{
		int r = rand() % ROW;//[0,10)
		int c = rand() % COL;
		if (map[r][c] == 0)
		{
			map[r][c] = -1;
			i++;
		}
	}

	//数字加一
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			if (map[i][j] == -1)
			{
				for (int k = i - 1; k <= i + 1; k++)
				{
					for (int r = j - 1; r <= j + 1; r++)
					{//数组越界问题
						if ((k >= 0 && k < ROW && r >= 0 && r < COL) && map[k][r] != -1)
						{
							map[k][r]++;
						}
					}
				}
			}
		}
	}

	//把所有的格子都用遮罩图盖起来，加密
	for (int i = 0; i < ROW; i++)
	{
		for (int k = 0; k < COL; k++)
		{
			map[i][k] += 20;
		}
	}
}

void drawmap(int map[ROW][COL], IMAGE img[])
{
	//数组数据转换成对应图片，并贴图
	for (int i = 0; i < ROW; i++)
	{
		for (int k = 0; k < COL; k++)
		{
			//空白和数字
			if (map[i][k] >= 0 && map[i][k] <= 8)
			{
				putimage(k * IMGW, i * IMGW, img + map[i][k]);
			}
			//雷
			else if (map[i][k] == -1)
			{
				putimage(k * IMGW, i * IMGW, img + 9);
			}
			//遮罩图
			else if (map[i][k] >= 19 && map[i][k] <= 28)
			{
				putimage(k * IMGW, i * IMGW, img + 10);
			}
		}
	}
}

void mouseEvent(int map[ROW][COL])
{
	//定义消息结构体（鼠标 按键 字符)
	ExMessage msg;
	if (peekmessage(&msg, EX_MOUSE))
	{
		int c = msg.x / IMGW, r = msg.y / IMGW;
		if (msg.message == WM_LBUTTONDOWN)
		{
			if (map[r][c] >= 19 && map[r][c] <= 28)//只能按一次
			{
				map[r][c] -= 20;
				openNull(map, r, c);
				judge(map, r, c);
				showmap(map);
			}
		}
	}
}

void openNull(int map[ROW][COL], int row, int col)
{
	//判断当前点击的是不是空白
	if (map[row][col] == 0)
	{
		for (int i = row - 1; i <= row + 1; i++)//九宫格
		{
			for (int k = col - 1; k <= col + 1; k++)
			{//判断是否在区域内 且 非雷 且 未被点开
				if (((i >= 0 && i < ROW && k >= 0 && k < COL) && map[i][k] != -1) && map[i][k] >= 19 && map[i][k] <= 28)
				{
					map[i][k] -= 20;
					openNull(map, i, k);
				}
			}
		}
	}
}

void judge(int map[ROW][COL], int row, int col)
{
	//点到雷就结束
	if (map[row][col] == -1)
	{
		for (int i = 0; i < ROW; i++)
		{
			for (int k = 0; k < COL; k++)
			{
				if (map[i][k] == 19)
				{
					map[i][k] -= 20;
				}
			}
		}
		isOver = true;
		result = false;
	}
	
	//通关 所有格子都点开了
	if (map[row][col] != -1)
	{
		int curOpenGridcnt = 0;

		for (int i = 0; i < ROW; i++)
		{
			for (int k = 0; k < COL; k++)
			{
				if (map[i][k] >= 0 && map[i][k] <= 8)
				{
					++curOpenGridcnt;
				}
			}
		}

		//cout << curOpenGridcnt << endl;

		if (curOpenGridcnt == ROW * COL - Num)
		{
			isOver = true;
			result = true;
		}
	}
}