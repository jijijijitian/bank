#include "myhead.h"

extern SOCKET sock;

int enter::m_enter(const char* usr, const char* passwd)
{
	/////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////
	struct data d;
	//����ͷ����
	d.head.action = 1;
	d.head.len = sizeof(d.body) + sizeof(d.tail);
	//����������
	strcpy_s(d.body.usr, usr);
	strcpy_s(d.body.passwd, passwd);
	//���벻���λ�Զ����'a'
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

	//����β����
	strcpy_s(d.tail.checkdigit, d.body.passwd);
	for (int i = 0; i<4; i++)
	{
		d.tail.checkdigit[i] = d.tail.checkdigit[i] ^ d.tail.checkdigit[i + 4];
		d.tail.checkdigit[i] = d.tail.checkdigit[i] ^ ((d.head.action >> (i % 2)) & 0xff);
		d.tail.checkdigit[i + 4] = d.tail.checkdigit[i] ^ d.tail.checkdigit[i + 4];
		d.tail.checkdigit[i + 4] = d.tail.checkdigit[i + 4] ^ ((d.head.action >> (i % 2)) & 0xff);
	}

	/////////////////////////////////////////////////////////////////////
	/////////////////////���/////////////////////////////////////////////
	memcpy(pipe, &d, sizeof(d));

	/////////////////////////////////////////////////////////////////////
	/////////////////////����/////////////////////////////////////////////
	int ret = send(sock, pipe, sizeof(pipe), 0);
	
	if (ret == -1)
	{
		   cout << "send error: " << WSAGetLastError() << endl;
		   return 1;
	}
	
	recv(sock, (char*)&rep, sizeof(rep), 0);
	if (rep.action == 0)
	{
		cout << "\n\n���ѵ�¼�ɹ���" << endl;
		return 1;
	}
	else
	{
		cout << "\n\n�����û���������������������롣" << endl;
	}

	return 0;
}