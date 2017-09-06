#include "myhead.h"

extern SOCKET sock;

int query::m_query(const long &id, const char* begin_date, const char* end_date)
{
	struct data d;
	//填充包头数据
	d.head.action = 6;
	d.head.len = sizeof(d.body) + sizeof(d.tail);
	//填充包体数据
	d.body.account_id = id;
	strcpy_s(d.body.begin_date, begin_date);
	strcpy_s(d.body.end_date, end_date);

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
	struct Data_rep data_rep;
	cout << "\n" << "您已查询成功，账户明细：" << endl;
	cout << setiosflags(ios::left) << setw(8) << "id"
		<< setiosflags(ios::left) << setw(10) << "save"
		<< setiosflags(ios::left) << setw(10) << "drow"
		<< setiosflags(ios::left) << setw(10) << "balance"
		<< setiosflags(ios::left) << setw(8) << "fromid"
		<< setiosflags(ios::left) << setw(19) << "date" << endl;
	while (1)
	{
		recv(sock, (char*)&data_rep, sizeof(data_rep), 0);
		if (data_rep.body_rep.total == 1)
		{
			if (data_rep.head_rep.action == 0)
			{
				cout << setiosflags(ios::left) << setw(8) << data_rep.body_rep.account_id
				 	 << setiosflags(ios::left) << setw(10) << data_rep.body_rep.save
					 << setiosflags(ios::left) << setw(10) << data_rep.body_rep.drow 
					 << setiosflags(ios::left) << setw(10) << data_rep.body_rep.balance 
					 << setiosflags(ios::left) << setw(8) << data_rep.body_rep.other_account_id
					 << setiosflags(ios::left) << setw(19) << data_rep.body_rep.date << endl;
			}
		}
		else
		{
			break;
		}
	}

	return 0;
}