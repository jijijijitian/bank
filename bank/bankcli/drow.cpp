#include "myhead.h"

extern SOCKET sock;

int drow::m_drow(const long &id, const char* passwd, const double &money)
{
	struct data d;
	//����ͷ����
	d.head.action = 4;
	d.head.len = sizeof(d.body) + sizeof(d.tail);
	//����������
	d.body.money = money;
	strcpy_s(d.body.passwd, passwd);
	d.body.account_id = id;

	//���벻���λ�Զ����'a'
	if ((strlen(d.body.passwd)) < 8)
	{
		for (int i = 7; i >= strlen(d.body.passwd); i--)
		{
			d.body.passwd[i] = 'a';
		}
	}
	d.body.passwd[8] = '\0';

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
	struct Data_rep data_rep;
	recv(sock, (char*)&data_rep, sizeof(data_rep), 0);
	if (data_rep.head_rep.action == 0)
	{
		cout << "\n" << "����ȡ��ɹ����˻���ϸ��" << endl;
		cout << setiosflags(ios::left) << setw(10) << "name"
			<< setiosflags(ios::left) << setw(10) << "balance"
			<< setiosflags(ios::left) << setw(19) << "data" << endl;
		cout << setiosflags(ios::left) << setw(10) << data_rep.body_rep.name
			<< setiosflags(ios::left) << setw(10) << data_rep.body_rep.balance
			<< setiosflags(ios::left) << setw(19) << data_rep.body_rep.data << endl;
		return 2;
	}
	else if (data_rep.head_rep.action == -1)
	{
		cout << "����������벻��ȷ�����������롣" << endl;
		return 3;
	}
	else if (data_rep.head_rep.action == -2)
	{
		cout << "��������û������ڣ����������롣" << endl;
		return 4;
	}

	return 0;
}