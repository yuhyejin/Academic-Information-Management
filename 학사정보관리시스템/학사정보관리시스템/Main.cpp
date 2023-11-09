#include <iostream> //입출력 헤더 파일
#include <mysql.h> //mysql 헤더 파일
#include <string> //string 헤더 파일
#include <stdlib.h> //c표준 유틸리티 함수들을 모아 놓은 헤더 파일

#pragma comment(lib, "libmysql.lib") //mysql 라이브러리를 링크시켜준다.
using namespace std;

class st_info {     // 학생정보
private: //캡슐화
	string name;    // 학생이름
	char* stno;    // 학생학번(ID)
	string major;   // 전공
	char* stpw;    // 학생 비밀번호
public: //외부클래스에서 st_info의 멤버변수들에 접근하기 위해 만들어진 함수
	void setStno(char* stno) {
		this->stno = stno;
	}
	char* getStno() {
		return stno;
	}
	void setMajor(string major) {
		this->major = major;
	}
	string getMajor() {
		return major;
	}
	void setStpw(char* stpw) {
		this->stpw = stpw;
	}
	char* getStpw() {
		return stpw;
	}
};

class pf_info {     // 교수정보
private: //캡슐화
	char pfname[20];    // 교수이름
	char* pfno;      // 교수ID
	char* pfpw;      // 교수 비밀번호
	char sb_name[20];   // 과목명
public: //외부클래스에서 pf_info의 멤버변수들에 접근하기 위해 만들어진 함수
	void setPfno(char* pfno) {
		this->pfno = pfno;
	}
	char* getPfno() {
		return pfno;
	}
	void setPfpw(char* pfpw) {
		this->pfpw = pfpw;
	}
	char* getPfpw() {
		return pfpw;
	}
};

class st_score {    // 학생성적
private: //캡슐화
	int sno;        // 번호
	char* stno;      // 학생학번(ID)
	string complete;    // 이수구분
	char* session;   // 이수학기 ( ex)1-1, 1-2 형태)
	char* sb_name;   // 과목
	char* score;     // 등급
	int grade;      // 학점 ( ex) 3학점, 2학점)
	string pfno;    // 교수ID
public: //외부클래스에서 st_score의 멤버변수들에 접근하기 위해 만들어진 함수
	void setSession(char* session) {
		this->session = session;
	}
	char* getSession() {
		return session;
	}
	void setSb_name(char* sb_name) {
		this->sb_name = sb_name;
	}
	char* getSb_name() {
		return sb_name;
	}
	void setScore(char* score) {
		this->score = score;
	}
	char* getScore() {
		return score;
	}
};


class DB {
public:
	string scor[10] = {};   // DB에서 학생성적을 받기 위한 배열
	string sb_na[10] = {};  //  DB에서 과목명을 받기 위한 배열
	string com[10] = {};    // DB에서 이수구분을 받기 위한 배열
	string grad[10] = {};   // DB에서 학점을 받기 위한 배열

	int liberal_re;      //DB에서 교양필수 취득학점을 받기 위한 변수
	int liberal_sel;     //DB에서 교양선택 취득학점을 받기 위한 변수
	int major;       //DB에서 주전공 취득학점을 받기 위한 변수
	int score_sum;        //DB에서 총졸업 취득학점을 받기 위한 변수
	int free_major;      //DB에서 자유전공 취득학점을 받기 위한 변수

	MYSQL Conn;
	MYSQL* ConnPtr = NULL;  // MYSQL Handle
	MYSQL_RES* Result;      // Query 결과 담는 구조체 포인터
	MYSQL_ROW Row; // Query 결과로 나온 열 정보를 담는 구조체
	int Stat;            // Query 성공 여부 받는 변수
	void a() { //데이터베이스 언제든 연결할 수 있도록 객체화

		mysql_init(&Conn);      // MYSQL initiated
								// DB 연결
		ConnPtr = mysql_real_connect(&Conn, "localhost", "root", "soft602", "syst", 3306, (char*)NULL, 0);

		// Connection Status
		if (ConnPtr == NULL)
		{
			cout << stderr << "Mysql Connection Error : " << mysql_error(&Conn) << "\n";
		}
		// DB 연결 해제
		//mysql_close(ConnPtr);
	}

