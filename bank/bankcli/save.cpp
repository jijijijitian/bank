#include "myhead.h"

extern SOCKET sock;

int save::m_save(const long &id, const double &money)
{
	struct data d;
	//填充包头数据
	d.head.action = 3;
	d.head.len = sizeof(d.body) + sizeof(d.tail);
	//填充包体数据
	d.body.money = money;
	d.body.account_id = id;

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
	recv(sock, (char*)&data_rep, sizeof(data_rep), 0);
	if (data_rep.head_rep.action == 0)
	{
		cout << "\n" << "您已存款成功，账户明细：" << endl;
		cout << setiosflags(ios::left) << setw(10) << "name" 
			<< setiosflags(ios::left) << setw(10) << "balance" 
			<< setiosflags(ios::left) << setw(19) << "data" << endl;
		cout << setiosflags(ios::left) << setw(10) << data_rep.body_rep.name 
			<< setiosflags(ios::left) << setw(10) << data_rep.body_rep.balance 
			<< setiosflags(ios::left) << setw(19) << data_rep.body_rep.data << endl;
		return 2;
	}

	return 0;
}