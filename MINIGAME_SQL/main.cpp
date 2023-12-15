#include <iostream>
#include <string>
#include <mysql.h>
#pragma comment(lib, "libmySQL.lib")


using namespace std;


int main() {

	//cout << "MySQL Client Version: " << mysql_get_client_info(); // MySQL 버전 출력(연동 성공)

	MYSQL mysql;
	mysql_init(&mysql);
	if (!mysql_real_connect(&mysql, "34.64.226.23", "root", "", "MINIGAME", 3306, NULL, 0))
		cout << "error";
	else
		cout << "success";
	mysql_close(&mysql);


	return 0;
}