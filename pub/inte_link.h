#ifndef INTE_L
#define INTE_L

#include <iostream>
#include <thread>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <functional>
#include "../../pub/thread_control.h"
#include"../../pub/spinlock.h"

enum link_kind { Inte_server, Inte_client };

class inte_link : public thread_control
{
public:
	bool reload=false;
	spinlock send_lock;
	const static int TRANS_MAX_LENGTH = 40960;
	int server_socket;
	int work_mode=-1;
	const int broadcast_socket=0xffffffff;
	bool net_on = false;
	std::string ip_addr;
	int port;
	unsigned int limit_link=1;
	unsigned int using_link=0;

	inte_link();
	int t_load();
	int t_loop();
	bool init();
	void accept_loop();
	void begin_recv_loop(int connect);

	virtual void parse(std::string &choose, std::stringstream& ss,int)=0;
	virtual void linked_to_server(int){}
	virtual void accept_one_link(int){}
	virtual void end_one_link(int){}

	bool net_send(int socket,const char*data,size_t len);
	virtual ~inte_link();

protected:
private:
	sockaddr_in server_addr;
};

#endif // INTE_L
