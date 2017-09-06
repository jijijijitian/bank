#include "myhead.h"
SOCKET sock;
int do_client()
{
	//检查协议栈安装情况
	WORD wVersionRequsted = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(wVersionRequsted, &wsaData) != 0)                 //初始化错误
	{
		return 1;                                                    
	}
	if (wsaData.wVersion != wVersionRequsted)                        //Winsock版本不匹配
	{
		WSACleanup();                                                //清除socket
		return 1;                                                   
	}

	//创建用于通信的套接口
	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
	{
		cout << "socket error: " << WSAGetLastError() << endl;
		return 1;
	}
	cout << "socket success..." << endl;

	//给套接字绑定必要的信息
	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(8887);

	//连接服务器
	int ret = connect(sock, (sockaddr*)&addr, sizeof(addr));
	if (ret == -1)
	{
		cout << "connect error: " << WSAGetLastError() << endl;
		return 1;
	}
	cout << "connect success..." << endl;

	//循环发送与接收
	int back = 1, back1 = 1;

	while (back)
	{
		entershow();
		char choice[2] = { 0 }, choice1[2] = { 0 };
		cin >> choice;
		switch (choice[0])
		{ 
		case '1':
		{
			class enter* ent = new enter;
			char ent_usr[10];
			cout << "请输入用户名（3~10）：";
			cin >> ent_usr;
			char ent_pwd[8];
			cout << "请输入密码（6~8）：";
			int ii = 0;
			while ((ent_pwd[ii++] = _getch()) != '\r')
			{
				putchar('*');
			}
			ent_pwd[ii - 1] = '\0';
			if (ent->m_enter(ent_usr, ent_pwd) == 1)
			{
				int back1 = 1;
				while (back1)
				{
					show();
					cin >> choice1;
					switch (choice1[0])
					{
					case '1':
					{
						class opencont* ope = new opencont;
						char ope_name[16] = { 0 };
						char ope_passwd1[9] = { 0 };
						char ope_passwd2[9] = { 0 };
						char ope_id[19] = { 0 };
						double ope_money = 0;

						cout << "请输入姓名（3~10）：";
						cin >> ope_name;
						do
						{
							cout << "请输入密码（6~8）：";
							cin >> ope_passwd1;
							cout << "请再次输入密码（6~8）：";
							cin >> ope_passwd2;
						} while (strcmp(ope_passwd1, ope_passwd2) != 0);
						cout << "请输入身份证号（18位）：" ;
						cin >> ope_id;
						cout << "请输入开户金额：" ;
						cin >> ope_money;

						ope->m_open(ope_name, ope_passwd1, ope_id, ope_money);

						delete ope;
						break;
					}
					case '2':
					{
						class save* sav = new save;

						long sav_id;
						cout << "请输入ID：";
						cin >> sav_id;
						double sav_money = 0;
						cout << "请输入存款金额：";
						cin >> sav_money;

						sav->m_save(sav_id, sav_money);
						
						delete sav;
						break;
					}
					case '3':
					{
						class drow* dro = new drow;

						long dro_id = 0;
						cout << "请输入ID：";
						cin >> dro_id;
						char dro_pwd[9] = { 0 };
						cout << "请输入密码：";
						cin >> dro_pwd;
						double dro_money = 0;
						cout << "请输入取款金额：";
						cin >> dro_money;

						dro->m_drow(dro_id, dro_pwd, dro_money);

						delete dro;
						break;
					}
					case '4':
					{
						class transfer* tra = new transfer;

						long tra_id = 0;
						cout << "请输入ID：";
						cin >> tra_id;
						char tra_pwd[9] = { 0 };
						cout << "请输入密码：";
						cin >> tra_pwd;
						double tra_money = 0;
						cout << "请输入转账金额：";
						cin >> tra_money;
						long tra_toid = 0;
						cout << "请输入对方ID：";
						cin >> tra_toid;

						tra->m_transfer(tra_id, tra_pwd, tra_money, tra_toid);

						delete tra;
						break;
					}
					case '5':
					{
						class query* que = new query;

						long que_id = 0;
						cout << "请输入ID：";
						cin >> que_id;
						char que_date1[20] = { 0 };
						cout << "请输入起始日期：";
						cin >> que_date1;
						char que_date2[20] = { 0 };
						cout << "请输入结束日期：";
						cin >> que_date2;

						que->m_query(que_id, que_date1, que_date2);

						delete que;
						break;
					}
					case '6':
					{
						class change_passwd* cha = new change_passwd;

						long cha_id = 0;
						cout << "请输入ID：";
						cin >> cha_id;
						char cha_passwd[9] = { 0 };
						cout << "请输入密码：";
						cin >> cha_passwd;
						char cha_newpasswd[9] = { 0 };
						cout << "请输入新密码：";
						cin >> cha_newpasswd;

						cha->m_change_passwd(cha_id, cha_passwd, cha_newpasswd);

						delete cha;
						break;
					}
					case '7':
					{
						break;
					}
					case '8':
					{
						back1 = 0;
						break;
					}
					default:
					{
						cout << "请重新输入！" << endl;
						break;
					}
					}
				}
			}
			delete ent;
			break;
		}
		case '2':
		{
			back = 0;
			break; 
		}
		default:
		{
			cout << "请重新输入！" << endl;
			break;
		}
		}
	}
	
	WSACleanup();
	if (SOCKET_ERROR == closesocket(sock))
	{
		sock = INVALID_SOCKET;
	}

	return 0;
}

void show()
{
	printf("\n*************************************\n");
	printf("**            选择操作:            **\n");
	printf("**   <1> 开户         <2> 存款     **\n");
	printf("**   <3> 取款         <4> 转账     **\n");
	printf("**   <5> 查询         <6> 修改密码 **\n");
	printf("**   <7> 销户         <8> 退出     **\n");
	printf("*************************************\n\n");
}

void entershow()
{
	printf("\n************************************\n");
	printf("**            选择操作:           **\n");
	printf("**   <1> 登录         <2> 退出    **\n");
	printf("************************************\n\n");
}
