#include "myhead.h"

int enter::m_enter(const char* usr, const char* passwd, const int &fd, const char* pipe, MYSQL* mysql)
{
	cout << "从客户端" << fd << "收到登录请求！" << endl;

	int act;
	memcpy(&act, pipe, 4);
	//获取校验码
	char cd1[9] = { 0 };
	char cd2[9] = { 0 };
	memcpy(cd2, pipe + 19, 9);
	memcpy(cd1, pipe + 28, 9);
	
	//验证校验码是否正确
	for (int i = 7; i>3; i--)
	{
		cd1[i] = cd1[i] ^ cd1[i - 4];
		cd1[i] = cd1[i] ^ ((act >> (i % 2)) & 0xff);
		cd1[i - 4] = cd1[i] ^ cd1[i - 4];
		cd1[i - 4] = cd1[i - 4] ^ ((act >> (i % 2)) & 0xff);
	}
	
	if (strcmp(cd1, cd2) != 0)
	{
		cout << "数据包丢包！" << endl;
		return -1;
	}	

	//////////////////////////////////////////////////////////////////////
	//////////////////获取数据包成功，进行登录操作
	if (!mysql_real_connect(mysql, "localhost", usr, passwd, "bank", 0, NULL, 0))
	{
		rep.action = 1;
		memcpy(&rep + 4, &pipe + 4, 4);
		rep.errornum = 1;
		strcpy_s(rep.errormsg, "用户名或密码错误");
		printf("connect error: %s\n", mysql_error(mysql));
		send(fd, (char*)&rep, sizeof(rep), 0);
		return -3;
	}
	else
	{
		cout << "登录成功！" << endl;
		rep.action = 0;
		send(fd, (char*)&rep, sizeof(rep), 0);

		return 0;
	}

}