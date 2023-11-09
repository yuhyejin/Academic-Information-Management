#include <iostream> //����� ��� ����
#include <mysql.h> //mysql ��� ����
#include <string> //string ��� ����
#include <stdlib.h> //cǥ�� ��ƿ��Ƽ �Լ����� ��� ���� ��� ����

#pragma comment(lib, "libmysql.lib") //mysql ���̺귯���� ��ũ�����ش�.
using namespace std;

class st_info {     // �л�����
private: //ĸ��ȭ
	string name;    // �л��̸�
	char* stno;    // �л��й�(ID)
	string major;   // ����
	char* stpw;    // �л� ��й�ȣ
public: //�ܺ�Ŭ�������� st_info�� ��������鿡 �����ϱ� ���� ������� �Լ�
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

class pf_info {     // ��������
private: //ĸ��ȭ
	char pfname[20];    // �����̸�
	char* pfno;      // ����ID
	char* pfpw;      // ���� ��й�ȣ
	char sb_name[20];   // �����
public: //�ܺ�Ŭ�������� pf_info�� ��������鿡 �����ϱ� ���� ������� �Լ�
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

class st_score {    // �л�����
private: //ĸ��ȭ
	int sno;        // ��ȣ
	char* stno;      // �л��й�(ID)
	string complete;    // �̼�����
	char* session;   // �̼��б� ( ex)1-1, 1-2 ����)
	char* sb_name;   // ����
	char* score;     // ���
	int grade;      // ���� ( ex) 3����, 2����)
	string pfno;    // ����ID
public: //�ܺ�Ŭ�������� st_score�� ��������鿡 �����ϱ� ���� ������� �Լ�
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
	string scor[10] = {};   // DB���� �л������� �ޱ� ���� �迭
	string sb_na[10] = {};  //  DB���� ������� �ޱ� ���� �迭
	string com[10] = {};    // DB���� �̼������� �ޱ� ���� �迭
	string grad[10] = {};   // DB���� ������ �ޱ� ���� �迭

	int liberal_re;      //DB���� �����ʼ� ��������� �ޱ� ���� ����
	int liberal_sel;     //DB���� ���缱�� ��������� �ޱ� ���� ����
	int major;       //DB���� ������ ��������� �ޱ� ���� ����
	int score_sum;        //DB���� ������ ��������� �ޱ� ���� ����
	int free_major;      //DB���� �������� ��������� �ޱ� ���� ����

	MYSQL Conn;
	MYSQL* ConnPtr = NULL;  // MYSQL Handle
	MYSQL_RES* Result;      // Query ��� ��� ����ü ������
	MYSQL_ROW Row; // Query ����� ���� �� ������ ��� ����ü
	int Stat;            // Query ���� ���� �޴� ����
	void a() { //�����ͺ��̽� ������ ������ �� �ֵ��� ��üȭ

		mysql_init(&Conn);      // MYSQL initiated
								// DB ����
		ConnPtr = mysql_real_connect(&Conn, "localhost", "root", "soft602", "syst", 3306, (char*)NULL, 0);

		// Connection Status
		if (ConnPtr == NULL)
		{
			cout << stderr << "Mysql Connection Error : " << mysql_error(&Conn) << "\n";
		}
		// DB ���� ����
		//mysql_close(ConnPtr);
	}

