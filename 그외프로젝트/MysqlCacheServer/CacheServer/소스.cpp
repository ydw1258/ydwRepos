#include<iostream>
#include <mysql.h>
using namespace std;
#include <WinSock2.h>

#pragma comment(lib, "libmysql.lib")

//include, lib�߰��� �� libmysql.dll������ ���� ������Ʈ�� ����
#pragma comment(lib, "ws2_32.lib")

int main()
{
	cout << mysql_get_client_info() << endl;
	
	MYSQL conn;
	MYSQL_RES* pSqlRes;
	MYSQL_ROW sqlRow;
	
	if (mysql_init(&conn) == nullptr)
	{
		cout << "mysql_init_fail" << endl;
		return 1;
	}
	if (mysql_real_connect(&conn, "localhost", "YDW", "1234", "test_game", 3306, NULL, 0) == nullptr)
	{
		cout << "mysql_real_connection_fail" << endl;
		return 1;
	}
	if (mysql_select_db(&conn, "test_game"))
	{
		cout << "mysql_select_fail : " << mysql_errno(&conn) << " : " << mysql_error(&conn) << endl;
		return 1;
	}

	const char *query = "select * from user_data";
	int state = mysql_query(&conn, query);
	
	if(state == 0)
	{
		//query�� select�ܿ��� ����� null�� (���������� select���� ����, ������ �ᵵ ����)
		pSqlRes = mysql_store_result(&conn); 
		if (pSqlRes != nullptr)
		{
			int numCol = mysql_num_fields(pSqlRes);
			
			while ((sqlRow = mysql_fetch_row(pSqlRes)) != nullptr)
			{
				for (int i = 0; i < numCol; i++)
				{
					cout << sqlRow[i] << " ";
				}
				cout << endl;
			}
		}
	}

	return 0;
}