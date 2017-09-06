#include "myhead.h"

int query::m_query(const int &fd, const char* pipe, MYSQL* mysql)
{
	cout << "从客户端" << fd << "收到转账请求！" << endl;

	struct data da;
	memcpy(&da.head.action, pipe, 4);
	memcpy(&da.body.account_id, pipe + 8, 4);
	memcpy(da.body.begin_date, pipe + 12, 20);
	memcpy(da.body.end_date, pipe + 32, 20);

	//////////////////////////////////////////////////////////////////////
	//////////////////获取数据包成功，进行存款操作/////////////////////////////
	//获取用户名
	char buff1[1024];
	sprintf_s(buff1, "select id, save, drow, balance, days, fromid from id%d where days >= '%s' and days <= '%s';"
		, da.body.account_id, da.body.begin_date, da.body.end_date);
	int result = mysql_query(mysql, buff1);
	if (result != 0)
	{
		printf("select error: %s\n", mysql_error(mysql));
		return -2;
	}

	struct Data_rep rep;
	int count = 0;
	MYSQL_RES* mysql_res = NULL;
	MYSQL_FIELD* mysql_field = NULL;
	MYSQL_ROW mysql_row = NULL;
	unsigned int cols = 0;
	mysql_res = mysql_store_result(mysql);
	cols = mysql_num_fields(mysql_res);
	if (mysql_res)
	{
		count = mysql_num_rows(mysql_res);    //行数
		cout << "count = " << count << endl;

		cout << "查询成功！" << endl;
		while ((mysql_row = mysql_fetch_row(mysql_res)))
		{
			rep.body_rep.account_id = atol(mysql_row[0]);
			rep.body_rep.save = atof(mysql_row[1]);
			rep.body_rep.drow = atof(mysql_row[2]);
			rep.body_rep.balance = atof(mysql_row[3]);
			strcpy_s(rep.body_rep.date, mysql_row[4]);
			if (mysql_row[5])
			{
				rep.body_rep.other_account_id = atol(mysql_row[5]);
			}
			else
			{
				rep.body_rep.other_account_id = 0;
			}
			rep.body_rep.total = 1;

			count--;
			cout << "2.count = " << count << endl;
			if (-1 == count)
			{
				rep.body_rep.total = 0;
			}
			rep.head_rep.action = 0;
			send(fd, (char*)&rep, sizeof(rep), 0);
		}
		mysql_free_result(mysql_res);
	}

	return 0;
}