	boolean login_st(string stno, string stpw) //학번과 맞는 비밀번호가 들어가있는지 - 학생
	{
		boolean log;
		a(); //DB호출
		char buff[255];     // query문을 담을 변수
		sprintf_s(buff, "select * from st_info"); // st_info 테이블 select하는 명렁문
		mysql_query(ConnPtr, "set names euckr");    // 한글이 깨지지 않도록 요청
		Stat = mysql_query(ConnPtr, buff);      // buff의 내용을 ConnPtr을 통해서 query 요청
		Result = mysql_store_result(ConnPtr);
		string id[4];   // DB에서 학번을 받기 위한 배열
		string pw[4];   // DB에서 비번을 받기 위한 배열
		int i = 0;
		while ((Row = mysql_fetch_row(Result)) != NULL) {       // mysql_fetch_row()를 사용해 Result의 값들을 한 row씩 Row에 저장한다.
			id[i] = Row[1];     // DB st_info테이블의 column명 2번째에 있는 snto 데이터들을 id에 저장
			pw[i] = Row[3];     // DB st_info테이블의 column명 4번째에 있는 stpw 데이터들을 pw에 저장
			i++;
		}
		for (int i = 0; i < 4; i++) {   // DB st_info에 들어있는 학생수 만큼 for문을 돌림
			if (stno == id[i] && stpw == pw[i]) {   // 입력받은 학번, 비밀번호가 DB st_info에 있는 데이터와 같은지 확인
				log = true;     // 있으면 true 반환
				break;
			}
			else {
				log = false;    // 없으면 false 반환
			}
		}return log;
	}
	boolean login_pf(string pfno, string pfpw) // 학번과 맞는 비밀번호가 들어가있는지 - 교수
	{
		boolean log;
		a();    // DB호출
		char buff[255];     // query문을 담을 변수
		sprintf_s(buff, "select pfno, pfpw from pf_info");  // pf_info 테이블 select하는 명렁문
		mysql_query(ConnPtr, "set names euckr");        // 한글이 깨지지 않도록 요청
		Stat = mysql_query(ConnPtr, buff);      // buff의 내용을 ConnPtr을 통해서 query 요청
		Result = mysql_store_result(ConnPtr);
		string id[60];       // DB에서 교수ID을 받기 위한 배열
		string pw[60];       // DB에서 비번을 받기 위한 배열
		int i = 0;
		while ((Row = mysql_fetch_row(Result)) != NULL) {       // mysql_fetch_row()를 사용해 Result의 값들을 한 row씩 Row에 저장한다.
			id[i] = Row[0];     // DB pf_info테이블의 pfno 데이터들을 id에 저장
			pw[i] = Row[1];     // DB pf_info테이블의 pfpw 데이터들을 pw에 저장
			i++;
		}
		for (int i = 0; i < 60; i++) {      // DB pf_info에 들어있는 교수 수 만큼 for문을 돌림
			if (pfno == id[i] && pfpw == pw[i]) {       // 입력받은 교수ID, 비밀번호가 DB pf_info에 있는 데이터와 같은지 확인
				log = true;     // 있으면 true 반환
				break;
			}
			else {
				log = false;        // 없으면 false 반환
			}
		}return log;
	}

	boolean score_check(char sb_name[30], char stno[30]) { //과목명과 학번이 맞게 들어갔는지

		boolean check;
		a();    // DB호출
		char buff[255];     // query문을 담을 변수
		sprintf_s(buff, "Select sb_name , stno from st_score");     // st_score 테이블 select하는 명렁문
		mysql_query(ConnPtr, "set names euckr");        // 한글이 깨지지 않도록 요청
		Stat = mysql_query(ConnPtr, buff);      // buff의 내용을 ConnPtr을 통해서 query 요청
		Result = mysql_store_result(ConnPtr);
		string name[180];       // DB에서 과목명을 받기 위한 배열
		string no[180];         // DB에서 학번을 받기 위한 배열
		int i = 0;
		while ((Row = mysql_fetch_row(Result)) != NULL) {       // mysql_fetch_row()를 사용해 Result의 값들을 한 row씩 Row에 저장한다.
			name[i] = Row[0];   // DB st_score테이블의 sb_name 데이터들을 name에 저장
			no[i] = Row[1];     // DB st_score테이블의 stno 데이터들을 no에 저장
			i++;
		}
		for (int i = 0; i < 180; i++) {     // DB st_score에 들어있는 과목수 만큼 for문을 돌림
			if (sb_name == name[i]) {       // 입력받은 과목명이 DB st_score에 있는 데이터와 같은지 확인
				if (stno == no[i]) {        // 입력받은 학번이 DB st_score에 있는 테이터와 같은지 확인
					check = true;   // 있으면 true 반환
					break;
				}
			}
			else {
				check = false;      // 없으면 false 반환
			}
		}
		return check;
	}