	boolean login_st(string stno, string stpw) //�й��� �´� ��й�ȣ�� ���ִ��� - �л�
	{
		boolean log;
		a(); //DBȣ��
		char buff[255];     // query���� ���� ����
		sprintf_s(buff, "select * from st_info"); // st_info ���̺� select�ϴ� ����
		mysql_query(ConnPtr, "set names euckr");    // �ѱ��� ������ �ʵ��� ��û
		Stat = mysql_query(ConnPtr, buff);      // buff�� ������ ConnPtr�� ���ؼ� query ��û
		Result = mysql_store_result(ConnPtr);
		string id[4];   // DB���� �й��� �ޱ� ���� �迭
		string pw[4];   // DB���� ����� �ޱ� ���� �迭
		int i = 0;
		while ((Row = mysql_fetch_row(Result)) != NULL) {       // mysql_fetch_row()�� ����� Result�� ������ �� row�� Row�� �����Ѵ�.
			id[i] = Row[1];     // DB st_info���̺��� column�� 2��°�� �ִ� snto �����͵��� id�� ����
			pw[i] = Row[3];     // DB st_info���̺��� column�� 4��°�� �ִ� stpw �����͵��� pw�� ����
			i++;
		}
		for (int i = 0; i < 4; i++) {   // DB st_info�� ����ִ� �л��� ��ŭ for���� ����
			if (stno == id[i] && stpw == pw[i]) {   // �Է¹��� �й�, ��й�ȣ�� DB st_info�� �ִ� �����Ϳ� ������ Ȯ��
				log = true;     // ������ true ��ȯ
				break;
			}
			else {
				log = false;    // ������ false ��ȯ
			}
		}return log;
	}
	boolean login_pf(string pfno, string pfpw) // �й��� �´� ��й�ȣ�� ���ִ��� - ����
	{
		boolean log;
		a();    // DBȣ��
		char buff[255];     // query���� ���� ����
		sprintf_s(buff, "select pfno, pfpw from pf_info");  // pf_info ���̺� select�ϴ� ����
		mysql_query(ConnPtr, "set names euckr");        // �ѱ��� ������ �ʵ��� ��û
		Stat = mysql_query(ConnPtr, buff);      // buff�� ������ ConnPtr�� ���ؼ� query ��û
		Result = mysql_store_result(ConnPtr);
		string id[60];       // DB���� ����ID�� �ޱ� ���� �迭
		string pw[60];       // DB���� ����� �ޱ� ���� �迭
		int i = 0;
		while ((Row = mysql_fetch_row(Result)) != NULL) {       // mysql_fetch_row()�� ����� Result�� ������ �� row�� Row�� �����Ѵ�.
			id[i] = Row[0];     // DB pf_info���̺��� pfno �����͵��� id�� ����
			pw[i] = Row[1];     // DB pf_info���̺��� pfpw �����͵��� pw�� ����
			i++;
		}
		for (int i = 0; i < 60; i++) {      // DB pf_info�� ����ִ� ���� �� ��ŭ for���� ����
			if (pfno == id[i] && pfpw == pw[i]) {       // �Է¹��� ����ID, ��й�ȣ�� DB pf_info�� �ִ� �����Ϳ� ������ Ȯ��
				log = true;     // ������ true ��ȯ
				break;
			}
			else {
				log = false;        // ������ false ��ȯ
			}
		}return log;
	}

	boolean score_check(char sb_name[30], char stno[30]) { //������ �й��� �°� ������

		boolean check;
		a();    // DBȣ��
		char buff[255];     // query���� ���� ����
		sprintf_s(buff, "Select sb_name , stno from st_score");     // st_score ���̺� select�ϴ� ����
		mysql_query(ConnPtr, "set names euckr");        // �ѱ��� ������ �ʵ��� ��û
		Stat = mysql_query(ConnPtr, buff);      // buff�� ������ ConnPtr�� ���ؼ� query ��û
		Result = mysql_store_result(ConnPtr);
		string name[180];       // DB���� ������� �ޱ� ���� �迭
		string no[180];         // DB���� �й��� �ޱ� ���� �迭
		int i = 0;
		while ((Row = mysql_fetch_row(Result)) != NULL) {       // mysql_fetch_row()�� ����� Result�� ������ �� row�� Row�� �����Ѵ�.
			name[i] = Row[0];   // DB st_score���̺��� sb_name �����͵��� name�� ����
			no[i] = Row[1];     // DB st_score���̺��� stno �����͵��� no�� ����
			i++;
		}
		for (int i = 0; i < 180; i++) {     // DB st_score�� ����ִ� ����� ��ŭ for���� ����
			if (sb_name == name[i]) {       // �Է¹��� ������� DB st_score�� �ִ� �����Ϳ� ������ Ȯ��
				if (stno == no[i]) {        // �Է¹��� �й��� DB st_score�� �ִ� �����Ϳ� ������ Ȯ��
					check = true;   // ������ true ��ȯ
					break;
				}
			}
			else {
				check = false;      // ������ false ��ȯ
			}
		}
		return check;
	}

