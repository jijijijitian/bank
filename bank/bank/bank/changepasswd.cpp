#include "myhead.h"

int change_passwd::m_change_passwd(const int &fd, const char* pipe, MYSQL* mysql)
{
	cout << "�ӿͻ���" << fd << "�յ��޸���������" << endl;

	struct data da;
	memcpy(&da.head.action, pipe, 4);
	memcpy(&da.body.account_id, pipe + 8, 4);
	memcpy(da.body.pass, pipe + 12, 9);
	memcpy(da.body.newpass, pipe + 21, 9);

	//////////////////////////////////////////////////////////////////////
	//////////////////��ȡ���ݰ��ɹ��������޸��������/////////////////////////////
	char buff1[1024];
	sprintf_s(buff1, "select passwd from register where id = %d;", da.body.account_id);
	int result = mysql_query(mysql, buff1);
	if (result != 0)
	{
		printf("select error: %s\n", mysql_error(mysql));
		return -2;
	}
	struct Head_rep rep;

	MYSQL_RES* mysql_res = NULL;
	MYSQL_ROW mysql_row = NULL;
	char passwd1[9] = { 0 };
	mysql_res = mysql_store_result(mysql);
	mysql_row = mysql_fetch_row(mysql_res);
	if (mysql_row)
	{	
		if (strcmp(da.body.pass, mysql_row[0]) == 0)
		{
			memset(buff1, 0, sizeof(buff1));
			sprintf_s(buff1, "update register set passwd = '%s' where id = %d;"
				, da.body.newpass, da.body.account_id);
			result = mysql_query(mysql, buff1);
			if (result != 0)
			{
				printf("select error: %s\n", mysql_error(mysql));
				return -2;
			}

			cout << "�޸ĳɹ���" << endl;
			rep.action = 0;
			send(fd, (char*)&rep, sizeof(rep), 0);
		}
		else
		{
			rep.action = 1;
			cout << "���벻��ȷ��" << endl;
			send(fd, (char*)&rep, sizeof(rep), 0);
		}
	}
	else
	{
		rep.action = 2;
		cout << "�û��������ڣ�" << endl;
		send(fd, (char*)&rep, sizeof(rep), 0);
	}
	mysql_free_result(mysql_res);

	return 0;
}