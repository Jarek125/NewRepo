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
	//����ͼ�δ���
	initgraph(COL*IMGW, COL*IMGW, EX_SHOWCONSOLE);
	//���������
	srand((unsigned)time(NULL));

	//������Ϸ��ͼ
	int map[ROW][COL] = { 0 };
	//��ʼ������
	initmap(map);
	//������ת����ͼ�ν���
	IMAGE img[12];//����ͼƬ����
	//����ͼƬ
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
				int ret = MessageBox(GetHWnd(), "��ողȵ����ˣ��Ƿ�����һ�֣�", "hint", MB_OKCANCEL);
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
				int ret = MessageBox(GetHWnd(), "��ո�Ӯ�ˣ��Ƿ�����һ�֣�", "hint", MB_OKCANCEL);
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
	//��map����
	memset(map, 0, ROW * COL * sizeof(int));

	//�������    ���������Ѷ���Ҫ��������
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

	//���ּ�һ
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			if (map[i][j] == -1)
			{
				for (int k = i - 1; k <= i + 1; k++)
				{
					for (int r = j - 1; r <= j + 1; r++)
					{//����Խ������
						if ((k >= 0 && k < ROW && r >= 0 && r < COL) && map[k][r] != -1)
						{
							map[k][r]++;
						}
					}
				}
			}
		}
	}

	//�����еĸ��Ӷ�������ͼ������������
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
	//��������ת���ɶ�ӦͼƬ������ͼ
	for (int i = 0; i < ROW; i++)
	{
		for (int k = 0; k < COL; k++)
		{
			//�հ׺�����
			if (map[i][k] >= 0 && map[i][k] <= 8)
			{
				putimage(k * IMGW, i * IMGW, img + map[i][k]);
			}
			//��
			else if (map[i][k] == -1)
			{
				putimage(k * IMGW, i * IMGW, img + 9);
			}
			//����ͼ
			else if (map[i][k] >= 19 && map[i][k] <= 28)
			{
				putimage(k * IMGW, i * IMGW, img + 10);
			}
		}
	}
}

void mouseEvent(int map[ROW][COL])
{
	//������Ϣ�ṹ�壨��� ���� �ַ�)
	ExMessage msg;
	if (peekmessage(&msg, EX_MOUSE))
	{
		int c = msg.x / IMGW, r = msg.y / IMGW;
		if (msg.message == WM_LBUTTONDOWN)
		{
			if (map[r][c] >= 19 && map[r][c] <= 28)//ֻ�ܰ�һ��
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
	//�жϵ�ǰ������ǲ��ǿհ�
	if (map[row][col] == 0)
	{
		for (int i = row - 1; i <= row + 1; i++)//�Ź���
		{
			for (int k = col - 1; k <= col + 1; k++)
			{//�ж��Ƿ��������� �� ���� �� δ���㿪
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
	//�㵽�׾ͽ���
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
	
	//ͨ�� ���и��Ӷ��㿪��
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