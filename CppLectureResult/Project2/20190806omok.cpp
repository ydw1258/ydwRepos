#include<iostream>
#include<conio.h>
using namespace std;

#define WIDTH 19
#define HEIGHT 19

#define DRAW_EMPTY 0
#define BLACK 1
#define WHITE 2

int main()
{
	int x = 0;
	int y = 0;
	int turn = 0;
	int map[HEIGHT][WIDTH] = { };

	while (true)
	{
		if (turn % 2 == 0)
		{
			cout << "������Դϴ�.";
		}
		else
		{
			cout << "������Դϴ�.";
		}

		cout << x << y << endl;
		
		for (int i = 0; i < WIDTH; i++)
		{
			for (int j = 0; j < HEIGHT; j++)
			{
				if (j == x && i == y)
				{
					cout << "��";
				}
				else if (map[i][j] == WHITE)
				{
					cout << "��";
				}
				else if (map[i][j] == BLACK)
				{
					cout << "��";
				}
				else
				{
					if (i == 0 && j == 0)
					{
						cout << "��";
					}
					else if (i == 0 && j == WIDTH - 1)
					{
						cout << "��";
					}
					else if (i == HEIGHT - 1 && j == 0)
					{
						cout << "��";
					}
					else if (i == HEIGHT - 1 && j == WIDTH - 1)
					{
						cout << "��";
					}
					else if (i == 0)
					{
						cout << "��";
					}
					else if (j == 0)
					{
						cout << "��";
					}
					else if (i == HEIGHT - 1)
					{
						cout << "��";
					}
					else if (j == WIDTH - 1)
					{
						cout << "��";
					}
					else
					{
						cout << "��";
					}
				}

			}
			cout << endl;
		}

		char input = getch();
		
		system("cls");

		if (input == 'w')
		{
			if (y >= 0)
				y--;
			else
				y = HEIGHT - 1;
		}
		else if(input == 'a')
		{
			if (x >= 0)
				x--;
			else
				x = WIDTH - 1;
		}
		else if (input == 's')
		{
			if (y < HEIGHT)
				y++;
			else
				y = 0;
		}
		else if (input == 'd')
		{
			if (x < WIDTH)
				x++;
			else
				x = 0;
		}
		else if (input == 'v')
		{
			if (turn % 2 == 0)
			{
				map[y][x] = BLACK;
			}
			else
			{
				map[y][x] = WHITE;
			}
			
			//���� �˻� üũ
			for (int i = 0; i < HEIGHT; i++)
			{
				for (int j = 0; j < WIDTH; j++)
				{
					if (map[i][j] == BLACK || map[i][j] == WHITE)
					{
						//������, �Ʒ�, �����ʴ밢��, ���� �밢�� Ȯ��
						if (map[i][j] == map[i][j + 1] && map[i][j] == map[i][j + 2] && map[i][j] == map[i][j + 3] && map[i][j] == map[i][j + 4] ||
							map[i][j] == map[i + 1][j] && map[i][j] == map[i + 2][j] && map[i][j] == map[i + 3][j] && map[i][j] == map[i + 4][j] ||
							map[i][j] == map[i + 1][j + 1] && map[i][j] == map[i + 2][j + 2] && map[i][j] == map[i + 3][j + 3] && map[i][j] == map[i + 4][j + 4] ||
							j >= 4 && (map[i][j] == map[i + 1][j - 1] && map[i][j] == map[i + 2][j - 2] && map[i][j] == map[i + 3][j - 3] && map[i][j] == map[i + 4][j - 4])
							)
						{
							if (turn % 2 == 0)
							{
								cout << "�浹 �¸�";
								return 0;
							}
							else
							{
								cout << "�鵹 �¸�";
								return 0;
							}

						}
					}
				}
			}
			turn++;
		}
	}
	
	return 0;
}