	boolean score_show(char stno[30], char session[50]) { //�л� ���� Ȯ��

		for (int i = 0; i < 10; i++) {      // ����Ȯ���� ������ �ϱ� ���ؼ� �� ������ ������ �ʱ�ȭ ��Ŵ
			scor[i] = "";   // DB���� ����� �ޱ� ���� �迭
			sb_na[i] = "";  // DB���� ������� �ޱ� ���� �迭
			com[i] = "";    // DB���� �̼������� �ޱ� ���� �迭
			grad[i] = "";   // DB���� ������ �ޱ� ���� �迭
		}
		a();    // DBȣ��
		char buff[255];     // query���� ���� ����
		sprintf_s(buff, "Select sb_name, score, complete, grade  from st_score where stno = '%s' and session = '%s'", stno, session);       // st_score ���̺� select�ϴ� ����
		mysql_query(ConnPtr, "set names euckr");    // �ѱ��� ������ �ʵ��� ��û
		Stat = mysql_query(ConnPtr, buff);      // buff�� ������ ConnPtr�� ���ؼ� query ��û
		Result = mysql_store_result(ConnPtr);   // mysql_store_result() �Լ��� ���� ���� ��ü�� ��� ���� Result ������ ����

		int i = 0;
		while ((Row = mysql_fetch_row(Result)) != NULL) {    // mysql_fetch_row()�� ����� Result�� ������ �� row�� Row�� �����Ѵ�.
			sb_na[i] = Row[0];      // DB st_score���̺��� sb_name �����͵��� name�� ����
			scor[i] = Row[1];       // DB st_score���̺��� score �����͵��� scor�� ����
			com[i] = Row[2];        // DB st_score���̺��� complete �����͵��� com�� ����
			grad[i] = Row[3];       // DB st_score���̺��� grade �����͵��� grad�� ����
			i++;
		}

		return true;
	}

	void score_add(char sb_name[30], char stno[30], char score[30]) { //���� �Է�

		a();    // DBȣ��
		char buff[255];     // query���� ���� ����
		sprintf_s(buff, "update st_score set score = '%s' where sb_name ='%s' and stno = '%s'", score, sb_name, stno);      // st_score ���̺� select�ϴ� ����
		mysql_query(ConnPtr, "set names euckr");        // �ѱ��� ������ �ʵ��� ��û
		mysql_query(ConnPtr, buff);     // buff�� ������ ConnPtr�� ���ؼ� query ��û
		Result = mysql_store_result(ConnPtr);       // mysql_store_result() �Լ��� ���� ���� ��ü�� ��� ���� Result ������ ����
	}

	boolean gradu_sum1(char stno[30]) { //�����ʼ� �հ�

		a();    // DBȣ��
		char buff[255];     // query���� ���� ����
		sprintf_s(buff, "Select sum(grade) from st_score where stno = '%s' and complete = '�����ʼ�'", stno);       // st_score ���̺� select�ϴ� ����
		mysql_query(ConnPtr, "set names euckr");        // �ѱ��� ������ �ʵ��� ��û
		Stat = mysql_query(ConnPtr, buff);      // buff�� ������ ConnPtr�� ���ؼ� query ��û
		Result = mysql_store_result(ConnPtr);   // mysql_store_result() �Լ��� ���� ���� ��ü�� ��� ���� Result ������ ����

		while ((Row = mysql_fetch_row(Result)) != NULL) {       // mysql_fetch_row()�� ����� Result�� ������ �� row�� Row�� �����Ѵ�.
			liberal_re = atoi(Row[0]);      // DB st_score���̺��� �����ʼ� �����͵��� liberal_re�� ����
		}

		return true;
	}


	boolean gradu_sum2(char stno[30]) { //���缱�� �հ�

		a();    // DBȣ��
		char buff[255];     // query���� ���� ����
		sprintf_s(buff, "Select sum(grade) from st_score where stno = '%s' and complete = '���缱��'", stno);   // st_score ���̺� select�ϴ� ����
		mysql_query(ConnPtr, "set names euckr");    // �ѱ��� ������ �ʵ��� ��û
		Stat = mysql_query(ConnPtr, buff);      // buff�� ������ ConnPtr�� ���ؼ� query ��û
		Result = mysql_store_result(ConnPtr);   // mysql_store_result() �Լ��� ���� ���� ��ü�� ��� ���� Result ������ ����

		while ((Row = mysql_fetch_row(Result)) != NULL) {       // mysql_fetch_row()�� ����� Result�� ������ �� row�� Row�� �����Ѵ�.
			liberal_sel = atoi(Row[0]);     // DB st_score���̺��� ���缱�� �����͵��� liberal_sel�� ����
		}

		return true;
	}