	boolean score_show(char stno[30], char session[50]) { //학생 성적 확인

		for (int i = 0; i < 10; i++) {      // 성적확인을 여러번 하기 위해서 돌 때마다 값들을 초기화 시킴
			scor[i] = "";   // DB에서 등급을 받기 위한 배열
			sb_na[i] = "";  // DB에서 과목명을 받기 위한 배열
			com[i] = "";    // DB에서 이수구분을 받기 위한 배열
			grad[i] = "";   // DB에서 학점을 받기 위한 배열
		}
		a();    // DB호출
		char buff[255];     // query문을 담을 변수
		sprintf_s(buff, "Select sb_name, score, complete, grade  from st_score where stno = '%s' and session = '%s'", stno, session);       // st_score 테이블 select하는 명렁문
		mysql_query(ConnPtr, "set names euckr");    // 한글이 깨지지 않도록 요청
		Stat = mysql_query(ConnPtr, buff);      // buff의 내용을 ConnPtr을 통해서 query 요청
		Result = mysql_store_result(ConnPtr);   // mysql_store_result() 함수를 통해 쿼리 전체의 결과 값을 Result 변수에 저장

		int i = 0;
		while ((Row = mysql_fetch_row(Result)) != NULL) {    // mysql_fetch_row()를 사용해 Result의 값들을 한 row씩 Row에 저장한다.
			sb_na[i] = Row[0];      // DB st_score테이블의 sb_name 데이터들을 name에 저장
			scor[i] = Row[1];       // DB st_score테이블의 score 데이터들을 scor에 저장
			com[i] = Row[2];        // DB st_score테이블의 complete 데이터들을 com에 저장
			grad[i] = Row[3];       // DB st_score테이블의 grade 데이터들을 grad에 저장
			i++;
		}

		return true;
	}

	void score_add(char sb_name[30], char stno[30], char score[30]) { //성적 입력

		a();    // DB호출
		char buff[255];     // query문을 담을 변수
		sprintf_s(buff, "update st_score set score = '%s' where sb_name ='%s' and stno = '%s'", score, sb_name, stno);      // st_score 테이블 select하는 명렁문
		mysql_query(ConnPtr, "set names euckr");        // 한글이 깨지지 않도록 요청
		mysql_query(ConnPtr, buff);     // buff의 내용을 ConnPtr을 통해서 query 요청
		Result = mysql_store_result(ConnPtr);       // mysql_store_result() 함수를 통해 쿼리 전체의 결과 값을 Result 변수에 저장
	}

	boolean gradu_sum1(char stno[30]) { //교양필수 합계

		a();    // DB호출
		char buff[255];     // query문을 담을 변수
		sprintf_s(buff, "Select sum(grade) from st_score where stno = '%s' and complete = '교양필수'", stno);       // st_score 테이블 select하는 명렁문
		mysql_query(ConnPtr, "set names euckr");        // 한글이 깨지지 않도록 요청
		Stat = mysql_query(ConnPtr, buff);      // buff의 내용을 ConnPtr을 통해서 query 요청
		Result = mysql_store_result(ConnPtr);   // mysql_store_result() 함수를 통해 쿼리 전체의 결과 값을 Result 변수에 저장

		while ((Row = mysql_fetch_row(Result)) != NULL) {       // mysql_fetch_row()를 사용해 Result의 값들을 한 row씩 Row에 저장한다.
			liberal_re = atoi(Row[0]);      // DB st_score테이블의 교양필수 데이터들을 liberal_re에 저장
		}

		return true;
	}


