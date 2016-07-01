#ifndef GRA_WORLD_LINK_H
#define GRA_WORLD_LINK_H
#include"../../pub/STDHF.h"
#include"../../pub/inte_link.h"

using namespace std;

class gra_world_link:public inte_link
{
public:
	void linked_to_server(int)
	{
		string tem="1234";
		send_to("get_ma",tem);
		send_to("login1",tem);
	}
	void end_one_link(int){reload=true;}
	void parse(std::string &choose, std::stringstream& ss,int connect);
	void send_to(std::string head,std::string& my_message);
	void send_to(std::string head,char* my_message, int length);
	gra_world_link();
	~gra_world_link();

};

#endif // GRA_WORLD_LINK_H
