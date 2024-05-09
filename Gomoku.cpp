#include<stdio.h>
#include<easyx.h>

#define GRID_W 25
#define ROW 15
#define COL 15
#define SPACE (3*GRID_W)

int J2 = 0;
enum Chess
{
	None,
	Black,
	White
};
int map[ROW][COL];
IMAGE bk;
struct Pos
{
	int row;
	int col;
	bool isShow;
	int player;
}pos = { -1,-1,false,Black };

void draw();
void loadResource();
void mouseMoveMsg(ExMessage* msg);
void mousePressMsg(ExMessage* msg);
bool judge(int r, int c);

void Gomoku()
{
	initgraph(600, 500);
	loadResource();

	while (1)
	{
		//解决闪屏 用双缓冲绘图
		BeginBatchDraw();
		draw();
		EndBatchDraw();

		//处理消息
		ExMessage msg;
		if (peekmessage(&msg, EX_MOUSE))
		{
			switch (msg.message)
			{
			case WM_MOUSEMOVE:
				mouseMoveMsg(&msg);
				break;
			case WM_LBUTTONDOWN:
			case WM_RBUTTONDOWN:
				mousePressMsg(&msg);
				break;
			default:
				break;
			}
		}

		if (J2 == -1)
		{
			J2 = 0;
			return;
		}
	}

	//getchar();
	//return 0;
}

void draw()
{
	putimage(0, 0, &bk);

	//网格线 15*15
	setlinecolor(BLACK);

	for (int i = 0; i < 15; i++)
	{
		line(SPACE, i * GRID_W + SPACE, 14 * GRID_W + SPACE, i * GRID_W + SPACE);
		line(i * GRID_W + SPACE, SPACE, i * GRID_W + SPACE, 14 * GRID_W + SPACE);
	}
	//画边框
	setlinestyle(PS_SOLID, 2);
	rectangle(SPACE, SPACE, SPACE + 14 * GRID_W, SPACE + 14 * GRID_W);
	//画点
	setfillcolor(BLACK);
	solidcircle(SPACE + 7 * GRID_W, SPACE + 7 * GRID_W, 5);
	solidcircle(SPACE + 3 * GRID_W, SPACE + 3 * GRID_W, 5);
	solidcircle(SPACE + 3 * GRID_W, SPACE + 11 * GRID_W, 5);
	solidcircle(SPACE + 11 * GRID_W, SPACE + 3 * GRID_W, 5);
	solidcircle(SPACE + 11 * GRID_W, SPACE + 11 * GRID_W, 5);

	//绘制棋子
	for (int i = 0; i < ROW; i++)
	{
		for (int k = 0; k < COL; k++)
		{
			if (map[i][k] == Black)
			{
				setfillcolor(BLACK);
				solidcircle(k * GRID_W + SPACE, i * GRID_W + SPACE, 10);
			}
			else if (map[i][k] == White)
			{
				setfillcolor(WHITE);
				solidcircle(k * GRID_W + SPACE, i * GRID_W + SPACE, 10);
			}
		}
	}

	//绘制蓝色的框
	setlinecolor(BLUE);
	if (pos.isShow)
	{
		rectangle(pos.col * GRID_W + SPACE - 12, pos.row * GRID_W + SPACE - 12, pos.col * GRID_W + SPACE + 12, pos.row * GRID_W + SPACE + 12);
	}
}

void loadResource()
{
	loadimage(&bk, "images/bk.jpg");
}

void mouseMoveMsg(ExMessage* msg)
{

	pos.isShow = false;
	for (int i = 0; i < ROW; i++)
	{
		for (int k = 0; k < COL; k++)
		{
			int gridx = k * GRID_W + SPACE;
			int gridy = i * GRID_W + SPACE;

			if (abs(msg->x - gridx) <= 12 && abs(msg->y - gridy) <= 12)
			{
				pos.isShow = true;
				pos.row = i;
				pos.col = k;
			}
		}
	}
}

void mousePressMsg(ExMessage* msg)
{
	//左键 下棋
	if (msg->message == WM_LBUTTONDOWN)
	{
		//只能在空位下棋
		if (map[pos.row][pos.col] == 0)
		{
			map[pos.row][pos.col] = pos.player;

			//判断输赢
			if (judge(pos.row, pos.col))
			{
				MessageBox(GetHWnd(), "你赢了", "hint", MB_OK);
				memset(map, 0, ROW * COL * sizeof(int));
				J2 = -1;
			}

			//切换
			pos.player = (pos.player == Black ? White : Black);

		}
	}
}

bool judge(int r, int c)
{
	int who = pos.player;
	for (int i = c - 4; i <= c; i++)
	{
		//注意下标
		if (i >= 0 && i < 15 &&
			map[r][i] == who && map[r][i + 1] == who &&
			map[r][i + 2] == who && map[r][i + 3] == who &&
			map[r][i + 4] == who)
		{
			return true;
		}
	}

	for (int i = r - 4; i <= r; i++)
	{
		//注意下标
		if (i >= 0 && i < 15 &&
			map[i][c] == who && map[i + 1][c] == who &&
			map[i + 2][c] == who && map[i + 3][c] == who &&
			map[i + 4][c] == who)
		{
			return true;
		}
	}

	for (int i = r - 4; i <= r; i++)
	{
		//注意下标
		for (int k = c - 4; k <= c; k++)
		{
			if (i >= 0 && i < 15 && k >= 0 && k < 15 &&
				map[i][k] == who && map[i + 1][k + 1] == who &&
				map[i + 2][k + 2] == who && map[i + 3][k + 3] == who &&
				map[i + 4][k + 4] == who)
			{
				return true;
			}
		}
	}

	for (int i = r - 4; i <= r; i++)
	{
		//注意下标
		for (int k = c + 4; k >= c; k--)
		{
			if (i >= 0 && i < 15 && k >= 0 && k < 15 &&
				map[i][k] == who && map[i + 1][k - 1] == who &&
				map[i + 2][k - 2] == who && map[i + 3][k - 3] == who &&
				map[i + 4][k - 4] == who)
			{
				return true;
			}
		}
	}

	return false;
}