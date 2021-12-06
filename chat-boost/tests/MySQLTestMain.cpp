#include <iostream>

#include <mysql.h>

#pragma comment(lib, "libmysql.lib")

using namespace std;

// MySQL LOCAL connection info

const char* host = "localhost";
const char* user = "root";
const char* passwd = "";
const char* db = "open_lobby";
const unsigned int port = 3306;
const char* unix_socket = nullptr;
const unsigned long clientflag = 0;

// -- end info --

int mysql_test_main(void)
{
	try
	{
		MYSQL* pMysql{};
		mysql_init(pMysql);

		if (!mysql_real_connect(pMysql, host, user, passwd, db, port, unix_socket, clientflag))
		{
			cout << "error\n";
		}
		else
		{
			cout << "success\n";
			int queryCount = mysql_query(pMysql,
				"SELECT * \
			FROM RED_SUER"
			);
			if (queryCount > 0)
			{
				cout << queryCount << endl;
			}
		}

		mysql_close(pMysql);
	}
	catch (const std::exception& e)
	{
		cerr << e.what() << endl;
	}
	
	cout << endl;
	return EXIT_SUCCESS;
}