	boolean gradu_sum2(char stno[30]) { //교양선택 합계

		a();    // DB호출
		char buff[255];     // query문을 담을 변수
		sprintf_s(buff, "Select sum(grade) from st_score where stno = '%s' and complete = '교양선택'", stno);   // st_score 테이블 select하는 명렁문
		mysql_query(ConnPtr, "set names euckr");    // 한글이 깨지지 않도록 요청
		Stat = mysql_query(ConnPtr, buff);      // buff의 내용을 ConnPtr을 통해서 query 요청
		Result = mysql_store_result(ConnPtr);   // mysql_store_result() 함수를 통해 쿼리 전체의 결과 값을 Result 변수에 저장

		while ((Row = mysql_fetch_row(Result)) != NULL) {       // mysql_fetch_row()를 사용해 Result의 값들을 한 row씩 Row에 저장한다.
			liberal_sel = atoi(Row[0]);     // DB st_score테이블의 교양선택 데이터들을 liberal_sel에 저장
		}

		return true;
	}


	boolean gradu_sum3(char stno[30]) { //전공 합계

		a();    // DB호출
		char buff[255];     // query문을 담을 변수
		sprintf_s(buff, "Select sum(grade) from st_score where stno = '%s' and complete in ('전공선택','전공필수') ", stno);        // st_score 테이블 select하는 명렁문
		mysql_query(ConnPtr, "set names euckr");        // 한글이 깨지지 않도록 요청
		Stat = mysql_query(ConnPtr, buff);      // buff의 내용을 ConnPtr을 통해서 query 요청
		Result = mysql_store_result(ConnPtr);   // mysql_store_result() 함수를 통해 쿼리 전체의 결과 값을 Result 변수에 저장

		while ((Row = mysql_fetch_row(Result)) != NULL) {       // mysql_fetch_row()를 사용해 Result의 값들을 한 row씩 Row에 저장한다.
			major = atoi(Row[0]);       // DB st_score테이블의 전공선택+전공필수 데이터들을 major에 저장
		}

		return true;
	}

	boolean gradu_sum4(char stno[30]) { //자유전공 합계

		a();    // DB호출
		char buff[255];     // query문을 담을 변수
		sprintf_s(buff, "Select sum(grade) from st_score where stno = '%s' and complete = '연계전공'", stno);       // st_score 테이블 select하는 명렁문
		mysql_query(ConnPtr, "set names euckr");        // 한글이 깨지지 않도록 요청
		Stat = mysql_query(ConnPtr, buff);      // buff의 내용을 ConnPtr을 통해서 query 요청
		Result = mysql_store_result(ConnPtr);       // mysql_store_result() 함수를 통해 쿼리 전체의 결과 값을 Result 변수에 저장

		while ((Row = mysql_fetch_row(Result)) != NULL) {       // mysql_fetch_row()를 사용해 Result의 값들을 한 row씩 Row에 저장한다.
			free_major = atoi(Row[0]);      // DB st_score테이블의 전공선택+전공필수 데이터들을 major에 저장
		}

		return true;
	}
	boolean gradu_sum5(char stno[30]) { //졸업학점 합계

		a();    // DB호출
		char buff[255];     // query문을 담을 변수
		sprintf_s(buff, "Select sum(grade) from st_score where stno = '%s'", stno);     // st_score 테이블 select하는 명렁문
		mysql_query(ConnPtr, "set names euckr");        // 한글이 깨지지 않도록 요청
		Stat = mysql_query(ConnPtr, buff);      // buff의 내용을 ConnPtr을 통해서 query 요청
		Result = mysql_store_result(ConnPtr);       // mysql_store_result() 함수를 통해 쿼리 전체의 결과 값을 Result 변수에 저장

		while ((Row = mysql_fetch_row(Result)) != NULL) {       // mysql_fetch_row()를 사용해 Result의 값들을 한 row씩 Row에 저장한다.
			score_sum = atoi(Row[0]);       // DB st_score테이블의 전공선택+전공필수 데이터들을 major에 저장
		}

		return true;
	}
};

class Main : DB {       // DB클래스 상속
public:
	st_info sti; //st_info클래스 객체화
	st_score sts;  //st_socre클래스 객체화
	pf_info pfi;  //pf_info클래스 객체화

	void start() { //시작함수
		int log;
		cout << "================ 안양대학교 학사 정보 시스템 =================" << endl << endl;
		cout << "1, 교수자 로그인" << endl << endl;
		cout << "2, 학생 로그인" << endl << endl;
		cout << "0번을 누르면 종료됩니다." << endl << endl;
		cout << "입력하시오 >> ";
		cin >> log;
		cout << endl << "==============================================================" << endl << endl;

		switch (log) {
		case 0:
			exit(0);    // 프로그램 종료
		case 1:
			login_Pf(); //교수 로그인 함수 호출
			break;
		case 2:
			login_St(); //학생 로그인 함수 호출
			break;
		default:    // 다른 숫자 입력시 start() 함수 다시 실행
			cout << "다시 입력하세요." << endl << endl << endl << endl;
			start();
		}
	}

