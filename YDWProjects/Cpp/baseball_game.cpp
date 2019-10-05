#include<iostream>
#include<cmath>
#include<time.h>

using namespace std;

static enum JUDGE
{
	BALL,
	STRIKE,
	OUT
};
int main()
{
	srand((unsigned int)time(0));
	
	while (true)
	{
		int a[3];
		for(int i = 0; i < 3; i++)
			a[i] = rand() % 10 + 1;
		
		int player[3];
		
		int judge[3] = {0}; //0 ball 1 strike 2 out

		for (int i = 0; i < 3; i++)
		{
			cout << "플레이어 입력 : ";
			cin >> player[i];
		}
		
		for (int i = 0; i < 3; i++)
		{
			if (a[i] == player[i])
			{
				judge[0]++;
				continue;
			}
			for (int j = 0; j < 3; j++)
			{
				if (a[j] == player[i])
				{
					judge[1]++;
					break;
				}
				else if (j == 2)
				{
					judge[2]++;
				}
			}
		}
		for (int i = 0; i < 3; i++)
			cout << a[i] << " ";
		cout << endl;
		cout << "ball : " << judge[0] << endl
			<< "strike : " << judge[1] << endl
			<< "out : " << judge[2] << endl;
	}
}