	boolean gradu_sum3(char stno[30]) { //���� �հ�

		a();    // DBȣ��
		char buff[255];     // query���� ���� ����
		sprintf_s(buff, "Select sum(grade) from st_score where stno = '%s' and complete in ('��������','�����ʼ�') ", stno);        // st_score ���̺� select�ϴ� ����
		mysql_query(ConnPtr, "set names euckr");        // �ѱ��� ������ �ʵ��� ��û
		Stat = mysql_query(ConnPtr, buff);      // buff�� ������ ConnPtr�� ���ؼ� query ��û
		Result = mysql_store_result(ConnPtr);   // mysql_store_result() �Լ��� ���� ���� ��ü�� ��� ���� Result ������ ����

		while ((Row = mysql_fetch_row(Result)) != NULL) {       // mysql_fetch_row()�� ����� Result�� ������ �� row�� Row�� �����Ѵ�.
			major = atoi(Row[0]);       // DB st_score���̺��� ��������+�����ʼ� �����͵��� major�� ����
		}

		return true;
	}

	boolean gradu_sum4(char stno[30]) { //�������� �հ�

		a();    // DBȣ��
		char buff[255];     // query���� ���� ����
		sprintf_s(buff, "Select sum(grade) from st_score where stno = '%s' and complete = '��������'", stno);       // st_score ���̺� select�ϴ� ����
		mysql_query(ConnPtr, "set names euckr");        // �ѱ��� ������ �ʵ��� ��û
		Stat = mysql_query(ConnPtr, buff);      // buff�� ������ ConnPtr�� ���ؼ� query ��û
		Result = mysql_store_result(ConnPtr);       // mysql_store_result() �Լ��� ���� ���� ��ü�� ��� ���� Result ������ ����

		while ((Row = mysql_fetch_row(Result)) != NULL) {       // mysql_fetch_row()�� ����� Result�� ������ �� row�� Row�� �����Ѵ�.
			free_major = atoi(Row[0]);      // DB st_score���̺��� ��������+�����ʼ� �����͵��� major�� ����
		}

		return true;
	}
	boolean gradu_sum5(char stno[30]) { //�������� �հ�

		a();    // DBȣ��
		char buff[255];     // query���� ���� ����
		sprintf_s(buff, "Select sum(grade) from st_score where stno = '%s'", stno);     // st_score ���̺� select�ϴ� ����
		mysql_query(ConnPtr, "set names euckr");        // �ѱ��� ������ �ʵ��� ��û
		Stat = mysql_query(ConnPtr, buff);      // buff�� ������ ConnPtr�� ���ؼ� query ��û
		Result = mysql_store_result(ConnPtr);       // mysql_store_result() �Լ��� ���� ���� ��ü�� ��� ���� Result ������ ����

		while ((Row = mysql_fetch_row(Result)) != NULL) {       // mysql_fetch_row()�� ����� Result�� ������ �� row�� Row�� �����Ѵ�.
			score_sum = atoi(Row[0]);       // DB st_score���̺��� ��������+�����ʼ� �����͵��� major�� ����
		}

		return true;
	}
};

class Main : DB {       // DBŬ���� ���
public:
	st_info sti; //st_infoŬ���� ��üȭ
	st_score sts;  //st_socreŬ���� ��üȭ
	pf_info pfi;  //pf_infoŬ���� ��üȭ

	void start() { //�����Լ�
		int log;
		cout << "================ �Ⱦ���б� �л� ���� �ý��� =================" << endl << endl;
		cout << "1, ������ �α���" << endl << endl;
		cout << "2, �л� �α���" << endl << endl;
		cout << "0���� ������ ����˴ϴ�." << endl << endl;
		cout << "�Է��Ͻÿ� >> ";
		cin >> log;
		cout << endl << "==============================================================" << endl << endl;

		switch (log) {
		case 0:
			exit(0);    // ���α׷� ����
		case 1:
			login_Pf(); //���� �α��� �Լ� ȣ��
			break;
		case 2:
			login_St(); //�л� �α��� �Լ� ȣ��
			break;
		default:    // �ٸ� ���� �Է½� start() �Լ� �ٽ� ����
			cout << "�ٽ� �Է��ϼ���." << endl << endl << endl << endl;
			start();
		}
	}

