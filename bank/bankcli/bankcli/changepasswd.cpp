#include "myhead.h"

extern SOCKET sock;

int change_passwd::m_change_passwd(const long &id, const char* passwd, const char* newpasswd)
{
	struct data d;
	//填充包头数据
	d.head.action = 7;
	d.head.len = sizeof(d.body) + sizeof(d.tail);
	//填充包体数据
	d.body.account_id = id;
	strcpy_s(d.body.pass, passwd);
	strcpy_s(d.body.newpass, newpasswd);
	//密码不足八位自动填充'a'
	if ((strlen(d.body.pass)) < 8)
	{
		for (int i = 7; i >= strlen(d.body.pass); i--)
		{
			d.body.pass[i] = 'a';
		}
	}
	d.body.pass[8] = '\0';

	if ((strlen(d.body.newpass)) < 8)
	{
		for (int i = 7; i >= strlen(d.body.newpass); i--)
		{
			d.body.newpass[i] = 'a';
		}
	}
	d.body.newpass[8] = '\0';
	//封包
	memcpy(pipe, &d, sizeof(d));

	//发包
	int ret = send(sock, pipe, sizeof(pipe), 0);
	if (ret == -1)
	{
		cout << "send error: " << WSAGetLastError() << endl;
		return 1;
	}

	//收包
	struct Head_rep rep;
	
	recv(sock, (char*)&rep, sizeof(rep), 0);
	if (rep.action == 0)
	{
		cout << "\n" << "您已修改成功。" << endl;
	}
	else if (rep.action == 1)
	{
		cout << "您输入的密码不正确。" << endl;
	}
	else if (rep.action == 2)
	{
		cout << "您输入的用户名不存在。" << endl;
	}
		
	return 0;
}