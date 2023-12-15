#include <iostream>
#include <string>
#include <mysql.h>
#include <stdlib.h>
#pragma comment(lib, "libmySQL.lib")

using namespace std;

MYSQL Conn;                              // MySQL 정보 담을 구조체
MYSQL* ConnPtr = NULL;                  // MySQL 핸들
MYSQL_RES* Result;                      // 쿼리 성공시 결과를 담는 구조체 포인터
MYSQL_ROW Row;                  // 쿼리 성공시 결과로 나온 행의 정보를 담는 구조체



int main() {
	
	///////////////////////////SQL 연동 확인///////////////////////////

	MYSQL mysql;
	mysql_init(&mysql);
	if (!mysql_real_connect(&mysql, "34.64.226.23", "root", "", "MINIGAME", 3306, NULL, 0)) {
		cout << "error\n";
	}
	else {
		cout << "success\n";
	}

	mysql_init(&Conn); // MySQL 정보 초기화  

	///////////////////////////SQL 쿼리 연동///////////////////////////
	
	ConnPtr = mysql_real_connect(&Conn, "34.64.226.23", "root", "", "MINIGAME", 3306, (char*)NULL, 0);

	// 연결 결과 확인. null일 경우 실패
	if (ConnPtr == NULL) {
		fprintf(stderr, "Mysql query error:%s", mysql_error(&Conn));
		return 1;
	}

	///////////////////////////회원가입///////////////////////////

	string UserName, UserId, UserPw;

	cout << "이름을 입력해주세요.";
	cin >> UserName;
	while (1) {
		cout << "아이디를 입력해주세요.";
		cin >> UserId;
		////아이디 중복 확인

		// ID 중복 체크를 위한 SQL 쿼리 생성
		string idCheckQuery = "SELECT ID FROM customer_table WHERE ID = '" + UserId + "'";

		// SQL 쿼리 실행
		mysql_query(&Conn, idCheckQuery.c_str());

		// 결과를 저장
		MYSQL_RES* idCheckResult = mysql_store_result(&Conn);

		// 결과 행의 개수를 확인하여 중복 여부를 판단
		if (mysql_num_rows(idCheckResult) == 0) {
			// 결과 세트 해제
			mysql_free_result(idCheckResult);
			
			// 중복이 없으면 반복문 종료
			break;
		}

		std::cout << "이미 존재하는 아이디 입니다!\n";
		mysql_free_result(idCheckResult);
	}
	
	std::cout << "비밀번호를 입력해주세요.";
	std::cin >> UserPw;

	cout << "가입을 진심으로 환영합니다!!!\n";
	Sleep(3000);

	//가입 인원 판별 및 No값 삽입
	int No = 0; // 앞으로 회원이 등록되면 NO(회원번호)로 회원을 파악


	// 고객 테이블의 레코드 개수를 가져오기 위한 SQL 쿼리 생성
	string AccountCountQuery = "SELECT COUNT(*) FROM customer_table";
	//고객 테이블에 있는 모든 칼럼을 검색해주세요

	// SQL 쿼리 실행
	if (mysql_query(&Conn, AccountCountQuery.c_str()) != 0) {
		// SQL 쿼리 실행 중 오류 발생 시 오류 메시지 출력
		fprintf(stderr, "SQL 문 실행 오류: %s\n", mysql_error(&Conn));
	}
	else {
		// SQL 쿼리 실행이 성공한 경우 결과를 가져오기
		MYSQL_RES* result = mysql_use_result(&Conn);

		// 결과 행 가져오기
		MYSQL_ROW row = mysql_fetch_row(result);

		// 결과 행이 존재하는 경우
		if (row != NULL) {
			// 레코드 개수를 정수로 변환하여 No 변수에 저장
			No = atoi(row[0]); //atoi()통해 정수로 반환

			// 결과 세트 해제
			mysql_free_result(result);
		}
	}
	//가입 인원 +1 =No   ex) 고객테이블에 5명의 고객이 존재하면 다음 회원부터는 NO.6 지정
	No += 1;

	// 회원 정보를 데이터베이스에 삽입하기 위한 SQL 쿼리 생성 및 실행
	string insertQuery = "INSERT INTO `MINIGAME`.`customer_table` (`No`, `ID`, `Name`, `Password`) VALUES ('" + to_string(No) + "','" + UserId + "', '" + UserName + "', '" + UserPw + "')";

	if (mysql_query(&Conn, insertQuery.c_str()) != 0) {
		fprintf(stderr, "Mysql query error:%s", mysql_error(&Conn));
	}



	///////////////////////////로그인///////////////////////////
	string InputId, InputPw;
	int missing = 0;
	string MemberNo = "NULL";

	while (1)
	{
		cout << "\n";
		cout << "아이디를 입력하세요. >";
		cin >> InputId;
		cout << "비밀번호를 입력하세요. >";
		cin >> InputPw;

		// SQL 쿼리 - customer_table에서 ID에 있는 모든 칼럼을 보여주세요
		string query = "SELECT * FROM customer_table WHERE ID = '" + InputId + "' AND password = '" + InputPw + "'";
		// SQL 쿼리를 실행하고 성공 여부 확인
		if (mysql_query(&Conn, query.c_str()) == 0) {
			// 결과를 저장
			MYSQL_RES* result = mysql_store_result(&Conn);

			// 결과가 있을 경우
			if (result) {
				// 첫 번째 행을 가져옴
				MYSQL_ROW row = mysql_fetch_row(result);

				// 행이 존재하면 로그인 성공
				if (row) {
					// 결과 세트 해제
					mysql_free_result(result);

					// 로그인한 사용자의 정보를 다시 가져오는 SQL 쿼리 생성
					string whoLoginquery = "SELECT Name, NO FROM MINIGAME.customer_table WHERE ID = '" + InputId + "'";

					// SQL 쿼리를 실행하고 성공 여부 확인
					if (mysql_query(&Conn, whoLoginquery.c_str()) == 0) {
						// 결과를 저장
						MYSQL_RES* result = mysql_store_result(&Conn);

						// 결과가 있을 경우
						if (result != NULL) {
							// 첫 번째 행을 가져옴
							MYSQL_ROW row = mysql_fetch_row(result);

							// 행이 존재하면 환영 메시지 출력
							if (row != NULL) {
								cout << "환영합니다! " << row[0] << "님!\n"; //row[0]에는 name이 저장되어 있음
								MemberNo = row[1]; //row[1]에는 No가 저장되어 있음
								break;
							}
						}

						// 결과 세트 해제
						mysql_free_result(result);
					}

					// 1초 대기
					Sleep(1000);
				}
				else {
					// 비밀번호 실패 최대 5번 가능
					missing += 1;
					cout << "비밀번호가 옳지않습니다." << endl;
					cout << "현재 비밀번호" << missing << "회 오류입니다." << endl;
					if (missing < 5) {
						continue;
					}
					cout << "로그인 5회 실패시 로그인이 제한됩니다." << endl;

					//결과 세트 해체
					mysql_free_result(result);

					//프로그램 종료
					cout << "\n";
					cout << "사유 : 로그인 5회 실패" << endl;
					cout << "\n\n\n\n";
					exit(0);

				}

			}
		}
		else {
			// 쿼리 실행중 오류 발생
			cout << "SQL query error: " << mysql_error(&Conn);
		}
	}
	cout << "\n";

	///////////////////////////지뢰찾기 기록 추가///////////////////////////
	int Test_mine_Score;
	string Test_mine_Player_ID=InputId;

	cout << "지뢰 찾기 점수 입력>>";
	cin >> Test_mine_Score;
	cout << "\n";
	int mine_record_No = 0; // 앞으로 회원이 등록되면 NO(회원번호)로 회원을 파악

	// 고객 테이블의 레코드 개수를 가져오기 위한 SQL 쿼리 생성
	string MineCountQuery = "SELECT COUNT(*) FROM minesweeper_game";
	//고객 테이블에 있는 모든 칼럼을 검색해주세요

	// SQL 쿼리 실행
	if (mysql_query(&Conn, MineCountQuery.c_str()) != 0) {
		// SQL 쿼리 실행 중 오류 발생 시 오류 메시지 출력
		fprintf(stderr, "SQL 문 실행 오류: %s\n", mysql_error(&Conn));
	}
	else {
		// SQL 쿼리 실행이 성공한 경우 결과를 가져오기
		MYSQL_RES* result = mysql_use_result(&Conn);

		// 결과 행 가져오기
		MYSQL_ROW row = mysql_fetch_row(result);

		// 결과 행이 존재하는 경우
		if (row != NULL) {
			// 레코드 개수를 정수로 변환하여 No 변수에 저장
			mine_record_No = atoi(row[0]); //atoi()통해 정수로 반환

			// 결과 세트 해제
			mysql_free_result(result);
		}
	}
	//가입 인원 +1 =No   ex) 고객테이블에 5명의 고객이 존재하면 다음 회원부터는 NO.6 지정
	mine_record_No += 2;

	string mineinsertQuery = "INSERT INTO `MINIGAME`.`minesweeper_game` (`No`, `Score`, `Player_ID`) VALUES ('" + to_string(mine_record_No) + "','" + to_string(Test_mine_Score) + "', '" + Test_mine_Player_ID + "')";

	if (mysql_query(&Conn, mineinsertQuery.c_str()) != 0) {
		fprintf(stderr, "Mysql query error:%s", mysql_error(&Conn));
	}

	cout << "\n";

	///////////////////////////타자게임 기록 추가///////////////////////////

	int Test_typing_Score;
	string Test_typing_Player_ID = InputId;;

	cout << "타자 게임 점수 입력>>";
	cin >> Test_typing_Score;
	cout << "\n";

	int typing_record_No = 0; // 앞으로 회원이 등록되면 NO(회원번호)로 회원을 파악

	// 고객 테이블의 레코드 개수를 가져오기 위한 SQL 쿼리 생성
	string TypingCountQuery = "SELECT COUNT(*) FROM typing_word_game";
	//고객 테이블에 있는 모든 칼럼을 검색해주세요

	// SQL 쿼리 실행
	if (mysql_query(&Conn, TypingCountQuery.c_str()) != 0) {
		// SQL 쿼리 실행 중 오류 발생 시 오류 메시지 출력
		fprintf(stderr, "SQL 문 실행 오류: %s\n", mysql_error(&Conn));
	}
	else {
		// SQL 쿼리 실행이 성공한 경우 결과를 가져오기
		MYSQL_RES* result = mysql_use_result(&Conn);

		// 결과 행 가져오기
		MYSQL_ROW row = mysql_fetch_row(result);

		// 결과 행이 존재하는 경우
		if (row != NULL) {
			// 레코드 개수를 정수로 변환하여 No 변수에 저장
			typing_record_No = atoi(row[0]); //atoi()통해 정수로 반환

			// 결과 세트 해제
			mysql_free_result(result);
		}
	}
	//가입 인원 +1 =No   ex) 고객테이블에 5명의 고객이 존재하면 다음 회원부터는 NO.6 지정
	typing_record_No += 2;

	string typinginsertQuery = "INSERT INTO `MINIGAME`.`typing_word_game` (`No`, `Score`, `Player_ID`) VALUES ('" + to_string(typing_record_No) + "','" + to_string(Test_typing_Score) + "', '" + Test_typing_Player_ID + "')";

	if (mysql_query(&Conn, typinginsertQuery.c_str()) != 0) {
		fprintf(stderr, "Mysql query error:%s", mysql_error(&Conn));
	}
	
	cout << "\n";

	///////////////////////////스코어 보드 확인///////////////////////////

	cout << "지뢰찾기 기록"<<endl;

	// 지뢰 찾기 테이블 출력 쿼리 요청
	const char* mineprintquery = "SELECT * FROM minesweeper_game ORDER BY Score ASC";
	
	int mine_stat;
	mine_stat = mysql_query(&Conn, mineprintquery);

	if (mine_stat != 0) {
		fprintf(stderr, "Mysql query error:%s\n", mysql_error(ConnPtr));
		return 0;
	}

	Result = mysql_store_result(&Conn); //MySQL에서 실행한 쿼리의 결과 집합을 Result에 저장

	cout << "등수 ";
	cout << "기록 ";
	cout << "아이디"<<endl;
	int mine_rank = 1;


	// 반복문 내에서 Row 변수를 사용하여 결과 집합에서 한 행씩 데이터를 처리이때, NULL을 만날 때까지 계속 반복
	while ((Row = mysql_fetch_row(Result)) != NULL)
	{
		cout << mine_rank<<"     ";
		cout << Row[0] << "  ";
		cout << Row[1] << endl;
		mine_rank++;
	}

	cout << "\n";

	cout << "타자게임 기록"<<endl;

	// 타자 게임 테이블 출력 쿼리 요청
	const char* typingprintquery = "SELECT * FROM typing_word_game ORDER BY Score DESC";

	int type_stat;
	type_stat = mysql_query(&Conn, typingprintquery);

	if (type_stat != 0) {
		fprintf(stderr, "Mysql query error:%s\n", mysql_error(ConnPtr));
		return 0;
	}

	Result = mysql_store_result(&Conn); //MySQL에서 실행한 쿼리의 결과 집합을 Result에 저장

	cout << "등수 ";
	cout << "기록 ";
	cout << "아이디"<<endl;
	int typing_rank = 1;

	// 반복문 내에서 Row 변수를 사용하여 결과 집합에서 한 행씩 데이터를 처리이때, NULL을 만날 때까지 계속 반복
	while ((Row = mysql_fetch_row(Result)) != NULL)
	{
		cout << typing_rank << "     ";
		cout << Row[0] << "  ";
		cout << Row[1] << endl;
		typing_rank++;
	}
	
	cout << "\n";



	mysql_free_result(Result);// MySQL C API에서 사용한 메모리를 해제하는 함수
	mysql_close(ConnPtr); // MySQL 데이터베이스 연결을 닫는 함수
}