	void login_St() { //�л� �α��� �Լ�
		char id[30]; //�й��� ����ڿ��� ���� �޾� stno�� �־��ֱ� ���� ����
		char pw[30]; //��й�ȣ�� ����ڿ��� ���� �޾� stpw�� �־��ֱ� ���� ����
		cout << endl << "========================== �α��� ============================" << endl;
		cout << endl << "�й�(ID) : ";
		cin >> id;
		sti.setStno(id);
		cout << endl << "��й�ȣ : ";
		cin >> pw;
		sti.setStpw(pw);
		cout << endl << "==============================================================" << endl << endl;

		if (login_st(sti.getStno(), sti.getStpw())) { // DB�� ����� �л��� �й��� ����� �Է��� ���� ��ġ�� ���
			cout << "*************************************************************" << endl << endl;
			cout << "                     �α��� �Ǿ����ϴ�." << endl << endl;
			cout << "*************************************************************" << endl << endl << endl;
			choice_St(); //�л� �ɼǼ��� �Լ� ȣ��
		}
		else {
			cout << "*************************************************************" << endl << endl;
			cout << "��ϵǾ����� ���� �й��̰ų� ��й�ȣ�� Ʋ�Ƚ��ϴ�." << endl;
			cout << endl << "*************************************************************" << endl;
			login_St(); // �ٽ� �α��� â���� �̵�
		}
	}

	void login_Pf() { //���� �α��� �Լ�
		char id[30]; //������ȣ�� ����ڿ��� ���� �޾� pfno�� �־��ֱ� ���� ����
		char pw[30]; //��й�ȣ�� ����ڿ��� ���� �޾� pfpw�� �־��ֱ� ���� ����
		cout << endl << "========================== �α��� ============================" << endl;
		cout << endl << "������ ��ȣ (ID) : ";
		cin >> id;
		pfi.setPfno(id);
		cout << endl << "��й�ȣ : ";
		cin >> pw;
		pfi.setPfpw(pw);
		cout << endl << "==============================================================" << endl << endl;
		if (login_pf(pfi.getPfno(), pfi.getPfpw())) { //DB�� ����� �������̵�� ����� �Է��� ���� ��ġ�� ���
			cout << "*************************************************************" << endl << endl;
			cout << "                     �α��� �Ǿ����ϴ�." << endl << endl;
			cout << "*************************************************************" << endl << endl << endl << endl;
			choice_Pf(); //���� �ɼǼ��� �Լ� ȣ��
		}
		else {
			cout << "*************************************************************" << endl << endl;
			cout << "��ϵǾ����� ���� ������ȣ�ų� ��й�ȣ�� Ʋ�Ƚ��ϴ�." << endl << endl;
			cout << "*************************************************************" << endl << endl << endl << endl;
			login_Pf(); // �ٽ� �α��� â���� �̵�
		}
	}

	void choice_Pf() { //���� �ɼǼ��� �Լ�
		int cho;
		cout << endl << "========================== ������ ============================" << endl << endl;
		cout << "1, �����Է�" << endl << endl;
		cout << "2, �α׾ƿ�" << endl << endl;
		cout << "�Է��Ͻÿ� >> ";
		cin >> cho;
		cout << endl << endl << "==============================================================" << endl << endl;

		switch (cho) {
		case 1:
			update_Score(); //���� �Է� �Լ� ȣ��
			break;
		case 2:
			logout(); //�α׾ƿ� �Լ� ȣ��
			start();
			break;
		default:    // �ٸ� ���� �Է� �� �ٽ� ���� �ɼ�â���� �̵�
			cout << "1�� 2�� �߿����� �Է����ּ���." << endl << endl << endl << endl;
			choice_Pf(); // �ٽ� �ɼǼ��� â���� �̵�
		}
	}

	void choice_St() { //�л� �ɼǼ��� �Լ�
		int cho;
		cout << endl << "=========================== �л� =============================" << endl << endl;
		cout << "1, ��������" << endl << endl;
		cout << "2, ��������" << endl << endl;
		cout << "3, �α׾ƿ�" << endl << endl;
		cout << "�Է��Ͻÿ� >> ";
		cin >> cho;
		cout << endl << endl << "==============================================================" << endl << endl;

		switch (cho) {
		case 1:
			score_see(); //���� Ȯ�� �Լ� ȣ��
			break;
		case 2:
			gradu(); //�������� �Լ� ȣ��
			break;
		case 3:
			logout(); //�α׾ƿ� �Լ� ȣ��
			start();
			break;
		default:    // �ٸ� ���� �Է� �� �ٽ� �л� �ɼ�â���� �̵�
			cout << "1�� 2�� 3�� �߿����� �Է����ּ���." << endl << endl << endl << endl;
			choice_St();
		}
	}