	void login_St() { //학생 로그인 함수
		char id[30]; //학번을 사용자에게 직접 받아 stno에 넣어주기 위한 변수
		char pw[30]; //비밀번호를 사용자에게 직접 받아 stpw에 넣어주기 위한 변수
		cout << endl << "========================== 로그인 ============================" << endl;
		cout << endl << "학번(ID) : ";
		cin >> id;
		sti.setStno(id);
		cout << endl << "비밀번호 : ";
		cin >> pw;
		sti.setStpw(pw);
		cout << endl << "==============================================================" << endl << endl;

		if (login_st(sti.getStno(), sti.getStpw())) { // DB에 저장된 학생의 학번과 비번이 입력한 값과 일치할 경우
			cout << "*************************************************************" << endl << endl;
			cout << "                     로그인 되었습니다." << endl << endl;
			cout << "*************************************************************" << endl << endl << endl;
			choice_St(); //학생 옵션선택 함수 호출
		}
		else {
			cout << "*************************************************************" << endl << endl;
			cout << "등록되어있지 않은 학번이거나 비밀번호가 틀렸습니다." << endl;
			cout << endl << "*************************************************************" << endl;
			login_St(); // 다시 로그인 창으로 이동
		}
	}

	void login_Pf() { //교수 로그인 함수
		char id[30]; //교수번호를 사용자에게 직접 받아 pfno에 넣어주기 위한 변수
		char pw[30]; //비밀번호를 사용자에게 직접 받아 pfpw에 넣어주기 위한 변수
		cout << endl << "========================== 로그인 ============================" << endl;
		cout << endl << "교수자 번호 (ID) : ";
		cin >> id;
		pfi.setPfno(id);
		cout << endl << "비밀번호 : ";
		cin >> pw;
		pfi.setPfpw(pw);
		cout << endl << "==============================================================" << endl << endl;
		if (login_pf(pfi.getPfno(), pfi.getPfpw())) { //DB에 저장된 교수아이디와 비번이 입력한 값과 일치할 경우
			cout << "*************************************************************" << endl << endl;
			cout << "                     로그인 되었습니다." << endl << endl;
			cout << "*************************************************************" << endl << endl << endl << endl;
			choice_Pf(); //교수 옵션선택 함수 호출
		}
		else {
			cout << "*************************************************************" << endl << endl;
			cout << "등록되어있지 않은 교수번호거나 비밀번호가 틀렸습니다." << endl << endl;
			cout << "*************************************************************" << endl << endl << endl << endl;
			login_Pf(); // 다시 로그인 창으로 이동
		}
	}

	void choice_Pf() { //교수 옵션선택 함수
		int cho;
		cout << endl << "========================== 교수님 ============================" << endl << endl;
		cout << "1, 성적입력" << endl << endl;
		cout << "2, 로그아웃" << endl << endl;
		cout << "입력하시오 >> ";
		cin >> cho;
		cout << endl << endl << "==============================================================" << endl << endl;

		switch (cho) {
		case 1:
			update_Score(); //성적 입력 함수 호출
			break;
		case 2:
			logout(); //로그아웃 함수 호출
			start();
			break;
		default:    // 다른 숫자 입력 시 다시 교수 옵션창으로 이동
			cout << "1번 2번 중에서만 입력해주세요." << endl << endl << endl << endl;
			choice_Pf(); // 다시 옵션선택 창으로 이동
		}
	}

	void choice_St() { //학생 옵션선택 함수
		int cho;
		cout << endl << "=========================== 학생 =============================" << endl << endl;
		cout << "1, 성적관리" << endl << endl;
		cout << "2, 졸업학점" << endl << endl;
		cout << "3, 로그아웃" << endl << endl;
		cout << "입력하시오 >> ";
		cin >> cho;
		cout << endl << endl << "==============================================================" << endl << endl;

		switch (cho) {
		case 1:
			score_see(); //성적 확인 함수 호출
			break;
		case 2:
			gradu(); //졸업학점 함수 호출
			break;
		case 3:
			logout(); //로그아웃 함수 호출
			start();
			break;
		default:    // 다른 숫자 입력 시 다시 학생 옵션창으로 이동
			cout << "1번 2번 3번 중에서만 입력해주세요." << endl << endl << endl << endl;
			choice_St();
		}
	}

