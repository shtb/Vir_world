#ifndef CONSOLE_M
#define CONSOLE_M

#include"STDHF.h"
#include"spinlock.h"

class console
{
public:
	spinlock lock;

	std::stringstream ss_input;
	console();
	void message(std::string);
	std::stringstream&con_input();
	void warning(std::string s);
	void error(std::string s);

	void progress_bar(std::string note,long*did,long sum);

	~console();
};

#endif