	void update_Score() { //���� �Է� �Լ�
		int add;
		char sbname[30]; //���� �Է��� ������ ����ڿ��� ���� �޾� sbname�� �־��ֱ� ���� ����
		char stno[30];  //���� �Է��� �й��� ����ڿ��� ���� �޾� sbno�� �־��ֱ� ���� ����
		char score[30];  //�Է��� ����(���)�� ����ڿ��� ���� �޾� score�� �־��ֱ� ���� ����
		cout << endl << "************************** ���� �߰� ************************" << endl;
		cout << endl << "�߰��� ���� : "; //���� �߰��� � �߰����� �Է¹޴´�.
		cin >> add;

		for (int j = 0; j < add; j++) {
			cout << " \n";
			cout << " => ���� �Է��� ���� : ";
			cin >> sbname;
			sts.setSb_name(sbname);
			cout << " \n";
			cout << " => ���� �Է��� �л��й� : ";
			cin >> stno;
			sti.setStno(stno);
			cout << " \n";
			cout << " => ���� : ";
			cin >> score;
			sts.setScore(score);

			if (score_check(sts.getSb_name(), sti.getStno())) {// �Է¹��� �����, �й��� DB�� ������ if�� ����
				score_add(sts.getSb_name(), sti.getStno(), sts.getScore());// �����, �й�, ������ �޾Ƽ� DB �Լ��� score_add()�� ����
				cout << "*************************************************************\n\n";
				cout << "                      �߰��Ǿ����ϴ�." << endl << endl;
				cout << "*************************************************************\n\n\n\n";
			}
			else {  // �Է¹��� �����, �й��� DB�� ������ else�� ���
				cout << "*************************************************************\n\n";
				cout << "           �������� �ʴ� �й��̰ų� �����Դϴ�.\n";
				cout << "                  �ٽ��Է����ּ���.\n\n";
				cout << "*************************************************************" << endl << endl << endl << endl;
				j--;
			}
		}
		choice_Pf();    // ���� �߰� �Ϸ��ϸ� �����ɼ� ���� �޴��� �̵�
	}

	void score_see() { //(�л���)����Ȯ�� �Լ�
	stop:
		char session[30]; //�Է¹��� �г�,�б⸦
		cout << endl << "�г�, �б� : ";
		cin >> session;
		sts.setSession(session); // �г�, �б⸦ �޾ƿ� (1-1, 1-2 ����)

		if (score_show(sti.getStno(), sts.getSession())) {    // �α��ο��� ���� �й��� �Է¹��� �б⸦ DB�Լ��� score_show()�� �Ѱ� �б�� �ʿ��� �������� �˻���
															  // for���� ���� com[i]�� ����Ϸ� ������ ���� ���̵��� �޶� UI�� ������������ 3-2�б� ������� �������� ����.
			cout << endl << " ����������������������������������������������������������������������������������������������������������������������������������������������������\n";
			cout << " ��                                  " << sts.getSession() << " ����                              ��\n";
			cout << " ����������������������������������������������������������������������������������������������������������������������������������������������������\n";
			cout << " ��  �̼�����  ��                   �����              ��    ����  ��  ���  ��\n";
			cout << " ����������������������������������������������������������������������������������������������������������������������������������������������������\n";
			cout << " ��  " << com[0] << "  ��             " << sb_na[0] << "          ��     " << grad[0] << "    ��    " << scor[0] << "   ��\n";
			cout << " ����������������������������������������������������������������������������������������������������������������������������������������������������\n";
			cout << " ��  " << com[1] << "  ��             " << sb_na[1] << "          ��     " << grad[1] << "    ��    " << scor[1] << "   ��\n";
			cout << " ����������������������������������������������������������������������������������������������������������������������������������������������������\n";
			cout << " ��  " << com[2] << "  ��                " << sb_na[2] << "             ��     " << grad[2] << "    ��    " << scor[2] << "   ��\n";
			cout << " ����������������������������������������������������������������������������������������������������������������������������������������������������\n";
			cout << " ��  " << com[3] << "  ��           " << sb_na[3] << "        ��     " << grad[3] << "    ��    " << scor[3] << "   ��\n";
			cout << " ����������������������������������������������������������������������������������������������������������������������������������������������������\n";
			cout << " ��  " << com[4] << "  ��                " << sb_na[4] << "         ��     " << grad[4] << "    ��    " << scor[4] << "   ��\n";
			cout << " ����������������������������������������������������������������������������������������������������������������������������������������������������\n";
			cout << " ��  " << com[5] << "  ��        " << sb_na[5] << "     ��     " << grad[5] << "    ��    " << scor[5] << "   ��\n";
			cout << " ����������������������������������������������������������������������������������������������������������������������������������������������������\n";
			cout << " ��      " << com[6] << "      ��                   " << sb_na[6] << "                    ��     " << grad[6] << "     ��    " << scor[6] << "    ��\n";
			cout << " ����������������������������������������������������������������������������������������������������������������������������������������������������\n";
			cout << " ��      " << com[7] << "      ��                   " << sb_na[7] << "                    ��     " << grad[7] << "     ��    " << scor[7] << "    ��\n";
			cout << " ����������������������������������������������������������������������������������������������������������������������������������������������������\n";
			cout << " ��      " << com[8] << "      ��                   " << sb_na[8] << "                    ��     " << grad[8] << "     ��    " << scor[8] << "    ��\n";
			cout << " ����������������������������������������������������������������������������������������������������������������������������������������������������\n";
			cout << " ��      " << com[9] << "      ��                   " << sb_na[9] << "                    ��     " << grad[9] << "     ��    " << scor[9] << "    ��\n";
			cout << " ����������������������������������������������������������������������������������������������������������������������������������������������������\n" << endl << endl << endl;
			choice_St();    // �б⸦ �����ָ� �ٽ� �л��ɼ� �޴��� �̵�
		}
		else {  // �Է��� �бⰡ DB�� �������� ������ ����
			cout << endl << "*************************************************************\n\n";
			cout << "                �������� �ʴ� �б��Դϴ�.\n";
			cout << "                  �ٽ��Է����ּ���.\n\n";
			cout << "*************************************************************" << endl << endl << endl << endl;
			goto stop;
		}

	}

