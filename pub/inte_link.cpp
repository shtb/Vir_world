#include<sys/signal.h>
#include<sys/time.h>
#include "../../pub/inte_link.h"
#include"../base/interfaces.h"

using std::cout;
using std::cerr;
using std::endl;
using std::cin;
using std::thread;
using std::string;
using std::ios;
using std::ifstream;
using std::ofstream;

void illegal_killer(int sock,bool* got_c)
{
	timeval now;
	gettimeofday(&now,0);
	long t_n=now.tv_sec*1000000+now.tv_usec;
	long target=t_n+500000;
	while(t_n<target)
	{
		gettimeofday(&now,0);
		t_n=now.tv_sec*1000000+now.tv_usec;
		usleep(100);
	}
	if(!*got_c)
	{
		cerr << "\033[35m >WARNING: "<<"kill illegal link!"<<"\033[0m"<<endl;
		shutdown(sock,SHUT_RDWR);
	}
	delete got_c;
}

inte_link::inte_link()
{
	// ctor
	signal(SIGPIPE,SIG_IGN);
}

inline long get_pw()
{
	ifstream sc("/tmp/scode",ios::in);
	long pw=0;
	sc>>pw;
	sc.close();
	return pw;
}

void inte_link::accept_loop()
{
	sockaddr_in connect_addr;
	while(!t_save_sy)
	{
		socklen_t tem_len=sizeof(connect_addr);
		bool isfull=false;
		while(using_link>=limit_link)
		{
			isfull=true;
			usleep(10000);
		}
		if(isfull)
		{
			init();
			return;
		}
		int connect_socket = accept(server_socket, (struct sockaddr*)&connect_addr,&tem_len);
		if(connect_socket== -1)
		{
			cerr << "\033[31m >ERROR: "<<strerror(errno)<<"\033[0m"<<endl;
			continue;
		}
		using_link++;
		if(using_link>=limit_link)
		{
			close(server_socket);
		}
		struct timeval timeout = {0,10000};
		if(setsockopt(connect_socket,SOL_SOCKET,SO_SNDTIMEO,&timeout,sizeof(timeout))!=0)
		{
			cerr << "\033[35m >ERROR: "<<"connect opt set failed! :"<<strerror(errno)<<"\033[0m"<<endl;
			shutdown(connect_socket,SHUT_RDWR);
			continue;
		}
		char buff[16];
		inet_ntop(AF_INET, (void *)&connect_addr.sin_addr, buff, 16);
		string tem_s=buff;
		cout << "\033[34m >"<<tem_s+" linked!"<<"\033[0m"<<endl;
		//check legal
		{
			long re_pw=-1;
			bool* legal_check=new bool;
			*legal_check=false;
			thread con_kill(illegal_killer,connect_socket,legal_check);
			con_kill.detach();
			int get_n = recv(connect_socket,&re_pw, 8, 0);
			if(get_n != 8)
			{
				cerr << "\033[35m >WARNING: "<<"connect losed!"<<"\033[0m"<<endl;
				continue;
			}
			else
			{
				if(re_pw!=get_pw())
				{
					cerr << "\033[35m >WARNING: "<<"illegal connect!"<<"\033[0m"<<endl;
					continue;
				}
				else
				{
					*legal_check=true;
				}
			}
		}
		accept_one_link(connect_socket);
		thread recv_thread(&inte_link::begin_recv_loop,this,connect_socket);
		recv_thread.detach();
	}
	net_on=false;
	close(server_socket);
}

int inte_link::t_load()
{
	while(!init()&&!t_save_sy)
	{
		usleep(937892);
	}
	return 0;
}

int inte_link::t_loop()
{
	if(reload)
	{
		usleep(4231567);
		reload=false;
		t_load();
	}
	usleep(100000);
	return 0;
}

