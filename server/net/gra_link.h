#ifndef GRA_LINK_H
#define GRA_LINK_H
#include"../../pub/STDHF.h"
#include"../../pub/inte_link.h"
#include"../base/world_structs.h"
#include"../../pub/spinlock.h"

using namespace std;

class gra_link:public inte_link
{
public:
	std::set<gra_client*>gra_clients;
	spinlock gra_clients_lock;
	const int shtb_socket=0xfffffffe;
	const int mulin_socket=0xfffffffd;
	const int shtb_and_unknown_socket=0xfffffffc;
	const int shtb_and_mulin_socket=0xfffffffb;

	void accept_one_link(int sock_id);
	void end_one_link(int sock_id);
	void parse(std::string &choose, std::stringstream& ss,int connect);
	void send_to(std::string head,std::string& my_message, int socket);
	void send_to(std::string head,std::stringstream& my_message, int socket);

	gra_link();
	~gra_link();

};

#endif // GRA_LINK_H
