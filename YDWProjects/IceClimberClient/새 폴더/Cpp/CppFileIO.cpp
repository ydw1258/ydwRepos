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
	cout << "����Ϸ�" << endl;
	outFile.close(); //�Ҹ��ڰ� �־ ȣ������ �ʾƵ� ������ ������ ȣ��
}
void inputStudent(Student st[], int& i)
{
	char str[256];
	cout << "��ȣ : ";
	cin >> st[i].num;
	cout << "�̸� : ";
	cin >> st[i].name;
	cout << "���� : ";
	cin >> st[i].kor;
	cout << "���� : ";
	cin >> st[i].eng;
	cout << "���� : ";
	cin >> st[i].math;

	cout << "�Է� �Ϸ�";
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
		cout << "1.�Է�\n 2.���\n 3.����\n 4.���� : ";
		cin >> num;

		if (num == 1)
			inputStudent(st, i);
		else if (num == 2)//�б�
			readStudent();
		else if (num == 3)//����
			saveStudent(st, i);
		else if (num == 4)
			break;
	}
}