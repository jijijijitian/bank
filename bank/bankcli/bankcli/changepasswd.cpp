#include "myhead.h"

extern SOCKET sock;

int change_passwd::m_change_passwd(const long &id, const char* passwd, const char* newpasswd)
{
	struct data d;
	//����ͷ����
	d.head.action = 7;
	d.head.len = sizeof(d.body) + sizeof(d.tail);
	//����������
	d.body.account_id = id;
	strcpy_s(d.body.pass, passwd);
	strcpy_s(d.body.newpass, newpasswd);
	//���벻���λ�Զ����'a'
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
	//���
	memcpy(pipe, &d, sizeof(d));

	//����
	int ret = send(sock, pipe, sizeof(pipe), 0);
	if (ret == -1)
	{
		cout << "send error: " << WSAGetLastError() << endl;
		return 1;
	}

	//�հ�
	struct Head_rep rep;
	
	recv(sock, (char*)&rep, sizeof(rep), 0);
	if (rep.action == 0)
	{
		cout << "\n" << "�����޸ĳɹ���" << endl;
	}
	else if (rep.action == 1)
	{
		cout << "����������벻��ȷ��" << endl;
	}
	else if (rep.action == 2)
	{
		cout << "��������û��������ڡ�" << endl;
	}
		
	return 0;
}