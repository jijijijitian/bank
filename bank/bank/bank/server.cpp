#include "myhead.h"

int do_server()
{
	//检查协议栈安装情况
	WORD wVersionRequsted = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(wVersionRequsted, &wsaData) != 0)                 //初始化错误
	{
		return -1;
	}
	if (wsaData.wVersion != wVersionRequsted)                        //Winsock版本不匹配
	{
		WSACleanup();                                                //清除socket
		return -1;
	}

	//创建用于通信的套接口
	SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
	{
		cout << "socket error: " << WSAGetLastError() << endl;
		return -1;
	}
	cout << "socket success..." << endl;

	//给套接字绑定必要的信息
	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(8887);

	//
	int ret = bind(sock, (sockaddr*)&addr, sizeof(addr));
	if (ret == SOCKET_ERROR)
	{
		cout << "bind error: " << WSAGetLastError() << endl;
		return -1;
	}
	cout << "bind sucess..." << endl;

	//
	ret = listen(sock, SOMAXCONN);
	if (ret == SOCKET_ERROR)
	{
		cout << "listen error: " << WSAGetLastError() << endl;
		return -1;
	}
	cout << "lsiten sucess..." << endl;

	//循环接收，读写
	SOCKET conn;
	sockaddr_in peeraddr;
	int peerlen;
	while (1)
	{
		//接受连接
		peerlen = sizeof(peeraddr);
		conn = accept(sock, (sockaddr*)&peeraddr, &peerlen);
		if (conn == INVALID_SOCKET)
		{
			cout << "accept error: " << WSAGetLastError() << endl;
			return -1;
		}
		cout << "accept success..."  << "客户端 = " << conn << endl;
		//cout << "客户端IP:" << inet_ntoa(peeraddr.sin_addr) << "  PORT:" << ntohs(peeraddr.sin_port) << endl;

		//recv接收
		DWORD dwThreadId;
		HANDLE hThread = CreateThread(NULL, 0, read_msg, (LPVOID)conn, 0, &dwThreadId);
		if (hThread == NULL)
		{
			cout << "CreateThread error: " << GetLastError() << endl;
			return -1;
		}
		CloseHandle(hThread);
	}

	WSACleanup();

	return 0;
}

DWORD WINAPI read_msg(LPVOID lpParameter)
{
	class bank ba;
	int fd = (int)lpParameter;
	int ret = 0;

	MYSQL* mysql;
	mysql = mysql_init(NULL);
	if (mysql == NULL)
	{
		cout << "init error:", mysql_error(mysql);
		return -2;
	}

	my_bool reconnect = true;
	mysql_options(mysql, MYSQL_OPT_RECONNECT, &reconnect);
	mysql_options(mysql, MYSQL_SET_CHARSET_NAME, "gbk");

	while (1)
	{
		memset(&ba, 0, sizeof(ba));

		int ret = recv(fd, ba.pipe, sizeof(ba.pipe), 0);

		if (ret == SOCKET_ERROR)
		{
			cout << "客户端" << fd << "关闭！" << endl;
			ExitThread(1);
			return -1;
		}
		if (ret > 0)
		{
			int choice = 0;
			memcpy(&choice, ba.pipe, 4);
			switch (choice)
			{
			case 1:
			{
				char usr[11], pwd[9]; 
				memcpy(usr, ba.pipe + 8, 11);
				memcpy(pwd, ba.pipe + 19, 9);
				class enter ent;
				int judge = ent.m_enter(usr, pwd, fd, ba.pipe, mysql);
				if (judge == -3)
				{
					mysql_close(mysql);
					
					mysql = mysql_init(NULL);
					if (mysql == NULL)
					{
						cout << "init error:", mysql_error(mysql);
						return -2;
					}

					my_bool reconnect = true;
					mysql_options(mysql, MYSQL_OPT_RECONNECT, &reconnect);
					mysql_options(mysql, MYSQL_SET_CHARSET_NAME, "gbk");
				}
				break;
			}
			case 2:
			{
				class opencont ope;
				ope.m_open(fd, ba.pipe, mysql);
				break;
			}
			case 3:
			{
				class save sav;
				sav.m_save(fd, ba.pipe, mysql);
				break;
			}
			case 4:
			{
				class drow dro;
				dro.m_drow(fd, ba.pipe, mysql);
				break;
			}
			case 5:
			{
				class transfer tra;
				tra.m_transfer(fd, ba.pipe, mysql);
				break;
			}
			case 6:
			{
				class query que;
				que.m_query(fd, ba.pipe, mysql);
				break;
			}
			case 7:
			{
				class change_passwd cha;
				cha.m_change_passwd(fd, ba.pipe, mysql);
				break;
			}
			default:
			{
				break;
			}
			}
			//send(conn, buf, strlen(buf), 0);
		}
	}

	mysql_close(mysql);
	if (SOCKET_ERROR == closesocket(fd))
	{
		fd = INVALID_SOCKET;
	}
}
