#include "myhead.h"

extern SOCKET sock;

int opencont::m_open(const char* name, char* passwd, const char* id, const double &money)
{
	/////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////
	struct data d;
	//填充包头数据
	d.head.action = 2;
	d.head.len = sizeof(d.body) + sizeof(d.tail);
	//填充包体数据
	strcpy_s(d.body.name, name);
	strcpy_s(d.body.id, id);
	d.body.money = money;
	strcpy_s(d.body.passwd, passwd);
	//密码不足八位自动填充'a'
	if ((strlen(d.body.passwd)) < 8)
	{
		for (int i = 7; i >= strlen(d.body.passwd); i--)
		{
			d.body.passwd[i] = 'a';
		}
	}
	d.body.passwd[8] = '\0';

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

	recv(sock, (char*)&data_rep, sizeof(data_rep), 0);
	if (data_rep.head_rep.action == 0)
	{
		cout << "\n您已开户成功！" << endl;
		cout << "请记住您的ID：" << data_rep.body_rep.account_id << " ; 开户日期：" << data_rep.body_rep.data << endl;
		return 1;
	}

	return 0;
}