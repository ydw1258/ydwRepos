#include<iostream>
#include<fstream>
#include<vector>
using namespace std;

struct data
{
	int index;
	char Name[256];
};
vector<string> split(string str, char delimiter)
{
	uint64_t start_pos = 0;
	uint64_t search_pos = 0;
	std::vector<std::string> result;

	while (start_pos < str.size())
	{
		search_pos = str.find_first_of(delimiter, start_pos);
		std::string tmp_str;

		if (search_pos == std::string::npos)
		{
			// for last token
			search_pos = str.size();
			tmp_str = str.substr(start_pos, search_pos - start_pos);
			result.push_back(tmp_str);
			break;
		}
		tmp_str = str.substr(start_pos, search_pos - start_pos);
		result.push_back(tmp_str);
		start_pos = search_pos + 1;
	}

	return result;
}

void readCSV()
{
	string str[100];
	ifstream inFile("Debug/sample.csv", ios::in);
	ofstream outFile("Debug/ioFile.txt", ios::out);

	int i = 0;

	while (!inFile.eof())
	{
		char temp[100];
		inFile.getline(temp, 100);
		cout << temp << endl;
		str[i] = temp;
		vector<string> v = split(str[i], ',');
		vector<string>::iterator pt;

		for (pt = v.begin(); pt != v.end(); pt++)
		{
			cout << *pt->c_str() << endl;
		}
		//outFile << str[i].c_str() << endl;
		
		i++;
	}
	outFile.close(); //소멸자가 있어서 호출하지 않아도 되지만 습관상 호출
}

int main()
{
	readCSV();
}