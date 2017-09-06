#include "myhead.h"

extern SOCKET sock;

int enter::m_enter(const char* usr, const char* passwd)
{
	/////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////
	struct data d;
	//填充包头数据
	d.head.action = 1;
	d.head.len = sizeof(d.body) + sizeof(d.tail);
	//填充包体数据
	strcpy_s(d.body.usr, usr);
	strcpy_s(d.body.passwd, passwd);
	//密码不足八位自动填充'a'
	if (strlen(d.body.passwd) < 9)
	{
		for (int i = 0; i < 8; i++)
		{
			if (d.body.passwd[i] == '0')
			{
				d.body.passwd[i] = 'a';
			}
		}
	}

	//填充包尾数据
	strcpy_s(d.tail.checkdigit, d.body.passwd);
	for (int i = 0; i<4; i++)
	{
		d.tail.checkdigit[i] = d.tail.checkdigit[i] ^ d.tail.checkdigit[i + 4];
		d.tail.checkdigit[i] = d.tail.checkdigit[i] ^ ((d.head.action >> (i % 2)) & 0xff);
		d.tail.checkdigit[i + 4] = d.tail.checkdigit[i] ^ d.tail.checkdigit[i + 4];
		d.tail.checkdigit[i + 4] = d.tail.checkdigit[i + 4] ^ ((d.head.action >> (i % 2)) & 0xff);
	}

	/////////////////////////////////////////////////////////////////////
	/////////////////////封包/////////////////////////////////////////////
	memcpy(pipe, &d, sizeof(d));

	/////////////////////////////////////////////////////////////////////
	/////////////////////发包/////////////////////////////////////////////
	int ret = send(sock, pipe, sizeof(pipe), 0);
	
	if (ret == -1)
	{
		   cout << "send error: " << WSAGetLastError() << endl;
		   return 1;
	}
	
	recv(sock, (char*)&rep, sizeof(rep), 0);
	if (rep.action == 0)
	{
		cout << "\n\n您已登录成功！" << endl;
		return 1;
	}
	else
	{
		cout << "\n\n您的用户名或密码错误，请重新输入。" << endl;
	}

	return 0;
}