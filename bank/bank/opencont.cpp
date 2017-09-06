#include "myhead.h"

int opencont::m_open(const int &fd, const char* pipe, MYSQL* mysql)
{
	cout << "从客户端" << fd << "收到开户请求！" << endl;

	struct data da;
	memcpy(&da.head.action, pipe, 4);
	memcpy(da.body.name, pipe + 25, 16);
	memcpy(da.body.passwd, pipe + 16, 9);
	memcpy(da.body.id, pipe + 41, 19);
	memcpy(&da.body.money, pipe + 8, 8);
	//获取校验码
	char cd[9] = { 0 };
	memcpy(cd, pipe + 64, 9);

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
	}

	//获取当前时间
	struct tm t;
	time_t now;
	time(&now);
	localtime_s(&t, &now);
	char days[20] = { 0 };
	sprintf_s(days, "%d-%02d-%02d %02d:%02d:%02d", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);

	//随记生成6位id
	long id = 0;
	int count = 1;
	srand((unsigned)time(NULL));
	for (int i = 0; i < 6; i++)
	{
		id += (rand() % 10) * count;
		count *= 10;
	}

	//////////////////////////////////////////////////////////////////////
	//////////////////获取数据包成功，进行开户操作/////////////////////////////
	char buff1[1024];
	sprintf_s(buff1, "insert into register values(%d, '%s', '%s', '%s', '%s', %lf);", id, da.body.name, da.body.id, cd, days, da.body.money);
	int result = mysql_query(mysql, buff1);
	if (result != 0)
	{
		printf("error: %s\n", mysql_error(mysql));
		return -2;
	}
	
	memset(buff1, 0, sizeof(buff1));
	sprintf_s(buff1, "create table ID%d(id mediumint, name char(16), save double(8,2), drow double(8,2), balance double(8,2), days datetime ,fromid mediumint);", id);
	result = mysql_query(mysql, buff1);
	if (result != 0)
	{
		printf("error: %s\n", mysql_error(mysql));
		return -3;
	}

	memset(buff1, 0, sizeof(buff1));
	sprintf_s(buff1, "insert into ID%d values(%d, '%s', %lf, 0, %lf, '%s', NULL);", id, id, da.body.name, da.body.money, da.body.money, days);
	result = mysql_query(mysql, buff1);
	if (result != 0)
	{
		printf("error: %s\n", mysql_error(mysql));
		return -4;
	}
	//发送应答数据包
	else
	{
		cout << "开户成功！" << endl;

		data_rep.head_rep.action = 0;
		data_rep.body_rep.account_id = id;
		strcpy_s(data_rep.body_rep.data, days);

		send(fd, (char*)&data_rep, sizeof(data_rep), 0);
	}	
	
	return 0;
}