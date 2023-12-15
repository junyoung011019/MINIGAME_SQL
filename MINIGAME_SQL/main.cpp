#include <iostream>
#include <string>
#include <mysql.h>
#include <stdlib.h>
#pragma comment(lib, "libmySQL.lib")

using namespace std;

MYSQL Conn;                              // MySQL ���� ���� ����ü
MYSQL* ConnPtr = NULL;                  // MySQL �ڵ�
MYSQL_RES* Result;                      // ���� ������ ����� ��� ����ü ������
MYSQL_ROW Row;                  // ���� ������ ����� ���� ���� ������ ��� ����ü



int main() {
	
	///////////////////////////SQL ���� Ȯ��///////////////////////////

	MYSQL mysql;
	mysql_init(&mysql);
	if (!mysql_real_connect(&mysql, "34.64.226.23", "root", "", "MINIGAME", 3306, NULL, 0)) {
		cout << "error\n";
	}
	else {
		cout << "success\n";
	}

	mysql_init(&Conn); // MySQL ���� �ʱ�ȭ  

	///////////////////////////SQL ���� ����///////////////////////////
	
	ConnPtr = mysql_real_connect(&Conn, "34.64.226.23", "root", "", "MINIGAME", 3306, (char*)NULL, 0);

	// ���� ��� Ȯ��. null�� ��� ����
	if (ConnPtr == NULL) {
		fprintf(stderr, "Mysql query error:%s", mysql_error(&Conn));
		return 1;
	}

	///////////////////////////ȸ������///////////////////////////

	string UserName, UserId, UserPw;

	cout << "�̸��� �Է����ּ���.";
	cin >> UserName;
	while (1) {
		cout << "���̵� �Է����ּ���.";
		cin >> UserId;
		////���̵� �ߺ� Ȯ��

		// ID �ߺ� üũ�� ���� SQL ���� ����
		string idCheckQuery = "SELECT ID FROM customer_table WHERE ID = '" + UserId + "'";

		// SQL ���� ����
		mysql_query(&Conn, idCheckQuery.c_str());

		// ����� ����
		MYSQL_RES* idCheckResult = mysql_store_result(&Conn);

		// ��� ���� ������ Ȯ���Ͽ� �ߺ� ���θ� �Ǵ�
		if (mysql_num_rows(idCheckResult) == 0) {
			// ��� ��Ʈ ����
			mysql_free_result(idCheckResult);
			
			// �ߺ��� ������ �ݺ��� ����
			break;
		}

		std::cout << "�̹� �����ϴ� ���̵� �Դϴ�!\n";
		mysql_free_result(idCheckResult);
	}
	
	std::cout << "��й�ȣ�� �Է����ּ���.";
	std::cin >> UserPw;

	cout << "������ �������� ȯ���մϴ�!!!\n";
	Sleep(3000);

	//���� �ο� �Ǻ� �� No�� ����
	int No = 0; // ������ ȸ���� ��ϵǸ� NO(ȸ����ȣ)�� ȸ���� �ľ�


	// �� ���̺��� ���ڵ� ������ �������� ���� SQL ���� ����
	string AccountCountQuery = "SELECT COUNT(*) FROM customer_table";
	//�� ���̺� �ִ� ��� Į���� �˻����ּ���

	// SQL ���� ����
	if (mysql_query(&Conn, AccountCountQuery.c_str()) != 0) {
		// SQL ���� ���� �� ���� �߻� �� ���� �޽��� ���
		fprintf(stderr, "SQL �� ���� ����: %s\n", mysql_error(&Conn));
	}
	else {
		// SQL ���� ������ ������ ��� ����� ��������
		MYSQL_RES* result = mysql_use_result(&Conn);

		// ��� �� ��������
		MYSQL_ROW row = mysql_fetch_row(result);

		// ��� ���� �����ϴ� ���
		if (row != NULL) {
			// ���ڵ� ������ ������ ��ȯ�Ͽ� No ������ ����
			No = atoi(row[0]); //atoi()���� ������ ��ȯ

			// ��� ��Ʈ ����
			mysql_free_result(result);
		}
	}
	//���� �ο� +1 =No   ex) �����̺� 5���� ���� �����ϸ� ���� ȸ�����ʹ� NO.6 ����
	No += 1;

	// ȸ�� ������ �����ͺ��̽��� �����ϱ� ���� SQL ���� ���� �� ����
	string insertQuery = "INSERT INTO `MINIGAME`.`customer_table` (`No`, `ID`, `Name`, `Password`) VALUES ('" + to_string(No) + "','" + UserId + "', '" + UserName + "', '" + UserPw + "')";

	if (mysql_query(&Conn, insertQuery.c_str()) != 0) {
		fprintf(stderr, "Mysql query error:%s", mysql_error(&Conn));
	}



	///////////////////////////�α���///////////////////////////
	string InputId, InputPw;
	int missing = 0;
	string MemberNo = "NULL";

	while (1)
	{
		cout << "\n";
		cout << "���̵� �Է��ϼ���. >";
		cin >> InputId;
		cout << "��й�ȣ�� �Է��ϼ���. >";
		cin >> InputPw;

		// SQL ���� - customer_table���� ID�� �ִ� ��� Į���� �����ּ���
		string query = "SELECT * FROM customer_table WHERE ID = '" + InputId + "' AND password = '" + InputPw + "'";
		// SQL ������ �����ϰ� ���� ���� Ȯ��
		if (mysql_query(&Conn, query.c_str()) == 0) {
			// ����� ����
			MYSQL_RES* result = mysql_store_result(&Conn);

			// ����� ���� ���
			if (result) {
				// ù ��° ���� ������
				MYSQL_ROW row = mysql_fetch_row(result);

				// ���� �����ϸ� �α��� ����
				if (row) {
					// ��� ��Ʈ ����
					mysql_free_result(result);

					// �α����� ������� ������ �ٽ� �������� SQL ���� ����
					string whoLoginquery = "SELECT Name, NO FROM MINIGAME.customer_table WHERE ID = '" + InputId + "'";

					// SQL ������ �����ϰ� ���� ���� Ȯ��
					if (mysql_query(&Conn, whoLoginquery.c_str()) == 0) {
						// ����� ����
						MYSQL_RES* result = mysql_store_result(&Conn);

						// ����� ���� ���
						if (result != NULL) {
							// ù ��° ���� ������
							MYSQL_ROW row = mysql_fetch_row(result);

							// ���� �����ϸ� ȯ�� �޽��� ���
							if (row != NULL) {
								cout << "ȯ���մϴ�! " << row[0] << "��!\n"; //row[0]���� name�� ����Ǿ� ����
								MemberNo = row[1]; //row[1]���� No�� ����Ǿ� ����
								break;
							}
						}

						// ��� ��Ʈ ����
						mysql_free_result(result);
					}

					// 1�� ���
					Sleep(1000);
				}
				else {
					// ��й�ȣ ���� �ִ� 5�� ����
					missing += 1;
					cout << "��й�ȣ�� �����ʽ��ϴ�." << endl;
					cout << "���� ��й�ȣ" << missing << "ȸ �����Դϴ�." << endl;
					if (missing < 5) {
						continue;
					}
					cout << "�α��� 5ȸ ���н� �α����� ���ѵ˴ϴ�." << endl;

					//��� ��Ʈ ��ü
					mysql_free_result(result);

					//���α׷� ����
					cout << "\n";
					cout << "���� : �α��� 5ȸ ����" << endl;
					cout << "\n\n\n\n";
					exit(0);

				}

			}
		}
		else {
			// ���� ������ ���� �߻�
			cout << "SQL query error: " << mysql_error(&Conn);
		}
	}
	cout << "\n";

	///////////////////////////����ã�� ��� �߰�///////////////////////////
	int Test_mine_Score;
	string Test_mine_Player_ID=InputId;

	cout << "���� ã�� ���� �Է�>>";
	cin >> Test_mine_Score;
	cout << "\n";
	int mine_record_No = 0; // ������ ȸ���� ��ϵǸ� NO(ȸ����ȣ)�� ȸ���� �ľ�

	// �� ���̺��� ���ڵ� ������ �������� ���� SQL ���� ����
	string MineCountQuery = "SELECT COUNT(*) FROM minesweeper_game";
	//�� ���̺� �ִ� ��� Į���� �˻����ּ���

	// SQL ���� ����
	if (mysql_query(&Conn, MineCountQuery.c_str()) != 0) {
		// SQL ���� ���� �� ���� �߻� �� ���� �޽��� ���
		fprintf(stderr, "SQL �� ���� ����: %s\n", mysql_error(&Conn));
	}
	else {
		// SQL ���� ������ ������ ��� ����� ��������
		MYSQL_RES* result = mysql_use_result(&Conn);

		// ��� �� ��������
		MYSQL_ROW row = mysql_fetch_row(result);

		// ��� ���� �����ϴ� ���
		if (row != NULL) {
			// ���ڵ� ������ ������ ��ȯ�Ͽ� No ������ ����
			mine_record_No = atoi(row[0]); //atoi()���� ������ ��ȯ

			// ��� ��Ʈ ����
			mysql_free_result(result);
		}
	}
	//���� �ο� +1 =No   ex) �����̺� 5���� ���� �����ϸ� ���� ȸ�����ʹ� NO.6 ����
	mine_record_No += 2;

	string mineinsertQuery = "INSERT INTO `MINIGAME`.`minesweeper_game` (`No`, `Score`, `Player_ID`) VALUES ('" + to_string(mine_record_No) + "','" + to_string(Test_mine_Score) + "', '" + Test_mine_Player_ID + "')";

	if (mysql_query(&Conn, mineinsertQuery.c_str()) != 0) {
		fprintf(stderr, "Mysql query error:%s", mysql_error(&Conn));
	}

	cout << "\n";

	///////////////////////////Ÿ�ڰ��� ��� �߰�///////////////////////////

	int Test_typing_Score;
	string Test_typing_Player_ID = InputId;;

	cout << "Ÿ�� ���� ���� �Է�>>";
	cin >> Test_typing_Score;
	cout << "\n";

	int typing_record_No = 0; // ������ ȸ���� ��ϵǸ� NO(ȸ����ȣ)�� ȸ���� �ľ�

	// �� ���̺��� ���ڵ� ������ �������� ���� SQL ���� ����
	string TypingCountQuery = "SELECT COUNT(*) FROM typing_word_game";
	//�� ���̺� �ִ� ��� Į���� �˻����ּ���

	// SQL ���� ����
	if (mysql_query(&Conn, TypingCountQuery.c_str()) != 0) {
		// SQL ���� ���� �� ���� �߻� �� ���� �޽��� ���
		fprintf(stderr, "SQL �� ���� ����: %s\n", mysql_error(&Conn));
	}
	else {
		// SQL ���� ������ ������ ��� ����� ��������
		MYSQL_RES* result = mysql_use_result(&Conn);

		// ��� �� ��������
		MYSQL_ROW row = mysql_fetch_row(result);

		// ��� ���� �����ϴ� ���
		if (row != NULL) {
			// ���ڵ� ������ ������ ��ȯ�Ͽ� No ������ ����
			typing_record_No = atoi(row[0]); //atoi()���� ������ ��ȯ

			// ��� ��Ʈ ����
			mysql_free_result(result);
		}
	}
	//���� �ο� +1 =No   ex) �����̺� 5���� ���� �����ϸ� ���� ȸ�����ʹ� NO.6 ����
	typing_record_No += 2;

	string typinginsertQuery = "INSERT INTO `MINIGAME`.`typing_word_game` (`No`, `Score`, `Player_ID`) VALUES ('" + to_string(typing_record_No) + "','" + to_string(Test_typing_Score) + "', '" + Test_typing_Player_ID + "')";

	if (mysql_query(&Conn, typinginsertQuery.c_str()) != 0) {
		fprintf(stderr, "Mysql query error:%s", mysql_error(&Conn));
	}
	
	cout << "\n";

	///////////////////////////���ھ� ���� Ȯ��///////////////////////////

	cout << "����ã�� ���"<<endl;

	// ���� ã�� ���̺� ��� ���� ��û
	const char* mineprintquery = "SELECT * FROM minesweeper_game ORDER BY Score ASC";
	
	int mine_stat;
	mine_stat = mysql_query(&Conn, mineprintquery);

	if (mine_stat != 0) {
		fprintf(stderr, "Mysql query error:%s\n", mysql_error(ConnPtr));
		return 0;
	}

	Result = mysql_store_result(&Conn); //MySQL���� ������ ������ ��� ������ Result�� ����

	cout << "��� ";
	cout << "��� ";
	cout << "���̵�"<<endl;
	int mine_rank = 1;


	// �ݺ��� ������ Row ������ ����Ͽ� ��� ���տ��� �� �྿ �����͸� ó���̶�, NULL�� ���� ������ ��� �ݺ�
	while ((Row = mysql_fetch_row(Result)) != NULL)
	{
		cout << mine_rank<<"     ";
		cout << Row[0] << "  ";
		cout << Row[1] << endl;
		mine_rank++;
	}

	cout << "\n";

	cout << "Ÿ�ڰ��� ���"<<endl;

	// Ÿ�� ���� ���̺� ��� ���� ��û
	const char* typingprintquery = "SELECT * FROM typing_word_game ORDER BY Score DESC";

	int type_stat;
	type_stat = mysql_query(&Conn, typingprintquery);

	if (type_stat != 0) {
		fprintf(stderr, "Mysql query error:%s\n", mysql_error(ConnPtr));
		return 0;
	}

	Result = mysql_store_result(&Conn); //MySQL���� ������ ������ ��� ������ Result�� ����

	cout << "��� ";
	cout << "��� ";
	cout << "���̵�"<<endl;
	int typing_rank = 1;

	// �ݺ��� ������ Row ������ ����Ͽ� ��� ���տ��� �� �྿ �����͸� ó���̶�, NULL�� ���� ������ ��� �ݺ�
	while ((Row = mysql_fetch_row(Result)) != NULL)
	{
		cout << typing_rank << "     ";
		cout << Row[0] << "  ";
		cout << Row[1] << endl;
		typing_rank++;
	}
	
	cout << "\n";



	mysql_free_result(Result);// MySQL C API���� ����� �޸𸮸� �����ϴ� �Լ�
	mysql_close(ConnPtr); // MySQL �����ͺ��̽� ������ �ݴ� �Լ�
}