	void update_Score() { //성적 입력 함수
		int add;
		char sbname[30]; //성적 입력할 과목을 사용자에게 직접 받아 sbname에 넣어주기 위한 변수
		char stno[30];  //성적 입력할 학번을 사용자에게 직접 받아 sbno에 넣어주기 위한 변수
		char score[30];  //입력할 성적(등급)을 사용자에게 직접 받아 score에 넣어주기 위한 변수
		cout << endl << "************************** 성적 추가 ************************" << endl;
		cout << endl << "추가할 개수 : "; //성적 추가를 몇개 추가할지 입력받는다.
		cin >> add;

		for (int j = 0; j < add; j++) {
			cout << " \n";
			cout << " => 성적 입력할 과목 : ";
			cin >> sbname;
			sts.setSb_name(sbname);
			cout << " \n";
			cout << " => 성적 입력할 학생학번 : ";
			cin >> stno;
			sti.setStno(stno);
			cout << " \n";
			cout << " => 성적 : ";
			cin >> score;
			sts.setScore(score);

			if (score_check(sts.getSb_name(), sti.getStno())) {// 입력받은 과목명, 학번이 DB에 있으면 if문 실행
				score_add(sts.getSb_name(), sti.getStno(), sts.getScore());// 과목명, 학번, 성적을 받아서 DB 함수인 score_add()에 보냄
				cout << "*************************************************************\n\n";
				cout << "                      추가되었습니다." << endl << endl;
				cout << "*************************************************************\n\n\n\n";
			}
			else {  // 입력받은 과목명, 학번이 DB에 없으면 else문 출력
				cout << "*************************************************************\n\n";
				cout << "           존재하지 않는 학번이거나 과목입니다.\n";
				cout << "                  다시입력해주세요.\n\n";
				cout << "*************************************************************" << endl << endl << endl << endl;
				j--;
			}
		}
		choice_Pf();    // 성적 추가 완료하면 교수옵션 선택 메뉴로 이동
	}

	void score_see() { //(학생용)성적확인 함수
	stop:
		char session[30]; //입력받을 학년,학기를
		cout << endl << "학년, 학기 : ";
		cin >> session;
		sts.setSession(session); // 학년, 학기를 받아옴 (1-1, 1-2 형식)

		if (score_show(sti.getStno(), sts.getSession())) {    // 로그인에서 받은 학번과 입력받은 학기를 DB함수인 score_show()에 넘겨 학기와 필요한 정보들을 검색함
															  // for문을 돌려 com[i]로 출력하려 했지만 과목 길이들이 달라 UI가 지저분해져서 3-2학기 과목들을 기준으로 맞춤.
			cout << endl << " ┌────────────────────────────────────────────────────────────────────────┐\n";
			cout << " │                                  " << sts.getSession() << " 성적                              │\n";
			cout << " ├────────────┬───────────────────────────────────────┬──────────┬────────┤\n";
			cout << " │  이수구분  │                   과목명              │    학점  │  등급  │\n";
			cout << " ├────────────┼───────────────────────────────────────┼──────────┼────────┤\n";
			cout << " │  " << com[0] << "  │             " << sb_na[0] << "          │     " << grad[0] << "    │    " << scor[0] << "   │\n";
			cout << " ├────────────┼───────────────────────────────────────┼──────────┼────────┤\n";
			cout << " │  " << com[1] << "  │             " << sb_na[1] << "          │     " << grad[1] << "    │    " << scor[1] << "   │\n";
			cout << " ├────────────┼───────────────────────────────────────┼──────────┼────────┤\n";
			cout << " │  " << com[2] << "  │                " << sb_na[2] << "             │     " << grad[2] << "    │    " << scor[2] << "   │\n";
			cout << " ├────────────┼───────────────────────────────────────┼──────────┼────────┤\n";
			cout << " │  " << com[3] << "  │           " << sb_na[3] << "        │     " << grad[3] << "    │    " << scor[3] << "   │\n";
			cout << " ├────────────┼───────────────────────────────────────┼──────────┼────────┤\n";
			cout << " │  " << com[4] << "  │                " << sb_na[4] << "         │     " << grad[4] << "    │    " << scor[4] << "   │\n";
			cout << " ├────────────┼───────────────────────────────────────┼──────────┼────────┤\n";
			cout << " │  " << com[5] << "  │        " << sb_na[5] << "     │     " << grad[5] << "    │    " << scor[5] << "   │\n";
			cout << " ├────────────┼───────────────────────────────────────┼──────────┼────────┤\n";
			cout << " │      " << com[6] << "      │                   " << sb_na[6] << "                    │     " << grad[6] << "     │    " << scor[6] << "    │\n";
			cout << " ├────────────┼───────────────────────────────────────┼──────────┼────────┤\n";
			cout << " │      " << com[7] << "      │                   " << sb_na[7] << "                    │     " << grad[7] << "     │    " << scor[7] << "    │\n";
			cout << " ├────────────┼───────────────────────────────────────┼──────────┼────────┤\n";
			cout << " │      " << com[8] << "      │                   " << sb_na[8] << "                    │     " << grad[8] << "     │    " << scor[8] << "    │\n";
			cout << " ├────────────┼───────────────────────────────────────┼──────────┼────────┤\n";
			cout << " │      " << com[9] << "      │                   " << sb_na[9] << "                    │     " << grad[9] << "     │    " << scor[9] << "    │\n";
			cout << " └────────────┴───────────────────────────────────────┴──────────┴────────┘\n" << endl << endl << endl;
			choice_St();    // 학기를 보여주면 다시 학생옵션 메뉴로 이동
		}
		else {  // 입력한 학기가 DB에 존재하지 않으면 실행
			cout << endl << "*************************************************************\n\n";
			cout << "                존재하지 않는 학기입니다.\n";
			cout << "                  다시입력해주세요.\n\n";
			cout << "*************************************************************" << endl << endl << endl << endl;
			goto stop;
		}

	}