	void gradu() { //(�л���)�������� Ȯ�� �Լ�

		gradu_sum1(sti.getStno());
		gradu_sum2(sti.getStno());
		gradu_sum3(sti.getStno());
		gradu_sum4(sti.getStno());
		gradu_sum5(sti.getStno());

		// �ܿ����� ���
		int re, sel, majorr, free_Major, score_Sum;
		re = 14 - liberal_re;
		sel = 25 - liberal_sel;
		majorr = 48 - major;
		free_Major = 42 - free_major;
		score_Sum = 130 - score_sum;

		cout << "���������������������������������������������������������������������������������������������������������������������������������������������������������������� \n";
		cout << "��          ��           ����            ��             ����         ��            �� \n";
		cout << "���������������������������������������������������������������������������������������������������������������������������������������������������������������� \n";
		cout << "�� �̼����� ��     �ʼ�    ��     ����    ��    ������   ��     ����   ��  ��������  �� \n";
		cout << "����������������������������������������������������������������������������������������������������������������������������������������������������������������\n";
		cout << "�� �������� ��      " << "14" << "     ��      " << "25" << "     ��      " << "48" << "     ��     " << "42" << "     ��     " << "130" << "    �� \n";
		cout << "���������������������������������������������������������������������������������������������������������������������������������������������������������������� \n";
		cout << "�� ������� ��      " << liberal_re << "     ��      " << liberal_sel << "     ��      " << major << "     ��      " << free_major << "    ��     " << score_sum << "    �� \n";
		cout << "���������������������������������������������������������������������������������������������������������������������������������������������������������������� \n";
		cout << "�� �ܿ����� ��      " << re << "      ��      " << sel << "      ��     " << majorr << "     ��     " << free_Major << "     ��     " << score_Sum << "     �� \n";
		cout << "���������������������������������������������������������������������������������������������������������������������������������������������������������������� \n\n\n\n";
		choice_St();    // �ٽ� �л��ɼ� �޴��� �̵�

	}


	void logout() { //�α׾ƿ� �Լ�
		cout << endl << "*************************************************************" << endl << endl;
		cout << "                   �α׾ƿ� �Ǿ����ϴ�." << endl << endl;
		cout << "*************************************************************" << endl << endl << endl << endl;
	}

};

int main()
{

	Main m;
	m.start(); //���� �Լ� ȣ��

	return 0;
}