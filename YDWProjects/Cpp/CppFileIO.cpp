#include<iostream>
#include<fstream>
using namespace std;

struct Student
{
	int num;
	char name[256];
	int kor;
	int eng;
	int math;
};
void saveStudent(Student st[], int i)
{
	ofstream outFile("Debug/ioFile.txt", ios::out);

	for (int j = 0; j < i; j++)
	{
		outFile << st[j].num << " " << st[j].name << " " << st[j].kor << " " << st[j].eng << " " << st[j].math << endl;
	}
	cout << "저장완료" << endl;
	outFile.close(); //소멸자가 있어서 호출하지 않아도 되지만 습관상 호출
}
void inputStudent(Student st[], int& i)
{
	char str[256];
	cout << "번호 : ";
	cin >> st[i].num;
	cout << "이름 : ";
	cin >> st[i].name;
	cout << "국어 : ";
	cin >> st[i].kor;
	cout << "영어 : ";
	cin >> st[i].eng;
	cout << "수학 : ";
	cin >> st[i].math;

	cout << "입력 완료";
	i++;
}
void readStudent()
{
	char temp[1024];
	ifstream inFile("Debug/ioFile.txt", ios::in);

	while (!inFile.eof())
	{
		inFile.getline(temp, 100);
		cout << temp << endl;
	}
}

int main()
{
	int szBuf = 5;

	Student st[100];
	int i = 0;

	while (true)
	{
		int num;
		cout << "1.입력\n 2.출력\n 3.저장\n 4.종료 : ";
		cin >> num;

		if (num == 1)
			inputStudent(st, i);
		else if (num == 2)//읽기
			readStudent();
		else if (num == 3)//쓰기
			saveStudent(st, i);
		else if (num == 4)
			break;
	}
}