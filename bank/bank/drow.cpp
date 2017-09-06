#include "myhead.h"

int drow::m_drow(const int &fd, const char* pipe, MYSQL* mysql)
{
	cout << "从客户端" << fd << "收到取款请求！" << endl;

	struct data da;
	memcpy(&da.head.action, pipe, 4);
	memcpy(&da.body.money, pipe + 8, 8);
	memcpy(&da.body.passwd, pipe + 20, 9);
	memcpy(&da.body.account_id, pipe + 16, 4);

	/*//获取校验码
	char cd[9] = { 0 };
	memcpy(cd, pipe + 24, 9);

	//验证校验码是否正确
	for (int i = 7; i>3; i--)
	{
	cd[i] = cd[i] ^ cd[i - 4];
	cd[i] = cd[i] ^ ((da.head.action >> (i % 2)) & 0xff);
	cd[i - 4] = cd[i] ^ cd[i - 4];
	cd[i - 4] = cd[i - 4] ^ ((da.head.action >> (i % 2)) & 0xff);
	}

	if (strcmp(cd, da.body.passwd) != 0)
	{
	cout << "数据包丢包！" << endl;
	return -1;
	}*/

	//获取当前时间
	char days[20] = { 0 };
	strcpy_s(days, gettime());

	//////////////////////////////////////////////////////////////////////
	//////////////////获取数据包成功，进行存款操作/////////////////////////////
	//获取用户名
	char buff1[1024];
	sprintf_s(buff1, "select money, name, passwd from register where id = %d;", da.body.account_id);
	int result = mysql_query(mysql, buff1);
	if (result != 0)
	{
		printf("select error: %s\n", mysql_error(mysql));
		return -2;
	}

	double money1 = 0;
	struct Data_rep rep;
	MYSQL_RES* mysql_res = NULL;
	MYSQL_ROW mysql_row;
	mysql_res = mysql_store_result(mysql);
	if (mysql_row = mysql_fetch_row(mysql_res))
	{
		if (strcmp(mysql_row[2], da.body.passwd) != 0)
		{
			cout << "密码不正确" << endl;
			rep.head_rep.action = -1;
			send(fd, (char*)&rep, sizeof(rep), 0);
			return -3;
		}
		money1 = atof(mysql_row[0]);
		rep.body_rep.balance = money1 - da.body.money;
		strcpy_s(rep.body_rep.name, mysql_row[1]);
	}
	else
	{
		cout << "用户不存在" << endl;
		rep.head_rep.action = -2;
		send(fd, (char*)&rep, sizeof(rep), 0);
		return -4;
	}

	memset(buff1, 0, sizeof(buff1));
	sprintf_s(buff1, "insert into id%d values(%d, '%s', 0, &lf, %lf, '%s', NULL);", da.body.account_id, da.body.account_id, rep.body_rep.name, da.body.money, rep.body_rep.balance, days);
	result = mysql_query(mysql, buff1);
	if (result != 0)
	{
		cout << "insert error: " << mysql_error(mysql) << endl;
		return -5;
	}

	memset(buff1, 0, sizeof(buff1));
	sprintf_s(buff1, "update register set money = %lf where id = %d;", rep.body_rep.balance, da.body.account_id);
	result = mysql_query(mysql, buff1);
	if (result != 0)
	{
		cout << "insert error: " << mysql_error(mysql) << endl;
		return -6;
	}

	//发送应答数据包
	else
	{
		cout << "存款成功！" << endl;

		rep.head_rep.action = 0;
		strcpy_s(rep.body_rep.data, days);

		send(fd, (char*)&rep, sizeof(rep), 0);
	}

	return 0;
}