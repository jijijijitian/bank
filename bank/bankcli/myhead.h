#ifndef _MYHEAD_H_
#define _MYHEAD_H_

#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <iostream>
#include <windows.h>
#include <string.h>
#include <mysql.h>
#include <conio.h>
#include <iomanip>

using namespace std;

int do_client();                      //服务器与客户端的链接工作                          
void show();
void entershow();

class bank                            //基类
{
public:
	char pipe[1024];                  //数据缓冲区
protected:
	typedef struct Head_send          //请求包头
	{
		int action;
		int len;
	}SendHead;

	typedef struct Head_rep          //应答包头
	{
		int action;
		int len;
		int errornum;
		char errormsg[30];
	}RecvHead;
	struct Head_rep rep;

	struct Tail                       //包尾
	{
		char checkdigit[9];           //校验码
	};
};

class enter : public bank
{
public:
	int m_enter(const char* usr, const char* passwd);
private:
	struct Body                       //柜员登录包体
	{
		char usr[11];
		char passwd[9];
	};

	struct data                       //柜员登录数据包
	{
		struct Head_send head;
		struct Body body;
		struct Tail tail;
	};
};

class opencont : public bank
{
public:
	int m_open(const char* name, char* passwd, const char* id, const double &money);
private:
	struct Body_send
	{
		double money;
		char passwd[9];
		char name[16];
		char id[19];
	};

	struct Body_rep
	{
		long account_id;
		char data[20];
	};

	struct Data_rep
	{
		struct Body_rep body_rep;
		struct Head_rep head_rep;
	};
	struct Data_rep data_rep;

	struct data
	{
		struct Head_send head;
		struct Body_send body;
		struct Tail tail;
	};
};

class save : public bank
{
public:
	int m_save(const long &id, const double &money);
private:
	struct Body_send
	{
		double money;
		long account_id;
	};

	struct Body_rep
	{
		char name[16];
		double balance;
		char data[20];
	};

	struct Data_rep
	{
		struct Body_rep body_rep;
		struct Head_rep head_rep;
	};

	struct data
	{
		struct Head_send head;
		struct Body_send body;
		struct Tail tail;
	};

};

class drow : public bank
{
public:
	int m_drow(const long &id, const char* passwd, const double &money);
private:
	struct Body_send
	{
		double money;
		long account_id;
		char passwd[9];		
	};

	struct Body_rep
	{
		char name[16];
		double balance;
		char data[20];
	};

	struct Data_rep
	{
		struct Body_rep body_rep;
		struct Head_rep head_rep;
	};

	struct data
	{
		struct Head_send head;
		struct Body_send body;
		struct Tail tail;
	};

};

class transfer : public bank
{
public:
	int m_transfer(const long &id, const char* passwd, const double &money, const long &toid);
private:
	struct Body_send
	{
		double money;
		long account_id;
		long other_account_id;
		char passwd[9];
	};

	struct Body_rep
	{
		char name[16];
		double balance;
		char data[20];
	};

	struct Data_rep
	{
		struct Body_rep body_rep;
		struct Head_rep head_rep;
	};

	struct data
	{
		struct Head_send head;
		struct Body_send body;
		struct Tail tail;
	};

};

class query : public bank
{
public:
	int m_query(const long &id, const char* begin_date, const char* end_date);
private:
	struct Body_send
	{		
		long account_id;
		char begin_date[20];
		char end_date[20];
	};

	struct Body_rep
	{		
		long account_id;
		long other_account_id;
		double save;
		double drow;
		double balance;		
		char date[20];
		int total;
	};

	struct Data_rep
	{
		struct Body_rep body_rep;
		struct Head_rep head_rep;
	};

	struct data
	{
		struct Head_send head;
		struct Body_send body;
		struct Tail tail;
	};
};

class change_passwd : public bank
{
public:
	int m_change_passwd(const long &id, const char* passwd, const char* newpasswd);
private:
	struct Body_send
	{
		long account_id;
		char pass[9];
		char newpass[9];
	};

	struct data
	{
		struct Head_send head;
		struct Body_send body;
		struct Tail tail;
	};
};

#endif