bool inte_link::init()
{
	if(ip_addr=="")
	{
		cout << "\033[34m >"<<"input the ip address:"<<"\033[0m"<<endl;
		cin >> ip_addr;
	}
	switch(work_mode)
	{
		case(Inte_server):
		{
			cout << "\033[34m >"<<"init internet server!"<<"\033[0m"<<endl;

			server_socket = socket(AF_INET, SOCK_STREAM, 0);
			if(server_socket < 0)
			{
				string s = strerror(errno);
				cerr << "\033[31m >ERROR: "<<"socket:" << s<<"\033[0m"<<endl;
				return false;
			}
			int sock_op=1;
			setsockopt( server_socket, SOL_SOCKET, SO_REUSEADDR, &sock_op, sizeof(sock_op) );
			server_addr.sin_family = AF_INET;

			inet_pton(AF_INET, ip_addr.c_str(), &server_addr.sin_addr);
			server_addr.sin_port = htons(port);
			char tem_ip[100];
			inet_ntop(AF_INET, &server_addr.sin_addr,tem_ip,100);
			cout<<"using:"<<tem_ip<<":"<<port<<"\033[0m"<<endl;

			if(::bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
			{
				string s = strerror(errno);
				cerr << "\033[31m >ERROR: "<<"bind:" <<s<<"\033[0m"<<endl;
				close(server_socket);
				return false;
			}
			if(listen(server_socket, 0) == -1)
			{
				string s = strerror(errno);
				cerr << "\033[31m >ERROR: "<<"listen:" << s<<"\033[0m"<<endl;
				close(server_socket);
				return false;
			}
			thread accept_thread(&inte_link::accept_loop, this);
			accept_thread.detach();
			cout << "\033[34m >"<<"Waiting for connect--"<<"\033[0m"<<endl;
			net_on = true;
			break;
		}
		case(Inte_client):
		{
			cout << "\033[34m >"<<"init internet client!"<<"\033[0m"<<endl;

			server_socket = socket(AF_INET, SOCK_STREAM, 0);
			if(server_socket < 0)
			{
				string s = strerror(errno);
				cerr << "\033[31m >ERROR: "<<"socket:" << s<<"\033[0m"<<endl;
				return false;
			}

			server_addr.sin_family = AF_INET;
			// server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
			inet_pton(AF_INET, ip_addr.c_str(), &server_addr.sin_addr);
			server_addr.sin_port = htons(port);
			char tem_ip[100];
			inet_ntop(AF_INET, &server_addr.sin_addr,tem_ip,100);
			cout<<"\033[0m"<<"link:"<<tem_ip<<":"<<port<<"\033[0m"<<endl;

			int result = connect(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
			if(result == -1)
			{
				string s = strerror(errno);
				my_print.error("connect:" + s);
				close(server_socket);
				return false;
			}
			else
			{
				my_print.message("connect succeed!");

				long pw=get_pw();
				int bytes = send(server_socket,&pw, 8, 0);
				if(bytes != 8)
				{
					cerr << "\033[31m >ERROR: "<<"send authentic message WRONG!"<<"\033[0m"<<endl;
				}
				net_on = true;
				thread recv_thread(&inte_link::begin_recv_loop, this, server_socket);
				recv_thread.detach();
				linked_to_server(server_socket);
			}
			break;
		}
		default:
			cerr << "\033[35m >WARNING: "<<"unkown internet work mode!"<<"\033[0m"<<endl;
			return false;
			break;
	}
	return true;
}

void inte_link::begin_recv_loop(int my_connect)
{
	char recv_buff[TRANS_MAX_LENGTH];

	std::stringstream source;
	unsigned int sour_len = 0;
	string choose;
	char head[7];
	unsigned int mess_len = -1;
	char* c_mess;
	std::stringstream s_mess;
	head[6] = '\0';
	int state = 0; // 0-ok,1-zhan,2-fen;
	int get_n;
	while(!t_save_sy&&net_on)
	{
		if(state == 0)
		{
			get_n = recv(my_connect, recv_buff, TRANS_MAX_LENGTH, 0);
			if(get_n <= 0)
			{
				my_print.warning("connect losed!");
				break;
			}
			source.write(recv_buff, get_n);
			while(source.str().length()- source.tellg()<10)
			{
				get_n = recv(my_connect, recv_buff, TRANS_MAX_LENGTH, 0);
				source.write(recv_buff, get_n);
			}
			source.read(head, 6);
			choose = head;
			source.read((char*)&mess_len, sizeof(int));
			// get-length
			sour_len = source.str().length();
			if(sour_len == mess_len + 10)
			{
				c_mess = new char[mess_len];
				source.read(c_mess, mess_len);
				s_mess.write(c_mess, mess_len);
				parse(choose, s_mess,my_connect);
				s_mess.clear();
				s_mess.str("");
				delete c_mess;
				source.clear();
				source.str("");
				sour_len = 0;
			}
			else if(sour_len > mess_len + 10)
			{
				state = 1;
			}
			else
			{
				state = 2;
			}
		}
		else if(state == 1)
		{
			while(sour_len > mess_len + 10)
			{
				c_mess = new char[mess_len];
				source.read(c_mess, mess_len);
				s_mess.write(c_mess, mess_len);
				parse(choose, s_mess,my_connect);
				s_mess.clear();
				s_mess.str("");
				delete c_mess;

				while(source.str().length()- source.tellg()<10)
				{
					get_n = recv(my_connect, recv_buff, TRANS_MAX_LENGTH, 0);
					source.write(recv_buff, get_n);
				}
				source.read(head, 6);
				choose = head;
				source.read((char*)&mess_len, sizeof(int));
				// get-length
				sour_len = source.str().length() - source.tellg() + 10;
			}
			if(sour_len == mess_len + 10)
			{
				c_mess = new char[mess_len];
				source.read(c_mess, mess_len);
				s_mess.write(c_mess, mess_len);
				parse(choose, s_mess,my_connect);
				s_mess.clear();
				s_mess.str("");
				delete c_mess;
				source.clear();
				source.str("");
				sour_len = 0;
				state = 0;
			}
			else
			{
				state = 2;
			}
		}
		else if(state == 2)
		{
			get_n = recv(my_connect, recv_buff, TRANS_MAX_LENGTH, 0);
			if(get_n <= 0)
			{
				my_print.warning("connect losed!");
				break;
			}
			source.write(recv_buff, get_n);
			sour_len = source.str().length() - source.tellg() + 10;
			if(sour_len == mess_len + 10)
			{
				c_mess = new char[mess_len];
				source.read(c_mess, mess_len);
				s_mess.write(c_mess, mess_len);
				parse(choose, s_mess,my_connect);
				s_mess.clear();
				s_mess.str("");
				delete c_mess;
				source.clear();
				source.str("");
				sour_len = 0;
				state = 0;
			}
			else if(sour_len > mess_len + 10)
			{
				state = 1;
			}
			else
			{
				state = 2;
			}
		}
		if(mess_len>1048576)
		{
			cout<<"ERROR:TOO LONG MESSAGE! "<<mess_len<<"\033[0m"<<endl;
			cout<<choose<<endl;
			break;
		}
	}
	end_one_link(my_connect);
	using_link--;
	close(my_connect);
}

bool inte_link::net_send(int socket,const char*data,size_t len)
{
	if(net_on)
	{
		size_t bytes = send(socket, data, len, 0);
		if(bytes!=len)
		{
			std::stringstream ss;
			ss<<strerror(errno);
			char p[7];
			p[6]=0;
			memcpy(p,data,6);
			ss<<" :"<<bytes<<"/"<<len<<"@"<<p;
			my_print.error("send:"+ss.str());
			shutdown(socket,SHUT_RDWR);
			if(work_mode==Inte_client)
				net_on=false;
			return false;
		}
		return true;
	}
	else
	{
		my_print.error("internet is not linked!");
		return false;
	}
}

inte_link::~inte_link()
{
	// dtor
}