	void gradu() { //(학생용)졸업학점 확인 함수

		gradu_sum1(sti.getStno());
		gradu_sum2(sti.getStno());
		gradu_sum3(sti.getStno());
		gradu_sum4(sti.getStno());
		gradu_sum5(sti.getStno());

		// 잔여학점 계산
		int re, sel, majorr, free_Major, score_Sum;
		re = 14 - liberal_re;
		sel = 25 - liberal_sel;
		majorr = 48 - major;
		free_Major = 42 - free_major;
		score_Sum = 130 - score_sum;

		cout << "┌──────────┬───────────────────────────┬──────────────────────────┬────────────┐ \n";
		cout << "│          │           교양            │             전공         │            │ \n";
		cout << "├──────────┼─────────────┬─────────────┼─────────────┬────────────┼────────────┤ \n";
		cout << "│ 이수구분 │     필수    │     선택    │    주전공   │     자유   │  졸업학점  │ \n";
		cout << "├──────────┼─────────────┼─────────────┼─────────────┼────────────┼────────────┤\n";
		cout << "│ 졸업학점 │      " << "14" << "     │      " << "25" << "     │      " << "48" << "     │     " << "42" << "     │     " << "130" << "    │ \n";
		cout << "├──────────┼─────────────┼─────────────┼─────────────┼────────────┼────────────┤ \n";
		cout << "│ 취득학점 │      " << liberal_re << "     │      " << liberal_sel << "     │      " << major << "     │      " << free_major << "    │     " << score_sum << "    │ \n";
		cout << "├──────────┼─────────────┼─────────────┼─────────────┼────────────┼────────────┤ \n";
		cout << "│ 잔여학점 │      " << re << "      │      " << sel << "      │     " << majorr << "     │     " << free_Major << "     │     " << score_Sum << "     │ \n";
		cout << "└──────────┴─────────────┴─────────────┴─────────────┴────────────┴────────────┘ \n\n\n\n";
		choice_St();    // 다시 학생옵션 메뉴로 이동

	}


	void logout() { //로그아웃 함수
		cout << endl << "*************************************************************" << endl << endl;
		cout << "                   로그아웃 되었습니다." << endl << endl;
		cout << "*************************************************************" << endl << endl << endl << endl;
	}

};

int main()
{

	Main m;
	m.start(); //시작 함수 호출

	return 0;
}