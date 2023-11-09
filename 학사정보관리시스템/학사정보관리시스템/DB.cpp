#include <iostream>
#include <my_global.h>
#include <WinSock2.h>
#include <mysql.h>
#pragma comment(lib, "libmysql.lib")
using namespace std;

int dbconnect()
{
	cout  << mysql_get_client_info() << "\n\n";

	MYSQL Conn;
	MYSQL* ConnPtr = NULL;  // MYSQL Handle
	MYSQL_RES* Result;      // Query ��� ��� ����ü ������
	MYSQL_ROW Row;         // Query ����� ���� �� ������ ��� ����ü
	int Stat;            // Query ���� ���� �޴� ����

	mysql_init(&Conn);      // MYSQL initiated

							// DB ����
	ConnPtr = mysql_real_connect(&Conn, "localhost", "root", "123123", "syst", 3306, (char*)NULL, 0);

	// Connection Status
	if (ConnPtr == NULL)
	{
		cout << stderr <<"Mysql Connection Error : " << mysql_error(&Conn)<< "\n";
		return 1;
	}

	// Query ��û
	const char* Query = "Select * FROM members";
	mysql_query(ConnPtr, "set names euckr");
	Stat = mysql_query(ConnPtr, Query);
	if (Stat != 0)
	{
		cout << stderr << "Mysql Err : " << mysql_error(&Conn);
		return 1;
	}

	Result = mysql_store_result(ConnPtr);
	while ((Row = mysql_fetch_row(Result)) != NULL)
	{
		for (int i = 0; i <sizeof(Row); i++) {
			cout << Row[i]<<endl;

		}

	}

	// DB ���� ����
	mysql_close(ConnPtr);

	return 0;
}


