#include "myhead.h"

int enter::m_enter(const char* usr, const char* passwd, const int &fd, const char* pipe, MYSQL* mysql)
{
	cout << "�ӿͻ���" << fd << "�յ���¼����" << endl;

	int act;
	memcpy(&act, pipe, 4);
	//��ȡУ����
	char cd1[9] = { 0 };
	char cd2[9] = { 0 };
	memcpy(cd2, pipe + 19, 9);
	memcpy(cd1, pipe + 28, 9);
	
	//��֤У�����Ƿ���ȷ
	for (int i = 7; i>3; i--)
	{
		cd1[i] = cd1[i] ^ cd1[i - 4];
		cd1[i] = cd1[i] ^ ((act >> (i % 2)) & 0xff);
		cd1[i - 4] = cd1[i] ^ cd1[i - 4];
		cd1[i - 4] = cd1[i - 4] ^ ((act >> (i % 2)) & 0xff);
	}
	
	if (strcmp(cd1, cd2) != 0)
	{
		cout << "���ݰ�������" << endl;
		return -1;
	}	

	//////////////////////////////////////////////////////////////////////
	//////////////////��ȡ���ݰ��ɹ������е�¼����
	if (!mysql_real_connect(mysql, "localhost", usr, passwd, "bank", 0, NULL, 0))
	{
		rep.action = 1;
		memcpy(&rep + 4, &pipe + 4, 4);
		rep.errornum = 1;
		strcpy_s(rep.errormsg, "�û������������");
		printf("connect error: %s\n", mysql_error(mysql));
		send(fd, (char*)&rep, sizeof(rep), 0);
		return -3;
	}
	else
	{
		cout << "��¼�ɹ���" << endl;
		rep.action = 0;
		send(fd, (char*)&rep, sizeof(rep), 0);

		return 0;
	}

}